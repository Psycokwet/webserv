#ifndef SERVER_HPP
#define SERVER_HPP

#include <stdio.h> 
#include <string.h>   //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>   //close 
#include <arpa/inet.h>    //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 

#include <map>
#include <set>
#include <vector>
#include <iostream>

#define TRUE 1
#define SERVER_ERR(err)	do { std::cerr << "ERROR while perfoming " << err << std::endl; exit(1); } while (0)
#define MAX_CLIENTS 25 
#define MAX_CLIENTS_QUEUE 3 
#define MAX(x, y) ((x > y) ? x : y) 

class	Server
{

private:
	std::map< int, std::set<int> >		_fdMap;		// key as fdServer, value as fdClients
	int									_maxFD;		// Current highest client FD
	std::set< int > 					_fdServer;	// _fdServer set
	fd_set								_fdReader;	// Structure to select client FD for reading
	int									_PORT[3] = {8080, 8081, 8082};
	int									_r;
	// Accept new client connection
	void	acceptClient(int fdServer);
	int 	findFdServer(int value);

	// // Remove existing client
	// void	removeClient(int fd);

	// Make all open socket ready to be read then select them. Return the number of FDs
	// ready to be read
	int		setFDForReading();

	// RQueue, std::set<int> &disconnectLisead from fd to get client commands then forward it to the IRC program
	void	recvProcessCommand(int totalFD);

public:
	Server();
	~Server();

	// Set up server properly for listening and accepting clients
	void	SetUp();
	
	// Kick off server's infinite loop (until SIGINT, SIGQUIT or SIGKILL received)
	void	Run();

};

#endif
