#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
// #include <map>
// #include <cstring>
// #include <string>
// #include <iostream>

int main()
{
	char cmd[] = "/usr/bin/php";
	char * argVec[] = {"php", "test.php", NULL};
	char * envVec[] = {NULL};

	printf("Start of execve call %s\n", cmd);
	printf("==============================\n");

	if (execve(cmd, argVec, envVec) == -1)
		perror("could not execute execve");
	printf("Something went wrong!\n");


	return (0);
}

/**************************
 *    Some notes
 * Need to use pipe and then "php test.php"
 * execve
 * ***********************/