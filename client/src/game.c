#include "game.h"
#include "raylib.h"
#include <fcntl.h>
#include <errno.h>

Game createGame()
{
    Game game = {
        .isMultiplayer = true,
        .tick = gameTick,
        .player = createPlayer(
            "../resources/character.png", 
            (Rectangle){50, 100, 20, 20},
            (Vector2){500, 500}
        ),
        .world = createWorld("../resources/center_world.png", &game.player)
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
}
const char* getGameDataAsString(Game* game)
{

}
void multiPlayerTick(Game* game)
{
    ServerPackage* serverPackage = &game->serverPackage;
    send(
        serverPackage->socketfd,
        "banana",
        strlen("banana"),
        0
    );
    int bytesReceived = recv(serverPackage->socketfd, serverPackage->buf, 100 -1, 0);
    if (bytesReceived > 0) {
        serverPackage->buf[bytesReceived] = '\0';
        printf("%s\n", serverPackage->buf);
    } else if (bytesReceived < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
        perror("recv");
    }
}
void endMultiplayer(Game* game)
{
    ServerPackage* serverPackage = &game->serverPackage;
    close(serverPackage->socketfd);
}