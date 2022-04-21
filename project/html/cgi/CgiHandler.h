#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP


#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

#include <map>

class CgiHandler 
{
 public:

  static const char *AUTH_TYPE;
  static const char *CONTENT_LENGTH;
  static const char *CONTENT_TYPE;
  static const char *GATEWAY_INTERFACE;
  static const char *PATH_INFO;
  static const char *PATH_TRANSLATED;
  static const char *QUERY_STRING;
  static const char *REDIRECT_STATUS;
  static const char *REMOTEaddr;
  static const char *REMOTE_IDENT;
  static const char *REMOTE_USER;
  static const char *REQUEST_URI;
  static const char *REQUEST_METHOD;
  static const char *SCRIPT_NAME;
  static const char *SCRIPT_FILENAME;
  static const char *SERVER_NAME;
  static const char *SERVER_PORT;
  static const char *SERVER_PROTOCOL;
  static const char *SERVER_SOFTWARE;

  static const int STDIN;
  static const int STDOUT;

  static const int BUFFER_SIZE;

CgiHandler(char c) 
{
    body = "";
    std::string literalPort = "8080";
    env[AUTH_TYPE] = "";
    env[CONTENT_LENGTH] = "0";
    env[CONTENT_TYPE] = "text/html";
    env[GATEWAY_INTERFACE] = "CGI/1.1";
    env[PATH_INFO] = "/Users/ilya/Desktop/a.out?var1=val1&var2=val2";
    env[PATH_TRANSLATED] = "/Users/ilya/Desktop/a.out";
    env[QUERY_STRING] = "";
    env[REDIRECT_STATUS] = "200";
    env[REMOTEaddr] = "8080";
    env[REMOTE_IDENT] = "";
    env[REMOTE_USER] = "";
    env[REQUEST_URI] = "/Users/ilya/Desktop/a.out";
    env[REQUEST_METHOD] = "GET";
    env[SCRIPT_NAME] = "/Users/ilya/Desktop/a.out";
    env[SCRIPT_FILENAME] = "/Users/ilya/Desktop/a.out";
    env[SERVER_NAME] = "http://localhost";
    env[SERVER_PORT] = "8080";
    env[SERVER_PROTOCOL] = "HTTP/1.1";
    env[SERVER_SOFTWARE] = "WebServ/42.0";
    std::string path = "/Users/ilya/Desktop/a.out?var1=val1&var2=val2";
    std::cerr << "pathQS: " << path << std::endl;
    std::string queryString = extractQueryString(path);
    std::cerr << "path: " << path << std::endl;
    std::cerr << "QS: " << queryString << std::endl;
    std::string extension = findExtension(path); //NEVER USED
    std::cerr << "extension: " << extension << std::endl;
  }
  


static std::string _toLiteral(int num) 
{
  std::stringstream ss;
  ss << num;
  return ss.str();
}

static std::string findExtension(const std::string &path) 
{
  size_t delimiter = path.find_last_of(".");
  return path.substr(delimiter);
}

#endif