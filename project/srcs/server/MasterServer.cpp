#include "MasterServer.hpp"

/*
** ---------------------------------- STATIC ----------------------------------
*/

static void clean_fd(t_fd *fd)
{
    bzero(fd->buf_read, BUF_SIZE + 1);
    bzero(fd->buf_write, BUF_SIZE + 1);
    fd->type = FD_FREE;
    fd->host = NO_HOST;
    fd->fct_read = NULL;
    fd->fct_write = NULL;
    fd->parser = NULL;
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

MasterServer::MasterServer()
{
	_base_request_parser =	GrammarParser::build(GRAMMAR_FILE);
	if(!_base_request_parser)
		throw BuildError();
}

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
    init_env(); // ! set a vector of _fdSet, set all fd is FD_FREE
    if (get_server_ready() == EXIT_FAILURE) // ! Set up fd socket for each server, change FD_FREE to FD_SERV
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}



void MasterServer::run() // ! do like main_loops
{
    // std::string command;
    // std::cout << "Your Server is running ... \nInsert 'exit' to stop server." << std::endl;
    /*************************************************************/
    /* Loop waiting for incoming connects or for incoming data   */
    /* on any of the connected sockets.                          */
    // /*************************************************************/
    while (1)
    {
        // std::getline(std::cin, command);
        // if (command.compare("exit") != 0)
        // {
        //     std::cout << "Command is invalid. Insert 'exit' to stop server." << std::endl;
        //     continue ;
        // }
        // else
        //     break ;
        init_fd(); //! Select fd that are not FD_FREE. Set it to _fdRead in default. if that fd has len (buf_write) > 0, it will be set to _fdWrite
        do_select(); // ! select if fd is type READ or WRITE, set them in _fdRead or _fdWrite
        check_fd(); // ! run through the _fdSet, if fd is on _fdRead, call fct_read, if it is on _fdWrite call fct_write
    }
}

/*
** --------------------------------- PRIVATE METHODS ----------------------------------
*/

void MasterServer::init_env()
{
    int     i;
    /**************************************************************************
    ! Do not use this, it causes freeze in computer.
    RLIMIT_NOFILE - The maximum number of file descriptors that the process may have open at one time.
    *************************************************************************
    struct rlimit rlp;
    if (getrlimit(RLIMIT_NOFILE, &rlp) == -1)
        return ; // or throw something
    this->_maxFd = rlp.rlim_cur;

    **************************************************************************/
    this->_maxFd = FD_MAX; // ! use this instead of above method.
    i = 0;
    while (i < this->_maxFd)
    {
        t_fd    new_fd;

        clean_fd(&new_fd);
        this->_fdSet.push_back(new_fd);
        i++;
    }
}

int MasterServer::get_server_ready()
{
    /*************************************************************
     * Check for repated port.
    *************************************************************/
    int server_size = _configAllServer.size();
    try
    {
        std::set<int> port_set;
        std::pair<std::set<int>::iterator,bool> ret;
        for (int i = 0; i < server_size; i++)
        {
            t_listen config_listen = _configAllServer[i]->getListen();
            ret = port_set.insert(config_listen._port);
            if (ret.second == false)
                throw RepeatedPort();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    /*************************************************************
     * Setup Server.
    *************************************************************/
    for (int i = 0; i < server_size; i++)
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
        
        std::cout << "Socket created is: " << s << std::endl;

        if (s == 0)
        {
            std::cerr << "Fail to set socket" << std::endl;
            return EXIT_FAILURE;
        }

        /*************************************************************/
        /* Allow socket descriptor to be reuseable                   */
        /*************************************************************/
        rc = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
        if (rc < 0)
        {
            std::cerr << "setsockopt() failed" << std::endl;
            return EXIT_FAILURE ;
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
            return EXIT_FAILURE ;
        }

        /*************************************************************/
        /* Try to specify maximun of client pending connection for   */
        /*   the master socket (server_fd)                           */
        /*************************************************************/
        rc = listen(s, MAX_CLIENTS);
        if (rc < 0)
        {
            std::cerr << "Fail to listen" << std::endl;
            return EXIT_FAILURE;
        }

        _fdSet[s].type = FD_SERV;
        _fdSet[s].host = config_listen._port;
        _fdSet[s].fct_read = &MasterServer::server_accept;
		_fdSet[s].parser = NULL;
    }
    return EXIT_SUCCESS;
}

void MasterServer::server_accept(int s)
{
    int cs;
    struct sockaddr_in csin;
    socklen_t csin_len;

    csin_len = sizeof(csin);
    cs = accept(s, (struct sockaddr*)&csin, &csin_len);

    std::cout << "fd after accept is: " << cs << std::endl;
    
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
    /* Initialize the timeval struct to 1 minutes.               */
    /*************************************************************/
    timeout.tv_sec = 1 * 60;
    timeout.tv_usec = 0;

    /*************************************************************/
    /* Call select() and wait 1 minutes for it to complete.      */
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
    /* Check to see if the 1 minute time out expired.         */
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

    //Receive request
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
            if((_fdSet[i].type == FD_CLIENT) && (i != fd)) // ! need to work on this conditions, is sending response to all clients?
            {
                printf(">>>>>>>>>>>>>>>>>> HERE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
                // char src[1000]; // will be response
                // char dest[1000]; // will be buf_read
                // strcpy(src, "HTTP/1.1 200 OK\nDate:Fri, 16 Mar 2020 17:21:12 GMT\nServer: my_server\nContent-Type: text/html;charset=UTF-8\nContent-Length: 1846\n\n<!DOCTYPE html>\n<html><h1>Hello world</h1></html>\n");
                // strcpy(dest, src);
                // send(i, dest, strlen(dest), 0);
                char response_from_server[] = "HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 88\nContent-Type: text/html\nConnection: Closed\n\n\n<html>\n<body>\n<h1>Hello World!</h1>\n</body>\n</html>\n";
                // Send Response based on Request
                // send(fd, _fdSet[fd].buf_read, strlen(_fdSet[fd].buf_read), 0);
                send(fd, response_from_server, strlen(response_from_server), 0);
                close(fd);
            }
            i++;
        }
    }
    printf("client read finish\n\n");

}

void MasterServer::client_write(int fd)
{
    std::cout << "My fd is: " << fd << std::endl;

}

void MasterServer::check_fd()
{
    int i;
    int fd_rest = _r;

    i = 0;
    while ((i < _maxFd) && (fd_rest > 0))
    {
        if (FD_ISSET(i, &_fdRead))
            (this->*_fdSet[i].fct_read)(i);
        if (FD_ISSET(i, &_fdWrite))
            (this->*_fdSet[i].fct_write)(i);
        if (FD_ISSET(i, &_fdRead) || FD_ISSET(i, &_fdWrite))
            fd_rest--;
        i++;
        
        printf("i = %d; fd_rest = %d\n", i, fd_rest);
    }
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */ 

