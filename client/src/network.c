#include "network.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

/* Kept as a separate helper so the rendering demo can run without a server. */
void connectToServer(void) {
    struct addrinfo hints = {0};
    struct addrinfo *serverInfo = NULL;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo("localhost", "3490", &hints, &serverInfo);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return;
    }

    int socketFd = socket(serverInfo->ai_family, serverInfo->ai_socktype,
                          serverInfo->ai_protocol);
    if (socketFd < 0) {
        perror("socket");
        freeaddrinfo(serverInfo);
        return;
    }

    if (connect(socketFd, serverInfo->ai_addr, serverInfo->ai_addrlen) < 0) {
        perror("connect");
        close(socketFd);
        freeaddrinfo(serverInfo);
        return;
    }

    char buffer[100] = {0};
    ssize_t received = recv(socketFd, buffer, sizeof(buffer) - 1, 0);
    if (received > 0) {
        buffer[received] = '\0';
        printf("next connection: %s\n", buffer);
    }

    close(socketFd);
    freeaddrinfo(serverInfo);
}
