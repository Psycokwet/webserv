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
    int         fdIn = dup(STDIN_FILENO);
    int         fdOut = dup(STDOUT_FILENO);
    int         fd[2];
    int         status;
    pid_t       pid;
    char        buffer[CGI_BUFFER_SIZE + 1];
    std::string new_body;
    char        **env_params;

    env_params = this->_mapToArray();

    if (pipe(fd) == 1)
        return (ERROR_500);
    pid = fork();
    if (pid == -1) // return Error
    {
        return (ERROR_500);
        // return ("ERROR 500 - Server crash"); // Will add this into Response
    }
    else if (pid == 0)
    {
        if (dup2(fd[0], STDOUT_FILENO) == -1)
            return (ERROR_500);
        if (dup2(fd[1], STDIN_FILENO) == -1)
            return (ERROR_500);
        close (fd[0]);
        if (execve(fileName.c_str(), NULL, env_params) == -1)
            return (ERROR_500);
    }
    else // main projcess
    {
        waitpid(pid, &status, 0);
        int ret = 1;
		while (ret > 0)
		{
			std::memset(buffer, 0, CGI_BUFFER_SIZE);
			ret = read(fd[0], buffer, CGI_BUFFER_SIZE);
			new_body += buffer;
		}
	}
    close (fd[1]);
    // free memory
    for (size_t i = 0; env_params[i]; i++)
		delete[] env_params[i];
	delete[] env_params;
    
    return new_body;
}