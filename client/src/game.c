#include "game.h"
#include "raylib.h"
#include "raymath.h"
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <math.h>

Game createGame()
{
    Game game = {
        .isMultiplayer = true,
        .onlineID = 0,
        .serverPackage = { .socketfd = -1 },
        .tick = gameTick,
        .otherPlayers = {0},
        .player = createPlayer(
            TextFormat("%s../Resources/character.png", GetApplicationDirectory()), 
            (Rectangle){50, 100, 20, 20},
            (Vector2){500, 500}
        ),
        .world = createWorld(TextFormat("%s../Resources/center_world.png", GetApplicationDirectory()), &game.player)
    };
    
    return game;
};
void gameTick(void* self)
{
    Game* game = (Game*)(self);
    if (game->isMultiplayer)
    {
        multiPlayerTick(game);
    }
    BeginDrawing();

        ClearBackground(RAYWHITE);

        float deltaTime = GetFrameTime();
        game->world.draw(&game->world);
        game->player.character.drawing.draw(&game->player.character);
        game->player.character.tick(&game->player.character, deltaTime);
        if (game->isMultiplayer)
        {
            int i = 0;
            while (i < MAX_PLAYERS)
            {
                if (game->otherPlayers[i].isActive)
                {
                    Character* character = &game->otherPlayers[i].player.character;
                    character->updateTime += deltaTime;
                    bool advanceFrame = character->updateTime >= character->maxUpdateTime;
                    if (advanceFrame) character->updateTime -= character->maxUpdateTime;
                    character->updateAnimation(character, advanceFrame);
                    drawOnlinePlayer(&game->player, &game->otherPlayers[i]);
                }
                i++;
            }
        }
    
    EndDrawing();
}

void initMultiplayer(Game* game)
{
    ServerPackage* serverPackage = &game->serverPackage;
    int status;
    

    memset(&serverPackage->hints, 0, sizeof serverPackage->hints);
    serverPackage->hints.ai_family = AF_UNSPEC;
    serverPackage->hints.ai_socktype = SOCK_DGRAM;

    status = getaddrinfo("localhost", "4950", &serverPackage->hints, &serverPackage->servinfo);
    if (status != 0) { fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status)); return; }

    serverPackage->socketfd = socket(
        serverPackage->servinfo->ai_family, 
        serverPackage->servinfo->ai_socktype, 
        serverPackage->servinfo->ai_protocol
    );
    if (serverPackage->socketfd < 0) { perror("socket"); freeaddrinfo(serverPackage->servinfo); serverPackage->servinfo = NULL; return; }
    if (connect(serverPackage->socketfd,
        serverPackage->servinfo->ai_addr, 
        serverPackage->servinfo->ai_addrlen
    ) < 0) { perror("connect"); close(serverPackage->socketfd); serverPackage->socketfd = -1; freeaddrinfo(serverPackage->servinfo); serverPackage->servinfo = NULL; return; }
    freeaddrinfo(serverPackage->servinfo); serverPackage->servinfo = NULL;

    uint32_t hello = MSG_HELLO;
    send(serverPackage->socketfd, &hello, sizeof hello, 0);
    struct timeval timeout = { .tv_sec = 1, .tv_usec = 0 };
    setsockopt(serverPackage->socketfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);
    int numBytes = recv(serverPackage->socketfd, serverPackage->buf, sizeof serverPackage->buf, 0);
    if (numBytes == sizeof(GameState)) {
        GameState state; memcpy(&state, serverPackage->buf, sizeof state);
        game->onlineID = state.your_id;
    }
    timeout.tv_sec = 0; timeout.tv_usec = 0;
    setsockopt(serverPackage->socketfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);
    int flags = fcntl(serverPackage->socketfd, F_GETFL, 0);
    fcntl(serverPackage->socketfd, F_SETFL, flags | O_NONBLOCK);

}
void multiPlayerTick(Game* game)
{
    ServerPackage* serverPackage = &game->serverPackage;
    if (serverPackage->socketfd < 0) return;
    PlayerUpdate update = { MSG_PLAYER_UPDATE, game->onlineID, game->player.character.characterPos.x, game->player.character.characterPos.y };
    send(serverPackage->socketfd, &update, sizeof update, 0);
    GameState state;
    int bytesReceived = recv(serverPackage->socketfd, &state, sizeof state, 0);
    if (bytesReceived == sizeof state && state.type == MSG_GAME_STATE) {
        game->onlineID = state.your_id;
        uint32_t count = state.player_count < MAX_PLAYERS ? state.player_count : MAX_PLAYERS;
        for (uint32_t i = 0; i < count; i++) {
            NetworkPlayer* p = &state.players[i];
            if (p->id == game->onlineID) continue;
            addGamePlayer(game, p->id, (int)p->x, (int)p->y);
            for (int j = 0; j < MAX_PLAYERS; j++) if (game->otherPlayers[j].isActive && game->otherPlayers[j].onlineID == p->id) {
                Character* character = &game->otherPlayers[j].player.character;
                Vector2 previousPos = character->characterPos;
                Vector2 updatedPos = {p->x, p->y};
                Vector2 movement = Vector2Subtract(updatedPos, previousPos);
                character->characterPos = updatedPos;
                if (fabsf(movement.x) > 0.001f || fabsf(movement.y) > 0.001f) {
                    character->characterState = Walking;
                    if (fabsf(movement.x) > fabsf(movement.y))
                        character->characterDirectionState = movement.x < 0 ? Left : Right;
                    else
                        character->characterDirectionState = movement.y < 0 ? Up : Down;
                } else {
                    character->characterState = Idle;
                }
                break;
            }
        }
    } else if (bytesReceived < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
        perror("recv");
    }
}
void endMultiplayer(Game* game)
{
    ServerPackage* serverPackage = &game->serverPackage;
    if (serverPackage->socketfd >= 0) close(serverPackage->socketfd);
}
void addGamePlayer(Game* game, int onlineID, int x, int y)
{
    int i = 0;
    while (i < MAX_PLAYERS)
    {
        if (game->onlineID == onlineID) return;
        if (game->otherPlayers[i].isActive && game->otherPlayers[i].onlineID == onlineID) return;
        if (!game->otherPlayers[i].isActive)
        {
            game->otherPlayers[i] = (OnlinePlayer){
                .isActive = true,
                .onlineID = onlineID,
                .player = createPlayer(
                    TextFormat("%s../Resources/character.png", GetApplicationDirectory()),
                    (Rectangle){50, 100, 20, 20},
                    (Vector2){x, y}
                )
            };
            printf("new player added %d\n", onlineID);
            return;
        }
        i++;
    }
}
