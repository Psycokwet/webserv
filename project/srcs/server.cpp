// Server side
#include <sys/socket.h> // for socket, setsockopt, bind, sockaddr structure, listen, accept
#include <sys/types.h> // for send
#include <cstdio> // for perror
#include <cstdlib> // for EXIT_FAILURE
#include <netinet/in.h> // for htons
#include <string> // for string and string.length
#include <unistd.h> // for read


#define PORT 8080

int  main(void)
{
    int server_fd;
    // int opt = 1;
    struct sockaddr_in server_address;
    int new_socket;
    int valread;
    char buffer[1024] = {0};
    char hello[] = "Hello from server";

    // server_fd = socket(domain, type, protocol); Create an endpoint for communication
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        perror("Fail to set socket\n");
        exit(EXIT_FAILURE);
    }
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    // // SO_REUSEADDR    enables local server_address reuse
    // // SO_REUSEPORT    enables duplicate server_address and port bindings
    // {
    //     perror("Fail to setsockopt");
    //     exit(EXIT_FAILURE);
    // }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);


    if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Fail to bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("Fail to listen");
        exit(EXIT_FAILURE);
    }
    int addrlen = sizeof(server_address);
    new_socket = accept(server_fd, (struct sockaddr *)&server_address, (socklen_t*)&addrlen);
    if (new_socket < 0)
    {
        perror("Fail to accept");
        exit(EXIT_FAILURE);
    }
    valread = read(new_socket, buffer, 1024);
    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("server's message sent\n");
    return (0);

}
