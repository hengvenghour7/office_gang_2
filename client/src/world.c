#include "world.h"
#include "globalVar.h"

World createWorld(const char* textureSrc, Player* player)
{
    return (World) {
        .draw = drawWorld,
        .texture = LoadTexture(textureSrc),
    };
}
void drawWorld(void* self)
{
    World* world = (World*) self;
    
    DrawTextureEx(world->texture, (Vector2) {0, 0}, 0, SCALE, WHITE);
}