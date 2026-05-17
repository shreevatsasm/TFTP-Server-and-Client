#ifndef TFTP_H
#define TFTP_H

#include <netinet/in.h>

void send_file(int sockfd, struct sockaddr_in *addr, char *filename);
void receive_file(int sockfd, struct sockaddr_in *addr, char *filename);

#endif