#include "./multiplayer.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

void initMultiplayer()
{
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    int socketfd;
    char buf[100];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo("localhost", "3490", &hints, &servinfo);

    socketfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    connect(socketfd, servinfo->ai_addr, servinfo->ai_addrlen);

    int numBytes = recv(socketfd, buf, 100 -1, 0);
    buf[numBytes] = '\0';

    printf("testttting game socket %s\n", buf);
    close(socketfd);
}