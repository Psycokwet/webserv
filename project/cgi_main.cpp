#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>         // open    

#define CGI_BUFFER_SIZE 100
#define ERROR_500 "Internal Server ERROR 500\n"
#define CMD_PHP_LINUX "/usr/bin/php"

std::map<std::string, std::string> initEnv()
{
    // ! Todo: add more stuffs from parsing config file and http request
    std::map<std::string, std::string> envVars;
    envVars["CONTENT_TYPE"] = ""; // type of file upload, from REQUEST
    envVars["CONTENT_LENGTH"] = ""; // from REQUEST, POST
    envVars["HTTP_COOKIE"] = ""; // ??
    envVars["HTTP_USER_AGENT"] = ""; // name of web browser, from REQUEST
    envVars["PATH_INFO"] = ""; // path of cgi script, from CONFIG file
    envVars["QUERY_STRING"] = ""; // GET?
    envVars["REMOTE_ADDR"] = ""; // IP of clinet
    envVars["REMOTE_HOST"] = ""; // 
    envVars["REQUEST_METHOD"] = ""; // GET or POST, from REQUEST
    envVars["SCRIPT_FILENAME"] = ""; //full path of cgi script, from REQUEST and CONFIG file
    envVars["SCRIPT_NAME"] = ""; // name of cgi scripts, from REQUEST
    envVars["SERVER_SOFTWARE"] = ""; // name and version of running server.
    return (envVars);
} // ! are there more envVar?

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

char**  mapToArray(std::map<std::string, std::string> envVars)
{
    char **return_array = new char*[envVars.size() + 1];
    int k = 0;
    std::map<std::string, std::string>::const_iterator i;
    for (i = envVars.begin(); i != envVars.end(); i++)
    {
        std::string item = i->first + "=" + i->second;

        return_array[k] = new char[item.length() + 1];
        std::strcpy(return_array[k], item.c_str());
        k++;
    }
    return_array[k] = NULL;
    return return_array;
}


#define CGI_OUTPUT_FILE "new.txt"

std::string executeCgi(std::string & fileName, std::map<std::string, std::string> env_map)
{
    pid_t       pid;
    int         pipe_ret;
    
    int         status;
    char        buffer[CGI_BUFFER_SIZE + 1];
    std::string new_body;
    char        **envVec;
    char        **argVec;

    std::vector<std::string> stringVec;
    stringVec.push_back("php");
    stringVec.push_back(fileName);

    envVec = mapToArray(env_map);
    argVec = stringToArray(stringVec);

    pid = fork();
    if (pid == -1)
    {
        return (ERROR_500);
    }
    else if (pid == 0) // child
    {
        int fileFd = open(CGI_OUTPUT_FILE, O_WRONLY | O_CREAT, 0777); // everyone can access the file. Create the file if does not exist. Open it.
        if (fileFd == -1)
            return (ERROR_500);

        int fileFd2 = dup2(fileFd, STDOUT_FILENO); // change the STDOUT inside the child process.--> meaning it will print all the output on terminal into the file
        close (fileFd);
        // From now on, all result from this result won't be on the terminal, instead they will be on the file new.txt.
        if (execve(CMD_PHP_LINUX, argVec , envVec) == -1)
            return (ERROR_500);
    }
    else // main projcess
    {
        waitpid(-1, &status, 0); // -1: wait for any child process to die
        int fileFd = open(CGI_OUTPUT_FILE, O_RDONLY);
        int ret = 1;
		while (ret > 0)
		{
			std::memset(buffer, 0, CGI_BUFFER_SIZE);
			ret = read(fileFd, buffer, CGI_BUFFER_SIZE - 1);
			new_body += buffer;
		}
        close(fileFd);
	}
    // free memory
    for (size_t i = 0; envVec[i]; i++)
		delete[] envVec[i];
	delete[] envVec;
    return new_body;
}

int main()
{
    std::map<std::string, std::string> my_env = initEnv();
    std::string body;
    std::string fileName = "cgi_test.php";

    body = executeCgi(fileName, my_env);
    std::cout << body << std::endl;
}