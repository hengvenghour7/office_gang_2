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
    };
    return game;
};
void gameTick(void* self)
{
    Game* game = (Game*)(self);
    game->player.character.drawing.draw(&game->player.character);
}