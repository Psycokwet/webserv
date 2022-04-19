#ifndef MASTERSERVER_HPP
#define MASTERSERVER_HPP

#include "Node.hpp"
#include "OneServer.hpp"
#include "AServerItem.hpp"
#include <sys/select.h> // FD_CLR, FD_ZERO, FD_SET, FD_ISSET macros

#define PORT

class MasterServer :public AServerItem
{
    public:
        MasterServer();
        MasterServer(const MasterServer & src);
        MasterServer & operator=(const MasterServer & rhs);
        virtual ~MasterServer();
		OneServer *createServer();

		std::ostream & print( std::ostream & o) const;
        // int setup();

    private:
        std::vector< OneServer* >      _configAllServer;

};


#endif /*...................MasterServer...............*/