#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <networkHelpers.h>

#include <stdlib.h>

int status;
struct addrinfo hints;
struct addrinfo *servinfo;
struct sockaddr_storage their_addr;
socklen_t addr_size;

int main (void) {

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo("localhost", "3490", &hints, &servinfo);
    if (status != 0) {
        fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(status));
    }
    int sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    
    bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
    listen (sockfd, 10);
    while (1)
    {
        /* code */
        int new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
        if (!fork()) {
            printf("waiting\n");
            int yes = 1;
            int bytes_sent;
        
            bytes_sent = send(new_fd, "goat from the server", 20, 0);
        }
        printf("next connection");
    }
    
    return 0;
}