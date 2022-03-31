#ifndef CONFIGONESERVER_HPP
#define CONFIGONESERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <utility> // std::pair, std::make_pair


class ConfigOneServer
{
    public:
        // ConfigOneServer();
        // ConfigOneServer(const ConfigOneServer & src);
        // ConfigOneServer & operator=(const ConfigOneServer & rhs);
        // ~ConfigOneServer();

        // Todo: parse content of one server from Node. 
        void parseDirective(std::list<Node*> directive_list, std::string directive_name);

    private:
        std::vector< std::pair<int, std::string> >      _listen; // !
        std::string                                     _root;
        std::vector< std::string >                      _server_name;
        std::vector< std::string >                      _autoindex;
        std::map< std::string, ConfigOneServer >        _location; // !
        // Todo: add more directives related to the project: error_page, cgi, cig_pass, methods, client_max_body_size (what else?)
        // ! Some directives are not in the ref of NGINX because it follows the project!


};

#endif /*------------------- ConfigOneServer --------*/