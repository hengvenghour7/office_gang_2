
#include "raylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include "network_protocol.h"

// ==========================================
// NETWORK DEFINITIONS
// ==========================================

#define PLAYER_RADIUS 20.0f
#define PLAYER_SPEED 250.0f

// ==========================================
// MAIN
// ==========================================

int main(int argc, char **argv)
{
    // --------------------------------------
    // 1. CREATE UDP SOCKET
    // --------------------------------------

    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (client_socket < 0)
    {
        perror("Failed to create socket");
        return 1;
    }

    // Make socket non-blocking
    int flags = fcntl(client_socket, F_GETFL, 0);

    if (flags < 0 || fcntl(client_socket, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        perror("Failed to set socket non-blocking");
        close(client_socket);
        return 1;
    }

    // --------------------------------------
    // 2. CONFIGURE SERVER ADDRESS
    // --------------------------------------

    struct sockaddr_in server_address;

    memset(&server_address, 0, sizeof(server_address));

    const char *server_ip = argc > 1 ? argv[1] : getenv("SERVER_IP");
    if (server_ip == NULL || server_ip[0] == '\0')
        server_ip = "127.0.0.1";

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, server_ip, &server_address.sin_addr) != 1)
    {
        fprintf(stderr, "Invalid server IP address: %s\n", server_ip);
        close(client_socket);
        return 1;
    }

    printf("Client socket created. Connecting to %s:%d over UDP.\n", server_ip, SERVER_PORT);

    // --------------------------------------
    // 3. INITIALIZE GAME
    // --------------------------------------

    const int screen_width = 800;
    const int screen_height = 450;

    InitWindow(screen_width, screen_height, "UDP Multiplayer Game");
    SetTargetFPS(60);

    Vector2 player_position = {
        screen_width / 2.0f,
        screen_height / 2.0f
    };

    GameState game_state = {0};

    int my_id = -1;

    float send_timer = 0.0f;
    const float send_interval = 0.05f;

    bool received_server_state = false;

    // --------------------------------------
    // 4. GAME LOOP
    // --------------------------------------

    while (!WindowShouldClose())
    {
        float delta_time = GetFrameTime();

        // ==================================
        // PLAYER MOVEMENT
        // ==================================

        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
            player_position.x += PLAYER_SPEED * delta_time;

        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
            player_position.x -= PLAYER_SPEED * delta_time;

        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
            player_position.y -= PLAYER_SPEED * delta_time;

        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
            player_position.y += PLAYER_SPEED * delta_time;

        // Keep player inside the screen
        if (player_position.x < PLAYER_RADIUS)
            player_position.x = PLAYER_RADIUS;

        if (player_position.x > screen_width - PLAYER_RADIUS)
            player_position.x = screen_width - PLAYER_RADIUS;

        if (player_position.y < PLAYER_RADIUS)
            player_position.y = PLAYER_RADIUS;

        if (player_position.y > screen_height - PLAYER_RADIUS)
            player_position.y = screen_height - PLAYER_RADIUS;

        // ==================================
        // SEND PLAYER POSITION TO SERVER
        // ==================================

        send_timer += delta_time;

        if (send_timer >= send_interval)
        {
            send_timer = 0.0f;

            PlayerUpdate update;

            update.type = MSG_PLAYER_UPDATE;
            update.x = player_position.x;
            update.y = player_position.y;

            ssize_t bytes_sent = sendto(
                client_socket,
                &update,
                sizeof(update),
                0,
                (struct sockaddr *)&server_address,
                sizeof(server_address)
            );

            if (bytes_sent < 0)
            {
                perror("Failed to send player update");
            }
        }

        // ==================================
        // RECEIVE GAME STATE FROM SERVER
        // ==================================

        while (1)
        {
            GameState incoming_state;

            ssize_t bytes_received = recvfrom(
                client_socket,
                &incoming_state,
                sizeof(incoming_state),
                0,
                NULL,
                NULL
            );

            if (bytes_received < 0)
            {
                if (errno != EAGAIN && errno != EWOULDBLOCK)
                {
                    perror("Failed to receive server data");
                }

                break;
            }

            // Validate received packet
            if (bytes_received != sizeof(GameState))
                continue;

            if (incoming_state.type != MSG_GAME_STATE)
                continue;

            if (incoming_state.player_count > MAX_PLAYERS)
                continue;

            // Store latest server state
            game_state = incoming_state;
            my_id = incoming_state.your_id;

            received_server_state = true;
        }

        // ==================================
        // DRAW GAME
        // ==================================

        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw all players received from server
        if (received_server_state)
        {
            for (uint32_t i = 0; i < game_state.player_count; i++)
            {
                NetworkPlayer player = game_state.players[i];

                Vector2 position = {
                    player.x,
                    player.y
                };

                // Own player = red
                // Other players = blue
                Color player_color =
                    (player.id == my_id) ? RED : BLUE;

                DrawCircleV(
                    position,
                    PLAYER_RADIUS,
                    player_color
                );

                DrawText(
                    TextFormat("P%d", player.id),
                    (int)position.x - 12,
                    (int)position.y - 35,
                    20,
                    BLACK
                );
            }
        }
        else
        {
            DrawText(
                "Waiting for server...",
                20,
                20,
                20,
                DARKGRAY
            );
        }

        // ----------------------------------
        // DISPLAY NETWORK INFORMATION
        // ----------------------------------

        DrawText(
            TextFormat("Your ID: %d", my_id),
            20,
            screen_height - 60,
            20,
            DARKGRAY
        );

        DrawText(
            TextFormat("Players: %d", game_state.player_count),
            20,
            screen_height - 35,
            20,
            DARKGRAY
        );

        DrawText(
            "Move: WASD / Arrow Keys",
            screen_width - 250,
            20,
            18,
            DARKGRAY
        );

        EndDrawing();
    }

    // --------------------------------------
    // 5. CLEANUP
    // --------------------------------------

    CloseWindow();

    close(client_socket);

    printf("Client disconnected.\n");

    return 0;
}
