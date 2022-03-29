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
        void    parse_directive_of_one_server(Node* directive_list, ConfigOneServer one_server, std::string name);



    private:
        std::vector<ConfigOneServer> _config_all_servers;

};


#endif /*-------------------- AllConfig ----------------------- */