#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "SGame.h"

typedef struct Client {
    int active;
    int id;
    struct sockaddr_storage addr;
    socklen_t addr_len;
} Client;

static int sameAddress(const struct sockaddr_storage* a, const struct sockaddr_storage* b)
{
    if (a->ss_family != b->ss_family) return 0;
    if (a->ss_family == AF_INET) {
        const struct sockaddr_in* x = (const struct sockaddr_in*)a;
        const struct sockaddr_in* y = (const struct sockaddr_in*)b;
        return x->sin_port == y->sin_port && x->sin_addr.s_addr == y->sin_addr.s_addr;
    }
    if (a->ss_family == AF_INET6) {
        const struct sockaddr_in6* x = (const struct sockaddr_in6*)a;
        const struct sockaddr_in6* y = (const struct sockaddr_in6*)b;
        return x->sin6_port == y->sin6_port && memcmp(&x->sin6_addr, &y->sin6_addr, sizeof x->sin6_addr) == 0;
    }
    return 0;
}

int main(void)
{
    struct addrinfo hints = {0}, *servinfo = NULL;
    hints.ai_family = AF_UNSPEC; hints.ai_socktype = SOCK_DGRAM; hints.ai_flags = AI_PASSIVE;
    int status = getaddrinfo(NULL, "4950", &hints, &servinfo);
    if (status != 0) { fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status)); return 1; }
    int sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (sockfd < 0 || bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) { perror("server socket/bind"); freeaddrinfo(servinfo); return 1; }
    freeaddrinfo(servinfo);
    int flags = fcntl(sockfd, F_GETFL, 0); fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    SGame game = createGame();
    Client clients[MAX_PLAYERS] = {0};
    int next_id = 1;
    for (;;) {
        struct sockaddr_storage peer = {0}; socklen_t peer_len = sizeof peer;
        unsigned char packet[sizeof(PlayerUpdate)];
        ssize_t received = recvfrom(sockfd, packet, sizeof packet, 0, (struct sockaddr*)&peer, &peer_len);
        if (received >= (ssize_t)sizeof(uint32_t)) {
            uint32_t type;
            memcpy(&type, packet, sizeof type);
            int slot = -1;
            for (int i = 0; i < MAX_PLAYERS; i++) if (clients[i].active && sameAddress(&clients[i].addr, &peer)) { slot = i; break; }
            if (type == MSG_HELLO && slot < 0) {
                for (int i = 0; i < MAX_PLAYERS; i++) if (!clients[i].active) { slot = i; break; }
                if (slot >= 0) {
                    clients[slot] = (Client){ .active = 1, .id = next_id++, .addr = peer, .addr_len = peer_len };
                    addPlayer(&game, clients[slot].id);
                }
            } else if (type == MSG_PLAYER_UPDATE && received == (ssize_t)sizeof(PlayerUpdate)) {
                PlayerUpdate update; memcpy(&update, packet, sizeof update);
                for (int i = 0; i < MAX_PLAYERS; i++) if (clients[i].active && sameAddress(&clients[i].addr, &peer) && clients[i].id == update.id) {
                    game.allPlayers[i].pos.x = update.x; game.allPlayers[i].pos.y = update.y; break;
                }
            }
            if (slot >= 0 && clients[slot].active) {
                GameState state = getGameState(&game, clients[slot].id);
                sendto(sockfd, &state, sizeof state, 0, (struct sockaddr*)&clients[slot].addr, clients[slot].addr_len);
            }
        }
        usleep(1000);
    }
}
