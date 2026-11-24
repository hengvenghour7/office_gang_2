#include "character.h"
#include "world.h"
typedef struct Game {
    Player player;
    World world;
    void (* tick)(void* self);
} Game;

Game createGame();
void gameTick(void* self);