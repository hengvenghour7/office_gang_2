#ifndef WORLD_H
#define WORLD_H

#include <raylib.h>
#include "character.h"

typedef struct World {
    void (*draw)(void* self);
    Texture2D texture;
    Player* player;
} World;

World createWorld(const char* textureSrc, Player* player);

void drawWorld(void* self);

#endif