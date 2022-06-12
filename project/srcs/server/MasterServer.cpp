#include "MasterServer.hpp"

/*
** ---------------------------------- STATIC ----------------------------------
*/


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

MasterServer::MasterServer()
: _maxFD(-1), _numberOfReadyFd(-1)
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
    int opt = TRUE;
    int server_size = _configAllServer.size();

	std::set<int> port_set;
	std::pair<std::set<int>::iterator,bool> ret;
	for (int i = 0; i < server_size; i++)
	{
		t_listen config_listen = _configAllServer[i]->getListen();
		ret = port_set.insert(config_listen._port);
		if (ret.second == false)
		{
			std::cout << "Duplicate Ports" << std::endl;
			return EXIT_FAILURE;
		}
    }

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
		address.sin_addr.s_addr = htonl(config_listen._address);
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

		_fdServer.insert(fdServ);               

		// std::pair< OneServer*, std::map <int, GrammarParser> > fd_map_value;
		std::pair< OneServer*, std::map <int, std::string> > fd_map_value;
		fd_map_value.first = _configAllServer[i];
		_fdMap[fdServ] = fd_map_value; 

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
        recvProcess();
    }
}

/*
** ------------------------- PRIVATE METHODS ----------------------------------
*/

void	MasterServer::setFDForReading()
{
	//  std::map<int, std::pair<OneServer*, std::map <int, GrammarParser> > >::iterator it;
	 std::map<int, std::pair<OneServer*, std::map <int, std::string> > >::iterator it;
	for (it = _fdMap.begin(); it!=_fdMap.end(); ++it)
	{
		int 			fdServer = it->first;
		// std::map <int, GrammarParser> 	fdClientMap = it->second.second;
		std::map <int, std::string> 	fdClientMap = it->second.second;

		_maxFD = MAX(_maxFD, fdServer);
		FD_SET(fdServer, &_fdReader);

		// std::map <int, GrammarParser>::iterator	clientIter;
		std::map <int, std::string>::iterator	clientIter;
		for (clientIter = fdClientMap.begin(); clientIter != fdClientMap.end(); ++clientIter)
		{
			int	clientFD = clientIter->first;
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


void	MasterServer::recvProcess()
{
	// Checking each socket for reading, starting from FD 3 because there should be nothing
	// to read from 0 (stdin), 1 (stdout) and 2 (stderr)
	for (int fd = 3; fd <= _maxFD && _numberOfReadyFd; ++fd)
	{
		if (FD_ISSET(fd, &_fdReader))
		{
			if (_fdServer.count(fd) == 1) // if fd is Server
				acceptClient(fd);
			else
			{
				char http_request[BUF_SIZE + 1];
				int valread;

				valread = recv(fd, http_request, BUF_SIZE, 0);
				valread = '\0';
				std::cout << valread << std::endl;

				// parser->feed(http_request);

				if (valread == BUF_SIZE)
				{
					std::cout << "client read incomplete \n";
					// parser->parse();
					return;
				}
				else if (valread <= 0) // If receive nothing from clients
				{  
					//Close the socket and erase it from fd of clients
					close(fd);
					int fdServ = findFdServer(fd); 
					_fdMap[fdServ].second.erase(fd);
					std::cout << "Closing " << fd << ". It belongs to " << fdServ << std::endl;
				}
				// else if ((resp = parser->finishParse()) == NULL)
				// {
				// 	std::cout << "THIS SHOULD NOT HAPPEN EVER, SOMETHING IS VERY WRONG\n";
				// }
				else // send response
				{  
					// resp->execute(this);
					// std::string finalResponsefake =
					// 	streamFunctionToString(&ResponseBuilder::print_response, resp);
					// std::cout
					// 	<< "finalResponsefake" << std::endl
					// 	<< finalResponsefake << std::endl;
					// send(fd, finalResponsefake.c_str(), finalResponsefake.size(), 0);

					std::string finalResponse = "HTTP/1.1 200 OK\nDate:Fri, 16 Mar 2020 17:21:12 GMT\nServer: my_server\nContent-Type: text/html;charset=UTF-8\nContent-Length: 1846\n\n<!DOCTYPE html>\n<html><h1>Hello world</h1></html>\n";
					std::cout << "A http response is sent\n" ;
					send(fd, finalResponse.c_str(), finalResponse.size(), 0);

					//Close the socket and erase it from client_fd of clients
					close (fd);
					int fdServ = findFdServer(fd); 
					_fdMap[fdServ].second.erase(fd);
					std::cout << "Closing " << fd << ". It belongs to " << fdServ << std::endl;
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
		return; // ! do something more than that
	}
	std::cout 	<< "New client on socket #" << clientFD 
				<< ". This socket belongs to Server at socket #" << fdServer
				<< std::endl;
	std::string parser = "I'm a GrammarParser";
	// GrammarParser parser = GrammarParser(*_base_request_parser);
	_fdMap[fdServer].second[clientFD] = parser;
}


int	MasterServer::findFdServer(int value)
{
	int fdServer = -1;

	// std::map<int, std::pair<OneServer*, std::map <int, GrammarParser> > >::iterator itFdMap;
	std::map<int, std::pair<OneServer*, std::map <int, std::string> > >::iterator itFdMap;

	for (itFdMap = _fdMap.begin(); itFdMap != _fdMap.end(); ++itFdMap)
	{
		// std::map<int, GrammarParser> clientMap;
		std::map<int, std::string> clientMap;
		clientMap = itFdMap->second.second;

		// std::map<int, GrammarParser>::iterator it_clientMap;
		std::map<int, std::string>::iterator it_clientMap;
		it_clientMap = clientMap.find(value);
		if (it_clientMap != clientMap.end())
		{
			fdServer = itFdMap->first;
			break; // to stop searching
		}
   }
   return fdServer;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */ 
