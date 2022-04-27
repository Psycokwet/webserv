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
#include "ALocation.hpp"
#include "OneLocation.hpp"
class OneLocation ;
class OneServer :public ALocation
{
    public:
      OneServer();
      // OneServer(const OneServer & src);
      // OneServer & operator=(const OneServer & rhs);
      ~OneServer();

      AServerItem *consume(Node *node);
      virtual std::ostream & print( std::ostream & o) const;
      static DIRECTIVES_MAP initializeDirectivesMap();

    private:
      virtual AServerItem * addServerName(Node *node);
      virtual AServerItem * addLocation(Node *node);

      std::map< std::string, OneLocation* >             _location; // !
      std::vector< std::string >                       _server_name;

      // Listen                                           _listen; // !
      // std::string                                      _root;
      // bool                                             _autoindex;
      // ErrorPage                                        _error_page;
      // std::vector< std::string>                        _method;
      // int                                              _client_max_body_size;
    protected:
      virtual AServerItem * addIndex(Node *node);
      static DIRECTIVES_MAP _directives_to_setter;
      virtual DIRECTIVES_MAP & getDirectiveMap();
};

#endif /*------------------- OneServer --------*/