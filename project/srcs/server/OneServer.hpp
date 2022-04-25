#ifndef ONESERVER_HPP
#define ONESERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <utility> // std::pair, std::make_pair
#include "../config/Node.hpp"
// #include "ErrorPage.hpp"
// #include "Listen.hpp"
#include "AServerItem.hpp"

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

      class MultipleDeclareError : public std::exception
      {
          public:
              virtual const char *what() const throw()
              {
                return "Directive is not allowed to multiple declared.";
              }
      };
    private:
      static DIRECTIVES_MAP _directives_to_setter;
      static DIRECTIVES_MAP initializeDirectivesMap();

      AServerItem * addListen(Node *node);
      AServerItem * addServerName(Node *node);
      AServerItem * addLocation(Node *node);
      AServerItem * addIndex(Node *node);

      std::map< std::string, OneServer >               _location; // !
      std::vector< std::string >                       _server_name;
      std::vector< std::string>                        _index;
      // Listen                                           _listen; // !
      // std::string                                      _root;
      // bool                                             _autoindex;
      // ErrorPage                                        _error_page;
      // std::vector< std::string>                        _method;
      // int                                              _client_max_body_size;
};

#endif /*------------------- OneServer --------*/