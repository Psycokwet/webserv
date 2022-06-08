#ifndef MASTERSERVER_HPP
#define MASTERSERVER_HPP

#include "../config/Node.hpp"
#include "../requests/GrammarParser.hpp"
#include "../../includes/webserv.h"
#include "OneServer.hpp"
#include "AServerItem.hpp"
#include <sys/select.h> // FD_CLR, FD_ZERO, FD_SET, FD_ISSET macros

class MasterServer;

typedef struct 	s_fd {
    int     type;
    int     host;
    void(MasterServer::*fct_read) (int);
    // void(MasterServer::*fct_write) (int);
    char    buf_read[BUF_SIZE + 1];
    // char    buf_write[BUF_SIZE + 1];
    GrammarParser*   parser;
} 	t_fd;

class MasterServer :public AServerItem
{
    public:
        MasterServer();
        MasterServer(const MasterServer & src);
        MasterServer & operator=(const MasterServer & rhs);
        virtual ~MasterServer();
      	virtual AServerItem *consume(Node *node);

		std::ostream & print( std::ostream & o) const;
        int build();
        void run();


        class BuildError : public std::exception
        {
            public:
                virtual const char *what() const throw()
                {
                    return "ERROR: Couldn't finish the build";
                }
        };

        class RepeatedPort : public std::exception
        {
            public:
                virtual const char *what() const throw()
                {
                return "ERROR: Different servers listen to same port";
                }
        };

    private:
        std::vector< OneServer* >           _configAllServer;
        std::map< int, std::set<int> >		_fdMap;		// key as fdServer, value as fdClients
        int									_maxFD;		// Current highest client FD
        std::set< int > 					_fdServer;	// _fdServer set
        fd_set								_fdReader;	// Structure to select client FD for reading
        int									_numberOfReadyFd;

       
    	GrammarParser*				   _base_request_parser;
         
		OneServer   *createServer();
        void	acceptClient(int fdServer);
        int 	findFdServer(int value);
        // Make all open socket ready to be read then select them. Return the number of FDs ready to be read
        int		setFDForReading();

        // RQueue, std::set<int> &disconnectLisead from fd to get client commands then forward it to the IRC program
        void	recvProcessCommand(int totalFD);
       

};


#endif /*...................MasterServer...............*/