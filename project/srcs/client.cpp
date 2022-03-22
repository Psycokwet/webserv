// Client side
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <unistd.h>
#include <string>
#include <arpa/inet.h> // for inet_pton
#define PORT 8080

int main(void)
{
    int client_socket;
    struct sockaddr_in server_addr;
    int valread;
    char client_message[] = "This is message from client\n";
    char buffer[1024] = {0};

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        printf("\n Socket creation error \n");
        return (-1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr); // convert IPv4 and IPv6 from text to binary form.
    connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    send(client_socket, client_message, strlen(client_message), 0);
    printf("Client's message sent\n");
    valread = read(client_socket, buffer, 1024);
    printf("%s\n", buffer);
    return (0);

}