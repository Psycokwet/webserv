#include "MasterServer.hpp"

/*
** ---------------------------------- STATIC ----------------------------------
*/

static void init_fd(t_fd *fd)
{
	bzero(fd->buf_read, BUF_SIZE + 1);
	bzero(fd->buf_write, BUF_SIZE + 1);
	fd->type = FD_FREE;
	fd->host = NO_HOST;
	fd->fct_read = NULL;
	fd->fct_write = NULL;
	fd->parser = NULL;
}

static void clean_fd(t_fd *fd)
{
	if (fd->parser)
		delete fd->parser;
	init_fd(fd);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

MasterServer::MasterServer()
{
	_base_request_parser = GrammarParser::build(GRAMMAR_FILE);
	if (!_base_request_parser)
		throw BuildError();
}

MasterServer::MasterServer(const MasterServer &src) : AServerItem()
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
	for (std::vector<t_fd>::iterator it = this->_fdSet.begin(); it != this->_fdSet.end(); it++)
		clean_fd(&(*it));
	if (_base_request_parser)
		delete _base_request_parser;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

MasterServer &MasterServer::operator=(const MasterServer &rhs)
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

std::ostream &MasterServer::print(std::ostream &o) const
{
	o << "I'm Master Server !" << std::endl;
	for (size_t i = 0; i < this->_configAllServer.size(); i++)
		(this->_configAllServer[i])->print(o) << std::endl;

	return o;
}

int MasterServer::build()
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
    std::string command;
    std::cout << "Your Server is running ... \nInsert 'exit' to stop server." << std::endl;
    /*************************************************************/
    /* Loop waiting for incoming connects or for incoming data   */
    /* on any of the connected sockets.                          */
    // /*************************************************************/
    while (1)
    {
        std::getline(std::cin, command);
        if (command.compare("exit") != 0)
        {
            std::cout << "Command is invalid. Insert 'exit' to stop server." << std::endl;
            continue ;
        }
        else
            break ;
        init_fd(); //! Select fd that are not FD_FREE. Set it to _fdRead in default. if that fd has len (buf_write) > 0, it will be set to _fdWrite
        do_select(); // ! select if fd is type READ or WRITE, set them in _fdRead or _fdWrite
        check_fd(); // ! run through the _fdSet, if fd is on _fdRead, call fct_read, if it is on _fdWrite call fct_write
    }
    std::cout << "Exit program" << std::endl;
}

AServerItem *MasterServer::findTheFirstServerItemWith(AServerItem *(*test)(AServerItem *, void *), void *datas)
{
	AServerItem *result = NULL;
	for (std::vector<OneServer *>::iterator it = _configAllServer.begin(); it != _configAllServer.end(); it++)
	{
		result = test(*it, datas);
		if (result)
			return result;
	}
	return NULL;
}
/*
** --------------------------------- PRIVATE METHODS ----------------------------------
*/

void MasterServer::init_env()
{
	int i;
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
		t_fd new_fd;

		init_fd(&new_fd);
		this->_fdSet.push_back(new_fd);
		i++;
	}
}

int MasterServer::get_server_ready()
{
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
                throw RepeatPort();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
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
        if (DEBUG){
            std::cout << "Socket created is: " << s << std::endl;
        }
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
    if (DEBUG)
        std::cout << "fd after accept is: " << cs << std::endl;
    
    printf("New client #%d from %s:%d\n", cs, inet_ntoa(csin.sin_addr), ntohs(csin.sin_port));
	init_fd(&_fdSet[cs]);
    _fdSet[cs].type = FD_CLIENT;
    _fdSet[cs].host = _fdSet[cs].host;
    _fdSet[cs].fct_read = &MasterServer::client_read;
    _fdSet[cs].fct_write = &MasterServer::client_write;
	_fdSet[cs].parser = new GrammarParser(*_base_request_parser);
}

void MasterServer::init_fdSet()
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
	struct timeval timeout;

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
		return; // ! throw something
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
	ResponseBuilder *resp;
	// int i;

	// Receive request
	r = recv(fd, _fdSet[fd].buf_read, BUF_SIZE, 0);
	_fdSet[fd].parser->feed(_fdSet[fd].buf_read);
	std::cout << "buf_read =\n"
			  << _fdSet[fd].buf_read << "\n";
	bzero(_fdSet[fd].buf_read, BUF_SIZE + 1);
	if (r == BUF_SIZE)
	{
		std::cout << "client read incomplete \n";
		_fdSet[fd].parser->parse();
		return;
	}

	if (r <= 0)
	{
		close(fd);
		clean_fd(&_fdSet[fd]);
		std::cout << "client #" << fd << " left\n";
		return;
	}
	else if ((resp = _fdSet[fd].parser->finishParse()) == NULL)
	{
		std::cout << "THIS SHOULD NOT HAPPEN EVER, SOMETHING IS VERY WRONG\n";
	}
	else
	{
		// i = 0;
		// while (i < _maxFd)
		// {
		//     if((_fdSet[i].type == FD_CLIENT) && (i != fd) && (_fdSet[i].host == _fdSet[fd].host)) // ! need to work on this conditions, is sending response to all clients?
		//     {
		//         // char src[1000]; // will be response
		//         // char dest[1000]; // will be buf_read
		//         // strcpy(src, "HTTP/1.1 200 OK\nDate:Fri, 16 Mar 2020 17:21:12 GMT\nServer: my_server\nContent-Type: text/html;charset=UTF-8\nContent-Length: 1846\n\n<!DOCTYPE html>\n<html><h1>Hello world</h1></html>\n");
		//         // strcpy(dest, src);
		//         // send(i, dest, strlen(dest), 0);

		//         // Send Response based on Request

		resp->execute(this);
		std::string finalResponsefake =
			streamFunctionToString(&ResponseBuilder::print_response, resp);
		std::cout
			<< "finalResponsefake" << std::endl
			<< finalResponsefake << std::endl;
		std::string finalResponse = "HTTP/1.1 200 OK\nDate:Fri, 16 Mar 2020 17:21:12 GMT\nServer: my_server\nContent-Type: text/html;charset=UTF-8\nContent-Length: 1846\n\n<!DOCTYPE html>\n<html><h1>Hello world</h1></html>\n";
		send(fd, finalResponsefake.c_str(), finalResponsefake.size(), 0);
		//     }
		//     i++;
		// }
	}
	std::cout << "client read finish " << r << "\n\n";
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
