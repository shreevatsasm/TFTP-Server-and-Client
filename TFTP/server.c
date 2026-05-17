#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "udp.h"
#include "tftp.h"
#include "common.h"

int new_port = START_PORT;

void handle_client(struct sockaddr_in client_addr)
{
    int sockfd = create_udp_socket();

    bind_socket(sockfd, new_port);

    printf("Client handled on port %d\n", new_port);

    int port = new_port;
    new_port++;

    sendto(sockfd, &port, sizeof(port), 0,
           (struct sockaddr *)&client_addr,
           sizeof(client_addr));

    char command[100];
    char filename[100];

    while (1)
    {
        receive_data(sockfd, command, &client_addr);

        if (strcmp(command, "bye") == 0)
            break;

        if (strcmp(command, "get") == 0)
        {
            receive_data(sockfd, filename, &client_addr);
            send_file(sockfd, &client_addr, filename);
        }
        else if (strcmp(command, "put") == 0)
        {
            receive_data(sockfd, filename, &client_addr);
            receive_file(sockfd, &client_addr, filename);
        }
    }

    close(sockfd);
    exit(0);
}

int main()
{
    int sockfd = create_udp_socket();

    bind_socket(sockfd, SERVER_PORT);

    printf("Server running on port %d\n", SERVER_PORT);

    struct sockaddr_in client_addr;
    char buffer[BUFFER_SIZE];

    while (1)
    {
        receive_data(sockfd, buffer, &client_addr);

        if (fork() == 0)
        {
            handle_client(client_addr);
        }
    }
}