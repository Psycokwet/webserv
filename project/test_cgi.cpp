#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <map>
#include <cstring>
#include <string>
#include <iostream>

  # define CGI_BUFSIZE 65536

  std::map<std::string, std::string>		initEnv()
  {
		std::map<std::string, std::string> env;
		// std::map<std::string, std::string>	headers = request.getHeaders();
		// if (headers.find("Auth-Scheme") != headers.end() && headers["Auth-Scheme"] != "")
		// 	env["AUTH_TYPE"] = headers["Authorization"];

		env["REDIRECT_STATUS"] = "200"; //Security needed to execute php-cgi
		env["GATEWAY_INTERFACE"] = "CGI/1.1";
		env["SCRIPT_NAME"] = "test.php";
		env["SCRIPT_FILENAME"] = "test.php";
		env["REQUEST_METHOD"] = "POST";
		// env["CONTENT_LENGTH"] = to_string(this->_body.length());
		// env["CONTENT_TYPE"] = headers["Content-Type"];
		// env["PATH_INFO"] = request.getPath(); //might need some change, using config path/contentLocation
		// env["PATH_TRANSLATED"] = request.getPath(); //might need some change, using config path/contentLocation
		// env["QUERY_STRING"] = request.getQuery();
		// env["REMOTEaddr"] = to_string(config.getHostPort().host);
		// env["REMOTE_IDENT"] = headers["Authorization"];
		// env["REMOTE_USER"] = headers["Authorization"];
		// env["REQUEST_URI"] = request.getPath() + request.getQuery();
		// if (headers.find("Hostname") != headers.end())
		// 	env["SERVER_NAME"] = headers["Hostname"];
		// else
		// 	env["SERVER_NAME"] = env["REMOTEaddr"];
		// env["SERVER_PORT"] = to_string(config.getHostPort().port);
		env["SERVER_PROTOCOL"] = "HTTP/1.1";
		env["SERVER_SOFTWARE"] = "Weebserv/1.0";

		// env.insert(config.getCgiParam().begin(), config.getCgiParam().end());
		return (env);
}

char					**GetEnvAsCstrArray(std::map<std::string, std::string> env) {
	char	**env_str = new char*[env.size() + 1];
	int	j = 0;
	for (std::map<std::string, std::string>::const_iterator i = env.begin(); i != env.end(); i++) {
		std::string	element = i->first + "=" + i->second;
		env_str[j] = new char[element.size() + 1];
		env_str[j] = strcpy(env_str[j], (const char*)element.c_str());
		j++;
	}
	env_str[j] = NULL;
	return env_str;
}


  int main()
  {
		pid_t pid;
		char *const parmList[] = {NULL};
		std::map<std::string, std::string> env_map = initEnv();
		char		**envParms = GetEnvAsCstrArray(env_map);

		int			saveStdin;
		int			saveStdout;
		std::string body;
		std::string	newBody;

		saveStdin = dup(STDIN_FILENO);
		saveStdout = dup(STDOUT_FILENO);

		FILE	*fIn = tmpfile();
		FILE	*fOut = tmpfile();
		long	fdIn = fileno(fIn);
		long	fdOut = fileno(fOut);
		int		ret = 1;

		std::string scriptName = "/mnt/nfs/homes/thi-nguy/sgoinfre/thi-nguy/webserv/project/test.php";


		write(fdIn, body.c_str(), body.size());
		lseek(fdIn, 0, SEEK_SET);


		// int len =strlen(*envParms);
		// printf("length of envParms: %d\n", len);

		// int i = 0;

		// while (envParms[i] != NULL)
		// {
		// 	printf("line %d: %s\n", i, envParms[i]);
		// 	i++;
		// }

		if ((pid = fork()) ==-1)
			perror("fork error");
		else if (pid == 0) {
			char * const * nll = NULL;

			dup2(fdIn, STDIN_FILENO);
			dup2(fdOut, STDOUT_FILENO);
			execve(scriptName.c_str(), nll, envParms);
			std::cerr << "Execve crashed." << std::endl;
			write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
		}
		else
		{
			char	buffer[CGI_BUFSIZE] = {0};

			waitpid(-1, NULL, 0);
			lseek(fdOut, 0, SEEK_SET);

			ret = 1;
			while (ret > 0)
			{
				memset(buffer, 0, CGI_BUFSIZE);
				ret = read(fdOut, buffer, CGI_BUFSIZE - 1);
				newBody += buffer;
			}
		}

		dup2(saveStdin, STDIN_FILENO);
		dup2(saveStdout, STDOUT_FILENO);
		fclose(fIn);
		fclose(fOut);
		close(fdIn);
		close(fdOut);
		close(saveStdin);
		close(saveStdout);

		std::cout << "newBody is: " << newBody << std::endl;

		return (0);
  }
