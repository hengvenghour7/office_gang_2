#include "game.h"

Game createGame()
{
    Game game = {
        .tick = gameTick,
        .player = createPlayer(
            "../resources/character.png", 
            (Rectangle){1, 1, 20, 20},
            (Rectangle){1, 1, 20, 20}
        ),
        .world = createWorld("../resources/center_world.png", &game.player)
    };
    return game;
};
void gameTick(void* self)
{
    float deltaTime = GetFrameTime();
    Game* game = (Game*)(self);
    game->world.draw(&game->world);
    game->player.character.drawing.draw(&game->player.character);
    game->player.character.tick(&game->player.character, deltaTime);
}