#ifndef ONELOCATION_HPP
#define ONELOCATION_HPP

#include <string>
#include <map>
#include <vector>
#include "AServerItem.hpp"
#include "../config/Node.hpp"

class OneLocation: public AServerItem
{
    #define LOCATION_DIRECTIVES_MAP std::map<std::string, AServerItem *(OneLocation::*)(Node*)>

    public:
        OneLocation();
        ~OneLocation();

        AServerItem *consume(Node *node);

        virtual std::ostream & print(std::ostream & o) const;

    private:
        static LOCATION_DIRECTIVES_MAP _directives_to_setter;
        static LOCATION_DIRECTIVES_MAP initializeDirectivesMap();

        AServerItem * addIndex(Node *node);

        // std::string                                      _root;
        std::vector< std::string>                        _index;
        // bool                                             _autoindex;
        // ErrorPage                                        _error_page;
        // std::vector< std::string>                        _method;
        // int                                              _client_max_body_size;

        

};

#endif /* ***************************************************** ONELOCATION_HPP */