#include "CgiHandler.hpp"
/**********************************************
 * Need to do 'fork'
 * Use 'execve' to run the script .php
 * Get the result, put it into a http response
 * Use 'pipe' to read output to a buffer
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
    

}
