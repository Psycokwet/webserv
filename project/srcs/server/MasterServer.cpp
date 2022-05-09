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
    FD_ZERO(&_fdSet);
    
    _max_fd = 0;
    unsigned int ms_size = _configAllServer.size();

    for(unsigned int i = 0; i < ms_size; i++)
	{
        int fd;
		if(_configAllServer[i]->build() == 0) // bind fd and address, listen to fd
        {
            std::cout << "\nWaiting for connections...\n";
            
            // add socket to set _fdSet
            fd = _configAllServer[i]->getFD();
            FD_SET(fd, &_fdSet);
            
            std::cout << "fd is: " << fd << std::endl;

            if (fd > _max_fd)
                _max_fd = fd;
        }
        else // ! Error while building OneServer
            return (-1);
	}
    if (_max_fd == 0) // ! Error to build MasterServer
    {
        return (-1);
    }
    return 0;
}

int MasterServer::run()
{
    std::cout << "MasterServer is running!!!" << std::endl;
    return 0;
}




/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */ 