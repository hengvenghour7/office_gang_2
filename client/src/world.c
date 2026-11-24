#include "world.h"
#include "globalVar.h"
#include <raymath.h>

World createWorld(const char* textureSrc, Player* player)
{
    return (World) {
        .draw = drawWorld,
        .texture = LoadTexture(textureSrc),
        .player = player
    };
}
void drawWorld(void* self)
{
    World* world = (World*) self;
    Vector2 playerPos = world->player->getPlayerPosForRender(world->player);
    DrawTextureEx(world->texture, playerPos, 0, SCALE, WHITE);
}