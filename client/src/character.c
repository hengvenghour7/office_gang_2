#include "character.h"

Character createCharacter (const char* textureSrc, Rectangle srcDimension, Rectangle dimension) {
    Character character = {
        .drawing.draw = drawCharacter,
        .texture = LoadTexture(textureSrc),
        .srcDimension = srcDimension,
        .characterPos = dimension,
    };
    return character;
};