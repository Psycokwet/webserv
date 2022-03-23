// Server side
#include <sys/socket.h> // for socket, setsockopt, bind, sockaddr structure, listen, accept
#include <sys/types.h> // for send
#include <cstdio> // for perror
#include <cstdlib> // for EXIT_FAILURE
#include <netinet/in.h> // for htons
#include <string> // for string and string.length
#include <unistd.h> // for read
#include <sys/select.h> // for FD_CLR, FD_ZERO, FD_SET, FD_ISSET macros
#include <errno.h> // for errno, EINTR
#include <arpa/inet.h> 


#define PORT 8080
#define TRUE 1
#define FALSE 0

int  main(void)
{
    int server_fd;
    // int opt = 1;
    struct sockaddr_in server_address;
    int new_socket;
    int valread;
    char buffer[1024];
    char message[] = "Hello from server";
    int i;
    int max_clients = 30;
    int client_socket[max_clients];
    int opt = TRUE;
    int addrlen;
    int max_sd;
    int sd;
    int activity;

    // set of socket descriptors
    fd_set readfds;
    
    // initialize add client socket to 0, so not checked
    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    // server_fd = socket(domain, type, protocol); Create an endpoint for communication
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        perror("Fail to set socket\n");
        exit(EXIT_FAILURE);
    }
    //set master socket (server_fd) to allow multiple connections , 
    //this is just a good habit, it will work without this //set master socket to allow multiple connections , 
    //this is just a good habit, it will work without this 
    // SO_REUSEADDR    enables local server_address reuse
    // SO_REUSEPORT    enables duplicate server_address and port bindings
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
    {
        perror("Fail to setsockopt");
        exit(EXIT_FAILURE);
    }
    // type of socket created:
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // bind the socket to localhost port 8080
    if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Fail to bind");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port number %d\n", PORT);

    // Try to specify maximun of 3 pending connection for the master socket (server_fd)
    if (listen(server_fd, 3) < 0)
    {
        perror("Fail to listen");
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection
    addrlen = sizeof(server_address);
    puts("Waiting for connections...");

    while (TRUE)
    {
        // Clear the scoket set
        FD_ZERO(&readfds);

        // add master socket to set
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        // add child sockets to set
        for (i = 0; i < max_clients; i++)
        {
            // socket descriptor
            sd = client_socket[i];

            // if valid socket, then add to set
            if (sd > 0)
                FD_SET(sd, &readfds);

            // get highest fd, will need it for select function
            if (sd > max_sd)
                max_sd = sd;
        }

        //wait for an activity on one of the sockets, timeout is NULL so wait permernently
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
            printf("Select error\n");

        // if something happened on the master socket, then it's an incomming connection
        if (FD_ISSET(server_fd, &readfds))
        {
            new_socket = accept(server_fd, (struct sockaddr *)&server_address, (socklen_t*)&addrlen);
            if (new_socket < 0)
            {
                perror("Fail to accept");
                exit(EXIT_FAILURE);
            }
            //inform user of socket number - used in send and receive commands 
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(server_address.sin_addr) , ntohs(server_address.sin_port));  

            // send new connection greeting message
            if (send(new_socket, message, strlen(message), 0) != strlen(message))
                perror("send");
            puts ("Welcome message sent successfully");

            // add new socket to array of sockets
            for (i = 0; i < max_clients; i++)
            {
                // if position is empty
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n", i);

                    break ;
                }
            }
        }

        // else, it comes fromt IO operation on some other socket
        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];
            if (FD_ISSET(sd, &readfds))
            {
                // check if it was for closing, and also read the incoming message
                if ((valread = read(sd, buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print 
                    getpeername(sd , (struct sockaddr*)&server_address , \
                        (socklen_t*)&addrlen);  
                    printf("Host disconnected , ip %s , port %d \n" , 
                          inet_ntoa(server_address.sin_addr) , ntohs(server_address.sin_port));  
                         
                    //Close the socket and mark as 0 in list for reuse 
                    close( sd );  
                    client_socket[i] = 0;  
                }
                //Echo back the message that came in 
                else 
                {  
                    //set the string terminating NULL byte on the end 
                    //of the data read 
                    buffer[valread] = '\0';  
                    send(sd , buffer , strlen(buffer) , 0 );  
                }
            }
        }
    }
    return (0);
}
