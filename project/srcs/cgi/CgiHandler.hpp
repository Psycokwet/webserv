#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

#include <map>

class CgiHandler
{
    public:

    private:
        std::map<std::string, std::string>      _envVars;

        
        CgiHandler();
        void        _initEnv();
        char**      _mapToArray() const;

}



#endif /************************** CgiHandler.hpp ***********/