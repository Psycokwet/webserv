#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

#include <map>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>

class CgiHandler
{
    public:
        std::string executeCgi(std::string & fileName);
        

    private:
        std::map<std::string, std::string>      _envVars;
        
        CgiHandler();
        void        _initEnv();
        char**      _mapToArray() const;

};



#endif /************************** CgiHandler.hpp ***********/