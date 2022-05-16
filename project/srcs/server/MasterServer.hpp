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
        void run();

    private:
        std::vector< OneServer* >      _configAllServer;
        std::vector< t_fd >            _fdSet;
        int                            _max;
        int                            _r;
        int                            _maxFd;
        fd_set                         _fdRead;
        fd_set                         _fdWrite;
         
		OneServer   *createServer();
        void        init_env();
        void        get_server_ready();
        void        init_fd();
        void        do_select();
        void        check_fd();
        
        void        server_accept(int s);
        void        client_read(int fd);
        void        client_write(int fd);

};


#endif /*...................MasterServer...............*/