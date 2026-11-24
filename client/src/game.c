#include "game.h"
#include "raylib.h"
#include "multiplayer/multiplayer.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

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
        gameMultiplayerTick(game);
    }
    BeginDrawing();

        ClearBackground(RAYWHITE);

        float deltaTime = GetFrameTime();
        game->world.draw(&game->world);
        game->player.character.drawing.draw(&game->player.character);
        game->player.character.tick(&game->player.character, deltaTime);
    
    EndDrawing();
}
void gameMultiplayerTick(void* self)
{
    Game* game = (Game*)self;
}