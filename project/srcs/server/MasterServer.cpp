#include "MasterServer.hpp"

/*
** ---------------------------------- STATIC ----------------------------------
*/

static void clean_fd(t_fd *fd)
{
    fd->type = FD_FREE;
    fd->host = NO_HOST;
    fd->fct_read = NULL;
    fd->fct_write = NULL;
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

MasterServer::MasterServer()
{}

MasterServer::MasterServer(const MasterServer & src): AServerItem()
{
    if (this != &src)
        *this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

MasterServer::~MasterServer()
{
	for (unsigned int index = 0; index < this->_configAllServer.size(); index++)
		delete this->_configAllServer[index];
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

MasterServer & MasterServer::operator=(const MasterServer & rhs)
{
    this->_configAllServer = rhs._configAllServer;
    return (*this);
}


AServerItem *MasterServer::consume(Node *node)
{
	(void)node;
	return this->createServer();
}

/*
** --------------------------------- PUBLIC METHODS ----------------------------------
*/

OneServer *MasterServer::createServer()
{
	this->_configAllServer.push_back(new OneServer());
	return this->_configAllServer[this->_configAllServer.size() - 1];
}


std::ostream &			MasterServer::print( std::ostream & o) const
{
	o << "I'm Master Server !" << std::endl;
	for (size_t i = 0; i < this->_configAllServer.size(); i++)
		o << *(this->_configAllServer[i]) << std::endl;
	
	return o;
}

int	MasterServer::build()
{
    /*************************************************************/
    /* Initialize the master fd_set                              */
    /*************************************************************/
    init_env();
    get_server_ready();
    return 0;
}



void MasterServer::run() // ! do like main_loops
{
    /*************************************************************/
    /* Loop waiting for incoming connects or for incoming data   */
    /* on any of the connected sockets.                          */
    /*************************************************************/
    while (1)
    {
        init_fd();
        do_select();
        check_fd();
    }
}

/*
** --------------------------------- PRIVATE METHODS ----------------------------------
*/

void MasterServer::init_env()
{
    int     i;

    /************************************************************
     * Get maximum number of fd that computer can handle at one time
    LIMIT_NOFILE - The maximum number of file descriptors that the process may have open at one time.
    ************************************************************/
    // struct rlimit rlp;
    // if (getrlimit(RLIMIT_NOFILE, &rlp) == -1)
    //     return ; // or throw something
    // this->_maxFd = rlp.rlim_cur;

    /************************************************************
     * Get maximum number of fd that computer can handle at one time
    (to do at school)
    I do this because at school the system call getrlimit freezes the computer.
    Otherwise we should do as above.
    ************************************************************/
    // ! can handle 50 fds at the same time. 
    _maxFd = 50;

    /************************************************************/
    i = 0;
    while (i < this->_maxFd)
    {
        t_fd    new_fd;

        clean_fd(&new_fd);
        this->_fdSet.push_back(new_fd);
        i++;
    }

    std::cout << "\n_maxFD: " << _maxFd;
    std::cout << "\nSize of fdSet: " << _fdSet.size() << std::endl;
}

void MasterServer::get_server_ready()
{

    for (unsigned long i = 0; i < _configAllServer.size(); i++)
    {
        int                 s;
        struct sockaddr_in  sin;
        t_listen config_listen = _configAllServer[i]->getListen();
        int rc, on = 1;

        /************************************************************
        * Create an AF_INET stream socket to receive incoming       
        * connections on
        * If PROTOCOL is zero, one is chosen automatically.
        * Returns a file descriptor for the new socket, or -1 for errors.                                            
        *************************************************************/
        s = socket(AF_INET, SOCK_STREAM, 0);
        if (s == 0)
        {
            std::cerr << "Fail to set socket" << std::endl;
            return ;
        }

        /*************************************************************/
        /* Allow socket descriptor to be reuseable                   */
        /*************************************************************/
        rc = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
        if (rc < 0)
        {
            std::cerr << "setsockopt() failed" << std::endl;
            return ;
        }

        /*************************************************************/
        /* Set address (host) and port                               */
        /*************************************************************/
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = htonl(config_listen._address);
        sin.sin_port = htons(config_listen._port);

        /*************************************************************/
        /* Bind the socket                                           */
        /*************************************************************/
        rc = bind(s, (struct sockaddr *)&sin, sizeof(sin));
        if (rc < 0)
        {
            std::cerr << "Fail to bind to port " << config_listen._port << std::endl;
            return ;
        }

        /*************************************************************/
        /* Try to specify maximun of client pending connection for   */
        /*   the master socket (server_fd)                           */
        /*************************************************************/
        rc = listen(s, MAX_CLIENTS);
        if (rc < 0)
        {
            std::cerr << "Fail to listen" << std::endl;
            return ;
        }

        _fdSet[s].type = FD_SERV;
        _fdSet[s].host = config_listen._port;
        _fdSet[s].fct_read = &MasterServer::server_accept;
    }
}

void MasterServer::server_accept(int s)
{
    int cs;
    struct sockaddr_in csin;
    socklen_t csin_len;

    csin_len = sizeof(csin);
    cs = accept(s, (struct sockaddr*)&csin, &csin_len);
    printf("New client #%d from %s:%d\n", cs, inet_ntoa(csin.sin_addr), ntohs(csin.sin_port));
    clean_fd(&_fdSet[cs]);
    _fdSet[cs].type = FD_CLIENT;
    _fdSet[cs].host = _fdSet[cs].host;
    _fdSet[cs].fct_read = &MasterServer::client_read;
    _fdSet[cs].fct_write = &MasterServer::client_write;
}

void MasterServer::init_fd()
{
    int i;

    i = 0;
    this->_max = 0;
    FD_ZERO(&this->_fdRead);
    FD_ZERO(&this->_fdWrite);
    while (i < this->_maxFd)
    {
        if (this->_fdSet[i].type != FD_FREE)
        {
            FD_SET(i, &this->_fdRead);
            if (strlen(this->_fdSet[i].buf_write) > 0)
                FD_SET(i, &this->_fdWrite);
            this->_max = MAX(_max, i);
        }
        i++;
    }
}

void MasterServer::do_select()
{
    struct timeval      timeout;

    /*************************************************************/
    /* Initialize the timeval struct to 3 minutes.               */
    /*************************************************************/
    timeout.tv_sec = 3 * 60;
    timeout.tv_usec = 0;

    /*************************************************************/
    /* Call select() and wait 3 minutes for it to complete.      */
    /* Wait for one or more fd become "ready" to read and write  */
    /*************************************************************/
    this->_r = select(this->_max + 1, &this->_fdRead, &this->_fdWrite, NULL, &timeout);

    /**********************************************************/
    /* Check to see if the select call failed.                */
    /**********************************************************/
    if (this->_r < 0)
    {
        std::cout << "select() failed" << std::endl;
        return ; // ! throw something
    }

    /**********************************************************/
    /* Check to see if the 3 minute time out expired.         */
    /**********************************************************/
    if (this->_r == 0)
    {
        std::cerr << "select() time out. End program." << std::endl;
        // ! close / clear opened socket
    }
}


void MasterServer::client_read(int fd)
{
    int r;
    int i;

    r = recv(fd, _fdSet[fd].buf_read, BUF_SIZE, 0);
    printf("buf_read =\n%s\n", _fdSet[fd].buf_read);
    if (r <= 0)
    {
        close(fd);
        clean_fd(&_fdSet[fd]);
        printf("Client #%d gone away\n", fd);
        
    }
    else
    {
        i = 0;
        while (i < _maxFd)
        {
            if((_fdSet[i].type == FD_CLIENT) && (i != fd) && (_fdSet[i].host == _fdSet[fd].host))
                send(i, _fdSet[fd].buf_read, r, 0);
        }
    }
}

void MasterServer::client_write(int fd)
{
    std::cout << "My fd is: " << fd << std::endl;

}

void MasterServer::check_fd()
{
    int i;

    i = 0;
    while ((i < _maxFd) && (_r > 0))
    {
        if (FD_ISSET(i, &_fdRead))
            (this->*_fdSet[i].fct_read)(i);
        if (FD_ISSET(i, &_fdWrite))
            (this->*_fdSet[i].fct_write)(i);
        if (FD_ISSET(i, &_fdRead) || FD_ISSET(i, &_fdWrite))
            _r--;
        i++;
    }
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */ 

