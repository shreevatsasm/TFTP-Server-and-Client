#ifndef UDP_H
#define UDP_H

#include <netinet/in.h>

int create_udp_socket();
void bind_socket(int sockfd, int port);
int send_data(int sockfd, char *buffer, int len, struct sockaddr_in *addr);
int receive_data(int sockfd, char *buffer, struct sockaddr_in *addr);

#endif