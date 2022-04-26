#ifndef ONELOCATION_HPP
#define ONELOCATION_HPP

#include <string>
#include <vector>
#include <map>
#include <utility> // std::pair, std::make_pair
#include "../config/Node.hpp"
// #include "ErrorPage.hpp"
// #include "Listen.hpp"
#include "OneServer.hpp"

class OneLocation :public OneServer
{
    public:
        OneLocation();
        ~OneLocation();

        
};

#endif /*------------------- OneLocation --------*/