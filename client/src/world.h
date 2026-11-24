#ifndef WORLD_H
#define WORLD_H

#include <raylib.h>

typedef struct World {
    void (*draw)(void* self);
    Texture2D texture;        
} World;

World createWorld(const char* textureSrc);

void drawWorld(void* self);

#endif