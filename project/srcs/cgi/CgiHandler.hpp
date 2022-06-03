#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include "../util/toArray.hpp"             

#define CGI_BUFFER_SIZE 100
#define CMD_PHP_LINUX "/usr/bin/php"
#define ERROR_500 "Internal Server ERROR 500\n"

class CgiHandler
{
    public:
        CgiHandler();
        std::string executeCgi(std::string & fileName);
        

    private:
        std::map<std::string, std::string>      _envVars;
        
        void        _initEnv();
};



#endif /************************** CgiHandler.hpp ***********/