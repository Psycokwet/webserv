#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

#include <map>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>             

#define CGI_BUFFER_SIZE 100
#define ERROR_500 "Internal Server ERROR 500\n"

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