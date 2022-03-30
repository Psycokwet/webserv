#ifndef ALLCONFIG_HPP
#define ALLCONFIG_HPP

#include <vector>
#include <list>
#include "Node.hpp"
#include "ConfigOneServer.hpp"

class AllConfig
{
    typedef std::map <std::string, std::vector <std::string> > t_map_server;
    public:
        // AllConfig();
        // AllConfig(const AllConfig & src);
        // AllConfig & operator=(const AllConfig & rhs);
        // ~AllConfig();

        void    parse(std::string config_path);
        // void    parse_directive_of_one_server(Node* directive_list, ConfigOneServer one_server, std::string name);
        void    parse_directive_of_one_server(Node* directive_list, t_map_server * one_server, std::string name);



    private:
        // std::vector<ConfigOneServer> _config_all_servers;
        std::vector< t_map_server > _config_all_servers; 

};

std::ostream &			operator<<( std::ostream & o, std::vector <std::string> const & i );


#endif /*-------------------- AllConfig ----------------------- */