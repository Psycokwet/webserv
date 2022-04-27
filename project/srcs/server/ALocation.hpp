#ifndef ALOCATION_HPP
#define ALOCATION_HPP

#include <string>
#include <vector>
#include <map>
# include <typeinfo> //typeid
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
      virtual AServerItem * addIndex(Node *node);
      AServerItem * addListen(Node *node);

    protected:
      virtual DIRECTIVES_MAP & getDirectiveMap();
      std::vector< std::string> _index;
};

#endif /*------------------- ALocation --------*/