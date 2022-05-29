#include "CgiHandler.hpp"
/**********************************************
 * Need to do 'fork'
 * Use 'execve' to run the script .php
 * Get the result
 * ********************************************/ 
 
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
CgiHandler::CgiHandler()
{
    _initEnv(); // ! Need to add REQUEST and CONFIG file here
}

/*
** -------------------------------PRIVATE METHODS --------------------------
*/

/*******************************************************
 * Initiate all needed environmental variables for php
*******************************************************/
void CgiHandler::_initEnv()
{
    // ! Todo: add more stuffs from parsing config file and http request
    
    this->_envVars["CONTENT_TYPE"] = ""; // type of file upload, from REQUEST
    this->_envVars["CONTENT_LENGTH"] = ""; // from REQUEST, POST
    this->_envVars["HTTP_COOKIE"] = ""; // ??
    this->_envVars["HTTP_USER_AGENT"] = ""; // name of web browser, from REQUEST
    this->_envVars["PATH_INFO"] = ""; // path of cgi script, from CONFIG file
    this->_envVars["QUERY_STRING"] = ""; // GET?
    this->_envVars["REMOTE_ADDR"] = ""; // IP of clinet
    this->_envVars["REMOTE_HOST"] = ""; // 
    this->_envVars["REQUEST_METHOD"] = ""; // GET or POST, from REQUEST
    this->_envVars["SCRIPT_FILENAME"] = ""; //full path of cgi script, from REQUEST and CONFIG file
    this->_envVars["SCRIPT_NAME"] = ""; // name of cgi scripts, from REQUEST
    this->_envVars["SERVER_SOFTWARE"] = ""; // name and version of running server.
} // ! are there more envVar?


char**  CgiHandler::_mapToArray() const
{
    char **return_array = new char*[_envVars.size() + 1];
    int k = 0;
    std::map<std::string, std::string>::const_iterator i;
    for (i = _envVars.begin(); i != _envVars.end(); i++)
    {
        std::string item = i->first + "=" + i->second;

        return_array[k] = new char[item.length() + 1];
        std::strcpy(return_array[k], item.c_str());
        k++;
    }
    return_array[k] = NULL;
    return return_array;
}

std::string CgiHandler::executeCgi(std::string & fileName)
{
    pid_t       pid;
    char        **env_params;

    env_params = this->_mapToArray();

    pid = fork();

    if (pid == -1) // return Error
    {
        return ("Code/Status for crashing");
    }
    else
    {
        if (pid != 0) // child process
        {
            // excecve
            // write to file

        }
        else // main projcess
        {
            // waitpid (wait for child process)
            // read the file above into a string

        }
    }
    //close every Fd, file
    // return (the string we got above)
}