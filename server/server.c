
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include "network_protocol.h"

#define PLAYER_TIMEOUT 15.0

// ==========================================
// CLIENT STRUCTURE
// ==========================================

typedef struct
{
    int active;
    int id;

    float x;
    float y;

    struct sockaddr_in address;

    double last_seen;
} Client;

Client clients[MAX_PLAYERS];

int next_player_id = 1;

// ==========================================
// TIME
// ==========================================

double get_time(void)
{
    struct timespec time;

    clock_gettime(CLOCK_MONOTONIC, &time);

    return time.tv_sec + time.tv_nsec / 1000000000.0;
}

// ==========================================
// FIND CLIENT
// ==========================================

int find_client(struct sockaddr_in *address)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!clients[i].active)
            continue;

        if (
            clients[i].address.sin_addr.s_addr == address->sin_addr.s_addr &&
            clients[i].address.sin_port == address->sin_port
        )
        {
            return i;
        }
    }

    return -1;
}

// ==========================================
// ADD CLIENT
// ==========================================

int add_client(struct sockaddr_in *address)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!clients[i].active)
        {
            clients[i].active = 1;

            clients[i].id = next_player_id++;

            clients[i].x = 400.0f;
            clients[i].y = 225.0f;

            clients[i].address = *address;

            clients[i].last_seen = get_time();

            printf("Player %d connected!\n", clients[i].id);

            return i;
        }
    }

    return -1;
}

// ==========================================
// REMOVE INACTIVE CLIENTS
// ==========================================

void remove_inactive_clients(void)
{
    double current_time = get_time();

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!clients[i].active)
            continue;

        if (current_time - clients[i].last_seen > PLAYER_TIMEOUT)
        {
            printf("Player %d disconnected!\n", clients[i].id);

            clients[i].active = 0;
        }
    }
}

// ==========================================
// SEND GAME STATE
// ==========================================

void send_game_state(
    int server_socket,
    int client_index
)
{
    GameState state = {0};

    state.type = MSG_GAME_STATE;
    state.your_id = clients[client_index].id;

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!clients[i].active)
            continue;

        int index = state.player_count;

        state.players[index].id = clients[i].id;
        state.players[index].x = clients[i].x;
        state.players[index].y = clients[i].y;

        state.player_count++;
    }

    sendto(
        server_socket,
        &state,
        sizeof(state),
        0,
        (struct sockaddr *)&clients[client_index].address,
        sizeof(clients[client_index].address)
    );
}

// ==========================================
// BROADCAST GAME STATE
// ==========================================

void broadcast_game_state(int server_socket)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!clients[i].active)
            continue;

        send_game_state(server_socket, i);
    }
}

// ==========================================
// MAIN
// ==========================================

int main(void)
{
    // 1. Create UDP socket

    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (server_socket < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    // 2. Configure server address

    struct sockaddr_in server_address = {0};

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    // 3. Bind socket

    if (bind(
        server_socket,
        (struct sockaddr *)&server_address,
        sizeof(server_address)
    ) < 0)
    {
        perror("Bind failed");
        close(server_socket);
        return 1;
    }

    printf("UDP Multiplayer Server started!\n");
    printf("Listening on port %d...\n", SERVER_PORT);

    // 4. Receive player updates

    while (1)
    {
        PlayerUpdate update;

        struct sockaddr_in client_address = {0};

        socklen_t client_length = sizeof(client_address);

        ssize_t bytes_received = recvfrom(
            server_socket,
            &update,
            sizeof(update),
            0,
            (struct sockaddr *)&client_address,
            &client_length
        );

        if (bytes_received < 0)
        {
            perror("Receive failed");
            continue;
        }

        if (bytes_received != sizeof(PlayerUpdate))
        {
            printf("Invalid packet size: %zd\n", bytes_received);
            continue;
        }

        if (update.type != MSG_PLAYER_UPDATE)
        {
            printf("Invalid message type: %u\n", update.type);
            continue;
        }

        // 5. Find or register client

        int client_index = find_client(&client_address);

        if (client_index == -1)
        {
            client_index = add_client(&client_address);
        }

        if (client_index == -1)
        {
            printf("Server is full!\n");
            continue;
        }

        // 6. Update player position

        clients[client_index].x = update.x;
        clients[client_index].y = update.y;

        clients[client_index].last_seen = get_time();

        // 7. Remove inactive clients

        remove_inactive_clients();

        // 8. Broadcast updated game state

        broadcast_game_state(server_socket);
    }

    close(server_socket);

    return 0;
}
