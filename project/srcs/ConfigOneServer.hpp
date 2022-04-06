#ifndef CONFIGONESERVER_HPP
#define CONFIGONESERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <utility> // std::pair, std::make_pair
#include "Node.hpp"

// # define NAME(x) _##x;

class ConfigOneServer
{
    public:
        typedef std::map<std::string, std::vector< std::string > >  t_inner_map;
        ConfigOneServer();
        // ConfigOneServer(const ConfigOneServer & src);
        // ConfigOneServer & operator=(const ConfigOneServer & rhs);
        // ~ConfigOneServer();

        // Todo: parse content of one server from Node. 
        void parseDirective(Node::t_node_list::const_iterator directive_list, std::string directive_name);
        void parseInnerArgs(std::vector<std::string> inner_args, std::string directive_name);

        t_inner_map    getInnerMap() const;
    private:
        t_inner_map      _inner_map_one_server; // !
        // std::vector< std::pair<std::string, int> >      _listen; // !
        // std::string                                     _root;
        // std::vector< std::string >                      _server_name;
        // std::vector< std::string >                      _autoindex;
        // std::map< std::string, ConfigOneServer >        _location; // !
        // Todo: add more directives related to the project: error_page, cgi, cig_pass, methods, client_max_body_size (what else?)
        // ! Some directives are not in the ref of NGINX because it follows the project!


};

#endif /*------------------- ConfigOneServer --------*/