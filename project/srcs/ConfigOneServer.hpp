#ifndef CONFIGONESERVER_HPP
#define CONFIGONESERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <utility> // std::pair, std::make_pair
#include "Node.hpp"
#include "ErrorPage.hpp"
#include "Listen.hpp"

// # define NAME(x) _##x;

class ConfigOneServer
{
    public:
        // typedef std::map<std::string, std::vector< std::string > >  t_inner_map;
        ConfigOneServer();
        // ConfigOneServer(const ConfigOneServer & src);
        // ConfigOneServer & operator=(const ConfigOneServer & rhs);
        // ~ConfigOneServer();

        // Todo: parse content of one server from Node. 
        void parseDirective(Node::t_node_list::const_iterator directive_list, std::string directive_name);
        void parseInnerArgs(std::vector<std::string> inner_args, std::string directive_name);

        // t_inner_map    getInnerMap() const;
    private:
        // t_inner_map      _inner_map_one_server; // !
        Listen                                          _listen; // !
        std::vector< std::string >                      _server_name;
        std::map< std::string, ConfigOneServer >        _location; // !
        std::string                                     _root;
        bool                                            _autoindex;
        std::vector< std::string>                       _index;
        ErrorPage                                       _error_page;
        std::vector< std::string>                       _method;
        int                                             _client_max_body_size;


        // Todo: add more directives related to the project: error_page, cgi, cig_pass, methods, client_max_body_size (what else?)
        // ! Some directives are not in the list of NGINX because we need to follow the project!


};

#endif /*------------------- ConfigOneServer --------*/