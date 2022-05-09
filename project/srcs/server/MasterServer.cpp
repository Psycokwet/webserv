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
    while (1)
    {
        fd_set  read_fd_set;
        fd_set  write_fd_set;

        int     activity = 0;

        while (activity == 0)
        {
            FD_ZERO(&write_fd_set);
            for (std::vector<int>::iterator it = _ready.begin() ; it != _ready.end() ; it++)
				FD_SET(*it, &write_fd_set);


            //wait for an activity on one of the sockets, timeout is NULL so wait permernently
            activity = select(_max_fd + 1, &read_fd_set, &write_fd_set, NULL, NULL);
            std::cout << "activity = " << activity;

        }

        // Todo here

        // if (activity > 0)
        // {
        //     for (std::vector<int>::const_iterator it = _ready.begin(); it != _ready.end(); it++ )
        //     {
        //         if (FD_SET(*it, &write_fd_set))
        //         {
        //             // ! send stuff
        //             // int one_act = send(stuff)
        //             // int one_act = server.send(it)
        //             if (one_act == 0)
        //                 _ready.erase(it);
        //             else if(one_act == -1)
        //             {
        //                 FD_CLR(*it, &_fdSet);
        //                 FD_CLR(*it, &read_fd_set);
        //                 _ready.erase(it);
        //             }
        //             one_act = 0;
        //             break
        //         }
        //     }
        //     if (one_act)
        //         std::cout << "...Received a connection ! ..." << std::endl;
        //     for ()
                
        // }

        // if ((activity < 0) && (errno != EINTR))
        // {
        //     std::cerr << "Error: Select does not work" << std::endl;
            
        //     // Todo: next: close / clear opened socket

        //     FD_ZERO(&_fdSet);
        // }
        // else
        // {
        //     for (std::vector<int>::const_iterator it = _ready.begin(); it != _ready.end(); it++ )
        //     {
        //         if (FD_ISSET(*it, &write_fd_set))
        //         {
        //             int ret = 
        //         }
        //     }


        // }


    }
    return 0;
}




/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */ 