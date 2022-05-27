#ifndef ONESERVER_HPP
#define ONESERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <utility> // std::pair, std::make_pair
#include "../config/Node.hpp"
#include "ErrorPage.hpp"
#include "Listen.hpp"
#include "AServerItem.hpp"

// # define NAME(x) _##x;
class OneServer :public AServerItem
{

	#define DIRECTIVES_MAP std::map<std::string, AServerItem *(OneServer::*)(Node*)>
    public:
        OneServer();
        // OneServer(const OneServer & src);
        // OneServer & operator=(const OneServer & rhs);
        ~OneServer();
		
		AServerItem *consume(Node *node);
		virtual std::ostream & print( std::ostream & o) const;

    private:
		static DIRECTIVES_MAP _directives_to_setter;
		static DIRECTIVES_MAP initializeDirectivesMap();

		AServerItem * addListen(Node *node);

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