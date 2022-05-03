#ifndef ALOCATION_HPP
#define ALOCATION_HPP

#include <string>
#include <vector>
#include <map>
#include <set>
#include "../config/Node.hpp"

// #include "ErrorPage.hpp"
// #include "Listen.hpp"
#include "AServerItem.hpp"
class ALocation :public AServerItem
{
	#define DIRECTIVES_TYPE AServerItem *(ALocation::*)(Node*)
    #define DIRECTIVES_MAP std::map<std::string, DIRECTIVES_TYPE>
    
    public:
      ALocation();
      // ALocation(const ALocation & src);
      // ALocation & operator=(const ALocation & rhs);
      ~ALocation();

      virtual AServerItem *consume(Node *node);
      virtual std::ostream & print( std::ostream & o) const;

      virtual AServerItem * addServerName(Node *node);
      virtual AServerItem * addLocation(Node *node);
      // virtual AServerItem * addListen(Node *node);

      virtual AServerItem * addIndex(Node *node);
      virtual AServerItem * addRoot(Node *node);
      virtual AServerItem * addAutoIndex(Node *node);
      virtual AServerItem * addMethod(Node *node);
      virtual AServerItem * addMaxSize(Node *node);

    protected:
      virtual DIRECTIVES_MAP & getDirectiveMap();
      std::vector< std::string >  _index;
      std::string                 _root;
      bool                        _autoindex;
      std::set< std::string>      _method;
      int                         _client_max_body_size;
      // ErrorPage             _error_page;
      // cgi
};

#endif /*------------------- ALocation --------*/