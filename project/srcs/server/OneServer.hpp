#ifndef ONESERVER_HPP
#define ONESERVER_HPP

#include <string>
#include <vector>
#include <map>
# include <typeinfo> //typeid
#include "../config/Node.hpp"
// #include "ErrorPage.hpp"
// #include "Listen.hpp"
#include "AServerItem.hpp"


class OneLocation;

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
                return "ERROR: Directive is multiple declared.";
              }
      };
      class IncompleteLocation : public std::exception
      {
          public:
              virtual const char *what() const throw()
              {
                return "ERROR: Location doesn't have an argument, or too much args";
              }
      };

      class DuplicateUriError : public std::exception
      {
          public:
              virtual const char *what() const throw()
              {
                return "ERROR: Uri is duplicated.";
              }
      };
    private:
      AServerItem * addListen(Node *node);
      AServerItem * addServerName(Node *node);
      AServerItem * addLocation(Node *node);

      std::map< std::string, OneLocation* >             _location; // !
      std::vector< std::string >                       _server_name;

    protected:
      static DIRECTIVES_MAP _directives_to_setter;
      static DIRECTIVES_MAP initializeDirectivesMap();

      AServerItem * addIndex(Node *node);

      std::vector< std::string>                        _index;
      // Listen                                           _listen; // !
      // std::string                                      _root;
      // bool                                             _autoindex;
      // ErrorPage                                        _error_page;
      // std::vector< std::string>                        _method;
      // int                                              _client_max_body_size;
};

#endif /*------------------- OneServer --------*/