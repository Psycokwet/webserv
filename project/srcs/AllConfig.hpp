#ifndef ALLCONFIG_HPP
#define ALLCONFIG_HPP

#include <vector>
#include "ConfigOneServer.hpp"

class AllConfig
{
    public:
        AllConfig();
        AllConfig(const AllConfig & src);
        AllConfig & operator=(const AllConfig & rhs);
        ~AllConfig();



    private:
        std::vector<ConfigOneServer> _config_all_server;

};


#endif /*-------------------- AllConfig ----------------------- */