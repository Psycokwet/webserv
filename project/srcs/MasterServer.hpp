#ifndef MASTERSERVER_HPP
#define MASTERSERVER_HPP

#include "Node.hpp"
#include "AllConfig.hpp"
#include <sys/select.h> // FD_CLR, FD_ZERO, FD_SET, FD_ISSET macros

#define PORT

class MasterServer 
{
    public:
        MasterServer();
        MasterServer(const MasterServer & src);
        MasterServer & operator=(const MasterServer & rhs);
        ~MasterServer();

        void parseConfig(std::string config_path);
        // int setup();

    private:
        AllConfig      _allConfig;

};


#endif /*...................MasterServer...............*/