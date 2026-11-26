#include "game.h"
#include "raylib.h"
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <networkHelpers.h>

Game createGame()
{
    Game game = {
        .isMultiplayer = true,
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

    serverPackage->socketfd = socket(
        serverPackage->servinfo->ai_family, 
        serverPackage->servinfo->ai_socktype, 
        serverPackage->servinfo->ai_protocol
    );
    connect(serverPackage->socketfd, 
        serverPackage->servinfo->ai_addr, 
        serverPackage->servinfo->ai_addrlen
    );

    send(serverPackage->socketfd, "initial_connection", 20, 0);
    int numBytes = recv(serverPackage->socketfd, serverPackage->buf, 100 -1, 0);
    // serverPackage->buf[numBytes] = '\0';
    int flags = fcntl(serverPackage->socketfd, F_GETFL, 0);
    fcntl(serverPackage->socketfd, F_SETFL, flags | O_NONBLOCK);

    printf("testttting game socket %s\n", serverPackage->buf);

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    setsockopt(
        serverPackage->socketfd,
        SOL_SOCKET,
        SO_RCVTIMEO,
        &tv,
        sizeof(tv)
    );
    int bytesReceived = recv(serverPackage->socketfd, serverPackage->buf, 100 -1, 0);
    if (bytesReceived > 0) {
        serverPackage->buf[bytesReceived] = '\0';
        char* str = malloc(100);
        strcpy(str, serverPackage->buf);
        char* myPtr = strtok(str, ",");
        while (myPtr != NULL)
        {
            int temp_id, x, y;
            temp_id = atoi(myPtr);
            myPtr = strtok(NULL, ",");
            if (myPtr == NULL) break;
            x = atoi(myPtr);
            myPtr = strtok(NULL, ",");
            if (myPtr == NULL) break;
            y = atoi(myPtr);
            myPtr = strtok(NULL, ",");
            addGamePlayer(game, temp_id, x, y);
            // updatePlayers(game, temp_id, x, y);
        }
        printf("yyy %s\n", str);
        }
}
char* getGameDataAsString(Game* game)
{
    char* s = malloc(2048);
    s[0] = '\0';
    int index = 0;
    addIntToString(s, game->onlineID);
    strcat(s,",");
    addIntToString(s, game->player.character.characterPos.x);
    strcat(s,",");
    addIntToString(s, game->player.character.characterPos.y);
    return s;
}
void multiPlayerTick(Game* game)
{
    ServerPackage* serverPackage = &game->serverPackage;
    char* gameData = getGameDataAsString(game);
    int Bytessend = send(serverPackage->socketfd, gameData, 100 -1, 0);
    // printf("data %s \n", gameData);
    free(gameData);
    
    int bytesReceived = recv(serverPackage->socketfd, serverPackage->buf, 100 -1, 0);
    if (bytesReceived > 0) {
        serverPackage->buf[bytesReceived] = '\0';
        char* str = malloc(100);
        strcpy(str, serverPackage->buf);
        char* myPtr = strtok(str, ",");
        while (myPtr != NULL)
        {
            int temp_id, x, y;
            temp_id = atoi(myPtr);
            myPtr = strtok(NULL, ",");
            if (myPtr == NULL) break;
            x = atoi(myPtr);
            myPtr = strtok(NULL, ",");
            if (myPtr == NULL) break;
            y = atoi(myPtr);
            myPtr = strtok(NULL, ",");
            addGamePlayer(game, temp_id, x, y);
            // updatePlayers(game, temp_id, x, y);
        }
        // printf("%s\n", str);
        free(str);
    } else if (bytesReceived < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
        perror("recv");
    }
    
}
void endMultiplayer(Game* game)
{
    ServerPackage* serverPackage = &game->serverPackage;
    close(serverPackage->socketfd);
}
void addGamePlayer(Game* game, int onlineID, int x, int y)
{
    int i = 0;
    while (i < MAX_PLAYERS)
    {
        if (game->onlineID == onlineID || game->otherPlayers[i].onlineID == onlineID) return;
        if (!game->otherPlayers[i].isActive)
        {
            game->otherPlayers[i] = (OnlinePlayer){
                .isActive = true,
                .onlineID = onlineID,
                .player = createPlayer(
                    "../resources/character.png", 
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
void updatePlayers(Game* game, int onlineID, int x, int y)
{
    if (game->onlineID == onlineID)
    {
        game->player.character.characterPos.x = x;
        game->player.character.characterPos.y = y;
        return;
    }
    int i = 0;
    while (i < MAX_PLAYERS)
    {
        OnlinePlayer* currentP = &game->otherPlayers[i];
        if (!currentP->isActive || currentP->onlineID != onlineID){
            i++;
            continue;
        }
        currentP->player.character.characterPos.x = x;
        currentP->player.character.characterPos.y = y;
        return;
    }
}