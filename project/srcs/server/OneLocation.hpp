#ifndef ONELOCATION_HPP
#define ONELOCATION_HPP

#include <string>
#include <vector>
#include <map>
#include <utility> // std::pair, std::make_pair
#include "../config/Node.hpp"
// #include "ErrorPage.hpp"
// #include "Listen.hpp"

#include "ALocation.hpp"
class OneLocation :public ALocation
{
    public:
        OneLocation();
        ~OneLocation();
    	virtual std::ostream & print( std::ostream & o) const;
		virtual AServerItem *addIndex(Node *node);
      	static DIRECTIVES_MAP initializeDirectivesMap();
	protected:
     	static DIRECTIVES_MAP _directives_to_setter;
      	virtual DIRECTIVES_MAP & getDirectiveMap();
};

#endif /*------------------- OneLocation --------*/