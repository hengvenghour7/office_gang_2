#include "character.h"
typedef struct Game {
    Player player;
    void (* tick)(void);
} Game;

Game createGame()
{
    Game game = {
        
    };
    return game;
};
void gameTick()
{

};