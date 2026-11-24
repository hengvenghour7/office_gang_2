#include "world.h"

World createWorld(const char* textureSrc)
{
    return (World) {
        .draw = drawWorld,
        .texture = LoadTexture(textureSrc),
    };
}
void drawWorld(void* self)
{
    World* world = (World*) self;
    DrawTextureEx(world->texture, (Vector2) {0, 0}, 0, 1, WHITE);
}