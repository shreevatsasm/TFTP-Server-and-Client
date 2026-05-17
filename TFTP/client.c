#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "udp.h"
#include "tftp.h"
#include "common.h"

int main()
{
    int sockfd = create_udp_socket();

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char buffer[100];
    int new_port;

    strcpy(buffer, "connect");

    send_data(sockfd, buffer, strlen(buffer), &server_addr);

    recvfrom(sockfd, &new_port, sizeof(new_port), 0,
             (struct sockaddr *)&server_addr,
             &(socklen_t){sizeof(server_addr)});

    printf("Connected to server on port %d\n", new_port);

    server_addr.sin_port = htons(new_port);

    char filename[100];

    while (1)
    {
        printf("tftp> ");
        scanf("%s", buffer);

        if (strcmp(buffer, "bye") == 0)
        {
            send_data(sockfd, buffer, strlen(buffer), &server_addr);
            break;
        }

        if (strcmp(buffer, "get") == 0 || strcmp(buffer, "put") == 0)
        {
            printf("Enter filename: ");
            scanf("%s", filename);

            send_data(sockfd, buffer, strlen(buffer), &server_addr);
            send_data(sockfd, filename, strlen(filename), &server_addr);

            if (strcmp(buffer, "get") == 0)
                receive_file(sockfd, &server_addr, filename);
            else
                send_file(sockfd, &server_addr, filename);
        }
    }

    close(sockfd);
    return 0;
}