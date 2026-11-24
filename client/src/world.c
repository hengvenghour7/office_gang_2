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

}