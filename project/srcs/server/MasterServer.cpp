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
	// Clear the scoket set
    FD_ZERO(&_readfds);
    
    int max_fd = 0;
    unsigned int ms_size = _configAllServer.size();

    for(unsigned int i = 0; i < ms_size; i++)
	{
		if(_configAllServer[i]->build() == 0) // bind fd and address, listen to fd
        {
            // add master socket to set FD_SET
            std::cout << "One server is built\n";
            std::cout << "Waiting for connections...\n";
        }
        else // ! Error while building OneServer
        {
            return (-1);
        }
	}
    if (max_fd == 0) // ! Error to build MasterServer
    {
        return (-1);
    }
    return (0);
}




/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */ 