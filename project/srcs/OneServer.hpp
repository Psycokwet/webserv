#ifndef ONESERVER_HPP
#define ONESERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <utility> // std::pair, std::make_pair
#include "Node.hpp"
#include "ErrorPage.hpp"
#include "Listen.hpp"

// # define NAME(x) _##x;

class OneServer
{
    public:
        OneServer();
        // OneServer(const OneServer & src);
        // OneServer & operator=(const OneServer & rhs);
        // ~OneServer();

    private:
        // Listen                                          _listen; // !
        std::vector< std::string >                      _server_name;
        // std::map< std::string, OneServer >        _location; // !
        // std::string                                     _root;
        // bool                                            _autoindex;
        // std::vector< std::string>                       _index;
        // ErrorPage                                       _error_page;
        // std::vector< std::string>                       _method;
        // int                                             _client_max_body_size;


        // Todo: add more directives related to the project: error_page, cgi, cig_pass, methods, client_max_body_size (what else?)
        // ! Some directives are not in the list of NGINX because we need to follow the project!


};

#endif /*------------------- OneServer --------*/