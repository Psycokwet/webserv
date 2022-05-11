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
    _max_fd = 0;
    unsigned int ms_size = _configAllServer.size();

    /*************************************************************/
    /* Initialize the master fd_set                              */
    /*************************************************************/
    FD_ZERO(&_fdSet);

    for(unsigned int i = 0; i < ms_size; i++)
	{
        int fd;
		if(_configAllServer[i]->build() == 0) // bind fd and address, listen to fd
        {
            std::cout << "\nWaiting for connections...\n";
            
            // add socket to set _fdSet
            fd = _configAllServer[i]->getFD();
            FD_SET(fd, &_fdSet);
            
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
    /*************************************************************/
    /* Loop waiting for incoming connects or for incoming data   */
    /* on any of the connected sockets.                          */
    /*************************************************************/
    while (1)
    {
        fd_set          read_fd_set;
        fd_set          write_fd_set;
        struct timeval  timeout;

        int activity = 0;
        while (activity == 0)
        {
            /*************************************************************/
            /* Initialize the timeval struct to 3 minutes.  If no        */
            /* activity after 3 minutes this program will end.           */
            /*************************************************************/
            timeout.tv_sec  = 3 * 60;
            timeout.tv_usec = 0;
            
            
            FD_ZERO(&write_fd_set);
            for (std::vector<int>::iterator it = _ready.begin() ; it != _ready.end() ; it++)
				FD_SET(*it, &write_fd_set);

            /*************************************************************/
            /* Call select() and wait 3 minutes for it to complete.      */
            /* Wait for one or more fd become "ready" to read and write  */
            /*************************************************************/
            activity = select(_max_fd + 1, &read_fd_set, &write_fd_set, NULL, &timeout);

            /**********************************************************/
            /* Check to see if the select call failed.                */
            /**********************************************************/
            if (activity < 0)
            {
                std::cout << "select() failed" << std::endl;
                break;
            }
        }
        /**********************************************************/
        /* Check to see if the 3 minute time out expired.         */
        /**********************************************************/
        if (activity == 0)
        {
            std::cerr << "select() time out. End program." << std::endl;
            break ;
            // Todo: next: close / clear opened socket

            FD_ZERO(&_fdSet);
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



        // }


    }
    return 0;
}




/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */ 