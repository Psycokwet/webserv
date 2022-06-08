#include "Server.hpp"

Server::Server() :
	_maxFD(-1)
{}


Server::~Server()
{}

void	Server::SetUp()
{
	// Create server socket
	int opt = TRUE;

	for (int i = 0; i < sizeof(_PORT)/sizeof(int); i++)
	{
		int fdServ;
		struct sockaddr_in address;
		fdServ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
		if (fdServ == -1)
			SERVER_ERR("Socket");
		if (setsockopt(fdServ, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&opt, sizeof(opt)) == -1)
			SERVER_ERR("Setsockopt");

		// Bind socket to port
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(_PORT[i]);
		if (bind(fdServ, (sockaddr *)&address, sizeof(address)) == -1)
			SERVER_ERR("Bind");

		// Listen for connections
		if (listen(fdServ, MAX_CLIENTS_QUEUE) == -1)
			SERVER_ERR("listen");

		std::cout	<< "Listening on port "
					<< _PORT[i]
					<< std::endl;

		
		std::pair<std::set<int>::iterator,bool> ret;
		ret = _fdServer.insert(fdServ);               // no new element inserted
		if (ret.second==false)
			SERVER_ERR("Repeated Ports");

		_fdMap[fdServ]; 

		_maxFD = MAX(_maxFD, fdServ);
	}
	std::cout << "mymap now contains " << _fdMap.size() << " elements.\n";
}

void	Server::Run()
{
	int	totalFD;

	while (true)
	{
		FD_ZERO(&_fdReader);
		totalFD = setFDForReading();
		recvProcessCommand(totalFD);
	}
}

int	Server::setFDForReading()
{
	int new_r;
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
	new_r = select(_maxFD + 1, &_fdReader, NULL, NULL, NULL);
	if (new_r == -1)
		SERVER_ERR("select");
	_r = MAX(_r, new_r);
	return _r;
}

int	Server::findFdServer(int value)
{
	int fdServer;

	std::map<int, std::set<int>>::iterator it;
	
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

void	Server::recvProcessCommand(int totalFD)
{
	// Checking each socket for reading, starting from FD 3 because there should be nothing
	// to read from 0 (stdin), 1 (stdout) and 2 (stderr)
	for (int fd = 3; fd <= _maxFD && totalFD; ++fd)
		if (FD_ISSET(fd, &_fdReader))
		{
			if (_fdServer.count(fd) == 1)
				acceptClient(fd);
			else
			{
				char http_response[] = "HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 88\nContent-Type: text/html\nConnection: Closed\n\n\n<html>\n<body>\n<h1>Hello, World!</h1>\n</body>\n</html>\n";  

				char http_request[100000];
				int valread;
				// int addrlen = sizeof(address);
				if ((valread = recv( fd , http_request, 100000, 0)) == 0) // ! Neu khong nhan duoc gi nua o client  
                {  
                    // // Somebody disconnected , get his details and print 
                    // getpeername(fd , (struct sockaddr*)&address, (socklen_t*)&addrlen);  
                    // printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
                    
                    //Close the socket and mark as 0 in list for reuse 
                    close(fd);
					int fdServ = findFdServer(fd);  
                    _fdMap[fdServ].erase(fd);
                }  
                     
                //Echo back the http_response that came in 
                else 
                {  
                    //set the string terminating NULL byte on the end 
                    //of the data read 
                    http_request[valread] = '\0';  
                    printf("Request Received from client:\n--------------\n%s\n", http_request);
                    send(fd , http_response , strlen(http_response) , 0 );
					std::cout << "A http response is sent\n" ;
                    close (fd);
					int fdServ = findFdServer(fd);  
                    _fdMap[fdServ].erase(fd);
                }  
			}
			--totalFD;
		}
}

void	Server::acceptClient(int fdServer)
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