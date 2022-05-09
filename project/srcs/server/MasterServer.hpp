#ifndef MASTERSERVER_HPP
#define MASTERSERVER_HPP

#include "../config/Node.hpp"
#include "../../includes/webserv.h"
#include "OneServer.hpp"
#include "AServerItem.hpp"
#include <sys/select.h> // FD_CLR, FD_ZERO, FD_SET, FD_ISSET macros

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
        int run();

    private:
        std::vector< OneServer* >      _configAllServer;
        fd_set                         _fdSet;
        int                            _max_fd;
        std::vector < int >            _ready;
         
		OneServer *createServer();

};


#endif /*...................MasterServer...............*/