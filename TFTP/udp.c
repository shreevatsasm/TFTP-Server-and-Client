#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "udp.h"
#include "common.h"

int create_udp_socket()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
    }
    return sockfd;
}

void bind_socket(int sockfd, int port)
{
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
    }
}

int send_data(int sockfd, char *buffer, int len, struct sockaddr_in *addr)
{
    return sendto(sockfd, buffer, len, 0,
                  (struct sockaddr *)addr, sizeof(*addr));
}

int receive_data(int sockfd, char *buffer, struct sockaddr_in *addr)
{
    socklen_t len = sizeof(*addr);
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                     (struct sockaddr *)addr, &len);

    if (n >= 0)
        buffer[n] = '\0';

    return n;
}