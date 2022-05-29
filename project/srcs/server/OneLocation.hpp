#ifndef ONELOCATION_HPP
#define ONELOCATION_HPP

#include <string>
#include <vector>
#include <map>
#include <utility> // std::pair, std::make_pair
#include "../config/Node.hpp"
// #include "ErrorPage.hpp"
// #include "Listen.hpp"
#include "../util/numbers.hpp"
#include "ALocation.hpp"
class OneLocation :public ALocation
{
    public:
        OneLocation();
        ~OneLocation();
    	virtual std::ostream & print( std::ostream & o) const;
      	static DIRECTIVES_MAP initializeDirectivesMap();

		virtual AServerItem *addIndex(Node *node);
		virtual AServerItem *addRoot(Node *node);
		virtual AServerItem *addAutoIndex(Node *node);
		virtual AServerItem *addMethod(Node *node);
		virtual AServerItem *addMaxSize(Node *node);
		virtual AServerItem *addErrorPage(Node *node);
		virtual AServerItem * addCgi(Node *node);

	protected:
     	static DIRECTIVES_MAP _directives_to_setter;
      	virtual DIRECTIVES_MAP & getDirectiveMap();
};

#endif /*------------------- OneLocation --------*/