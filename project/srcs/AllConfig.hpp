#ifndef ALLCONFIG_HPP
#define ALLCONFIG_HPP

#include <vector>
#include <list>
#include "Node.hpp"
#include "ConfigOneServer.hpp"

class AllConfig
{
    public:
        // AllConfig();
        // AllConfig(const AllConfig & src);
        // AllConfig & operator=(const AllConfig & rhs);
        // ~AllConfig();

        void    parse(std::string config_path);



    private:
        // std::vector<ConfigOneServer> _config_all_servers;
        ConfigOneServer         _config_all_servers; 

};

std::ostream &			operator<<( std::ostream & o, std::vector <std::string> const & i );


#endif /*-------------------- AllConfig ----------------------- */