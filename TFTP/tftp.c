#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "tftp.h"
#include "udp.h"
#include "common.h"

void send_file(int sockfd, struct sockaddr_in *addr, char *filename)
{
    FILE *fp = fopen(filename, "rb");
    char buffer[BUFFER_SIZE];

    if (!fp)
    {
        printf("File not found\n");
        return;
    }

    int n;

    while ((n = fread(buffer, 1, BUFFER_SIZE, fp)) > 0)
    {
        send_data(sockfd, buffer, n, addr);
        receive_data(sockfd, buffer, addr); // ACK
    }

    printf("File send complete\n");
    fclose(fp);
}

void receive_file(int sockfd, struct sockaddr_in *addr, char *filename)
{
    FILE *fp = fopen(filename, "wb");
    char buffer[BUFFER_SIZE];

    if (!fp)
    {
        printf("Cannot create file\n");
        return;
    }

    int n;

    while ((n = receive_data(sockfd, buffer, addr)) > 0)
    {
        fwrite(buffer, 1, n, fp);
        send_data(sockfd, "ACK", 3, addr);

        if (n < BUFFER_SIZE)
            break;
    }

    printf("File receive complete\n");
    fclose(fp);
}
