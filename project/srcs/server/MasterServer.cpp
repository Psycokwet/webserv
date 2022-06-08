#include "MasterServer.hpp"

/*
** ---------------------------------- STATIC ----------------------------------
*/


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

MasterServer::MasterServer()
: _maxFD(-1)
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
    int opt = TRUE;
    int server_size = _configAllServer.size();

    for (int i = 0; i < server_size; i++)
    {
		int fdServ;
		struct sockaddr_in address;
        t_listen config_listen = _configAllServer[i]->getListen();

        /************************************************************
        * Create an AF_INET stream socket to receive incoming       
        * connections on
        * If PROTOCOL is zero, one is chosen automatically.
        * Returns a file descriptor for the new socket, or -1 for errors.                                            
        *************************************************************/
		fdServ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
		if (fdServ == -1)
        {
            std::cerr << "Fail to set socket" << std::endl;
            return EXIT_FAILURE;
        }

        /*************************************************************/
        /* Allow socket descriptor to be reuseable                   */
        /*************************************************************/
		if (setsockopt(fdServ, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&opt, sizeof(opt)) == -1)
		{
            std::cerr << "setsockopt() failed" << std::endl;
            return EXIT_FAILURE ;
        }

        
        /*************************************************************/
        /* Set address (host) and port                               */
        /*************************************************************/
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(config_listen._port);

		/*************************************************************/
        /* Bind the socket                                           */
        /*************************************************************/
		if (bind(fdServ, (sockaddr *)&address, sizeof(address)) == -1)
        {
            std::cerr << "Fail to bind to port " << config_listen._port << std::endl;
            return EXIT_FAILURE ;
        }

        /*************************************************************/
        /* Try to specify maximun of client pending connection for   */
        /*   the master socket (server_fd)                           */
        /*************************************************************/
		if (listen(fdServ, MAX_CLIENT_QUEUE) == -1)
        {
            std::cerr << "Fail to listen" << std::endl;
            return EXIT_FAILURE;
        }

		std::cout	<< "Listening on port "
					<< config_listen._port
					<< std::endl;

		std::pair<std::set<int>::iterator,bool> ret;
		ret = _fdServer.insert(fdServ);               
		if (ret.second==false)
        {
            std::cerr << "Repeated Ports" << std::endl;
            return EXIT_FAILURE;
        }

		_fdMap[fdServ]; 

		_maxFD = MAX(_maxFD, fdServ);
	}
	std::cout << "mymap now contains " << _fdMap.size() << " elements.\n";
    return EXIT_SUCCESS;
}



void MasterServer::run() // ! do like main_loops
{
    while (TRUE)
    {
        FD_ZERO(&_fdReader);
        setFDForReading();
        recvProcessCommand();
    }
}

/*
** ------------------------- PRIVATE METHODS ----------------------------------
*/

void	MasterServer::setFDForReading()
{
	std::map<int, std::set<int> >::iterator it;
	for (it = _fdMap.begin(); it!=_fdMap.end(); ++it)
	{
		int 			fdServer = it->first;
		std::set<int> 	fdClientSet = it->second;

		_maxFD = MAX(_maxFD, fdServer);
		FD_SET(fdServer, &_fdReader);

		std::set<int>::iterator	clientIter;
		for (clientIter = fdClientSet.begin(); clientIter != fdClientSet.end(); ++clientIter) // chay qua tung thang client trong client set cua tung server
		{
			int	clientFD = *clientIter;
			FD_SET(clientFD, &_fdReader);
			_maxFD = MAX(_maxFD, clientFD);
		}
	}

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
	int new_r;
	new_r = select(_maxFD + 1, &_fdReader, NULL, NULL, &timeout);
	if (new_r == -1)
		SERVER_ERR("select");
    if (new_r == 0)
		SERVER_ERR("Select: Time Out");
        
	_numberOfReadyFd = MAX(_numberOfReadyFd, new_r);
}


void	MasterServer::recvProcessCommand()
{
	// Checking each socket for reading, starting from FD 3 because there should be nothing
	// to read from 0 (stdin), 1 (stdout) and 2 (stderr)
	for (int fd = 3; fd <= _maxFD && _numberOfReadyFd; ++fd)
	{
		if (FD_ISSET(fd, &_fdReader))
		{
			if (_fdServer.count(fd) == 1) // if fd is Server
				acceptClient(fd);
			else // if fd is Client
			{
				char http_response[] = "HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 88\nContent-Type: text/html\nConnection: Closed\n\n\n<html>\n<body>\n<h1>Hello, World!</h1>\n</body>\n</html>\n";  

				char http_request[100000];
				int valread;
				if ((valread = recv( fd , http_request, 100000, 0)) == 0) // If receive nothing from clients
				{  
					
					//Close the socket and erase it from fd of clients
					close(fd);
					int fdServ = findFdServer(fd);  
					_fdMap[fdServ].erase(fd);
				}  
				else // send response
				{  
					http_request[valread] = '\0';  
					printf("Request Received from client:\n--------------\n%s\n", http_request);
					send(fd , http_response , strlen(http_response) , 0 );
					std::cout << "A http response is sent\n" ;

					//Close the socket and erase it from fd of clients
					close (fd);
					int fdServ = findFdServer(fd);
					_fdMap[fdServ].erase(fd);
				}  
			}
			--_numberOfReadyFd;
		}
	}
}

void	MasterServer::acceptClient(int fdServer)
{
	sockaddr_in	sin;
	socklen_t	sin_len = 0;

	int	clientFD = accept(fdServer, (sockaddr *)&sin, &sin_len);
	if (clientFD == -1)
	{
		std::cerr << "Failed to accept a new connection\n";
		return;
	}
	std::cout 	<< "New client on socket #" << clientFD 
				<< ". This socket belongs to Server at socket #" << fdServer
				<< std::endl;
	_fdMap[fdServer].insert(clientFD);
}


int	MasterServer::findFdServer(int value)
{
	int fdServer = -1;

	std::map< int, std::set<int> >::iterator it;
	
	for (it = _fdMap.begin(); it != _fdMap.end(); ++it)
	{
		if (it->second.count(value) == 1)
		{
			fdServer = it->first;
			break; // to stop searching
		}
   }
   return fdServer;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */ 

