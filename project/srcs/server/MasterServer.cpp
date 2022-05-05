#include "MasterServer.hpp"

/*
** ---------------------------------- STATIC ----------------------------------
*/

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
** --------------------------------- METHODS ----------------------------------
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
	for(unsigned int i = 0; i < _configAllServer.size(); i++)
	{
		buildOneServer(_configAllServer[i]);
	}

}

int MasterServer::buildOneServer(OneServer * one_server)
{
	int i;
    int max_clients = 3;
    int client_socket[max_clients];
	int server_fd;

	for (i = 0; i < max_clients; i++)
        client_socket[i] = 0;

    // server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // if (server_fd == 0)
    // {
    //     perror("Fail to set socket\n");
    //     exit(EXIT_FAILURE);
    // }
	//     // SO_REUSEPORT    enables duplicate server_address and port bindings
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
    // {
    //     perror("Fail to setsockopt");
    //     exit(EXIT_FAILURE);
    // }
    // // type of socket created:
    // server_address.sin_family = AF_INET;
    // server_address.sin_addr.s_addr = htonl(INADDR_ANY); // ! need to change this to our server_name
    // server_address.sin_port = htons(PORT);

    // // bind the socket to localhost port 8080
    // if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    // {
    //     perror("Fail to bind");
    //     exit(EXIT_FAILURE);
    // }
    // printf("Listener on port number %d\n", PORT);

    // // Try to specify maximun of 3 pending connection for the master socket (server_fd)
    // if (listen(server_fd, 3) < 0)
    // {
    //     perror("Fail to listen");
    //     exit(EXIT_FAILURE);
    // }

    // //accept the incoming connection
    // addrlen = sizeof(server_address);
    // puts("Waiting for connections...");


}




/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */