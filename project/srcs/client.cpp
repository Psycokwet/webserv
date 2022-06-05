// Client side
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <unistd.h>
#include <string>
#include <cstring>
#include <arpa/inet.h> // for inet_pton
#define PORT 8080

int main(void)
{
    int num_clients = 20;
    int valread;
    char client_message[] = "This is Request from one client\n"; // ! change to Request_sample
    char buffer[1024] = {0};

    // for (int i = 0; i < num_clients; i++)
    // {
        int client_socket;
        struct sockaddr_in server_addr;
        client_socket = socket(AF_INET, SOCK_STREAM, 0); // ! Quan trong nhat la buoc nay, tao fd voi cung AF_INET vaf SOCK_STREAM nhu server_fd
        if (client_socket < 0)
        {
            printf("\n Socket creation error \n");
            // continue ;
        }
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
        inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr); // convert IPv4 and IPv6 from text to binary form.
        connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
        send(client_socket, client_message, strlen(client_message), 0);
        // printf("Request sent from Client #%d\n", i);

        // valread = recv(client_socket, buffer, 10000000, MSG_WAITALL);
        // printf("%s\n", buffer);

    // }

    return (0);

}