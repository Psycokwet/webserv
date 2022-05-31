#include "CgiHandler.hpp"
/**********************************************
 * Need to do 'fork'
 * Use 'execve' to run the script .php
 * Get the result
 * ********************************************/ 
 
/*
** ------------------------------- STATIC --------------------------------
*/

char **stringToArray(std::vector<std::string> argVector)
{
    char **return_array = new char*[argVector.size() + 1];
    unsigned long i;
    for (i = 0; i < argVector.size(); i++)
    {
        return_array[i] = new char[argVector[i].length() + 1];
        std::strcpy(return_array[i], argVector[i].c_str());
    }
    return_array[i] = NULL;
    return return_array;
}
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
    int         status;
    int         pipe_ret;
    char        buffer[CGI_BUFFER_SIZE + 1];
    std::string new_body;
    char        **envVec;
    char        **argVec;

    std::vector<std::string> stringVec;
    stringVec.push_back("php");
    stringVec.push_back(fileName);

    envVec = this->_mapToArray();
    argVec = stringToArray(stringVec);

    pid = fork();
    if (pid == -1)
        return ERROR_500;
    if (pid == 0) // child
    {
        int fileFd = open("new.txt", O_WRONLY | O_CREAT, 0777);
        if (fileFd == -1)
            return ERROR_500;
        int fileFd2 = dup2(fileFd, STDOUT_FILENO);
        close (fileFd);
        if (execve(CMD_PHP_LINUX, argVec, envVec) == -1)
            return ERROR_500;
    }
    else // parent
    {
        waitpid(-1, &status, 0);
        int fileFd = open("new.txt", O_RDONLY);
        if (fileFd == -1)
            return ERROR_500;
        
        int ret = 1;
        while (ret > 0)
        {
            std::memset(buffer, 0, CGI_BUFFER_SIZE);
            ret = read(fileFd, buffer, CGI_BUFFER_SIZE - 1);
            new_body += buffer;
        }
        close (fileFd);
    }
    // free memory
    for (size_t i = 0; envVec[i]; i++)
		delete[] envVec[i];
	delete[] envVec;

    return new_body;
}