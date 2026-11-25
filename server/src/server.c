#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <networkHelpers.h>

#include <fcntl.h>
#include <stdlib.h>

#include "SGame.h"

int status;
struct addrinfo hints;
struct addrinfo *servinfo;
struct sockaddr_storage their_addr;
socklen_t addr_size;

int main (void) {

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    char buf[100];
    int onlineID = 0;

    SGame game = createGame();

    status = getaddrinfo("localhost", "4950", &hints, &servinfo);
    if (status != 0) {
        fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(status));
    }
    int sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    
    bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    char* gameData = getGameDataAsString(&game);
    printf("%s \n", gameData);
    free(gameData);
    while (1)
    {
        /* code */
        // int new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
        addr_size = sizeof their_addr;
            int yes = 1;
            int bytes_sent;
            recvfrom(sockfd, buf, 100 -1, 0,
                (struct sockaddr *)&their_addr,
                &addr_size
            );
            if (strcmp(buf, "initial_connection") == 0) {
                printf("new connection established\n");
                buf[0] = ' ';
            }
            bytes_sent = sendto(sockfd, "goat from the server", 20, 0, 
                (struct sockaddr *)&their_addr,
                addr_size
            );
    }
    
    return 0;
}