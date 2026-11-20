#include <raylib.h>

typedef struct Character {
    Rectangle dimension;
    Texture2D Texture;
    int columnStart;
    int columnEnd;
    int row;
    int rowEnd;
} Character;

Character createCharacter (Rectangle dimension, const char * imgSrc, int columnEnd) {
    struct Character character = {
        .dimension = dimension,
        .Texture = LoadTexture(imgSrc),
        .columnStart = 0,
        .columnEnd = columnEnd,
        .row = 0,
        .rowEnd = 0,
    };
    return character;
}

void drawCharacter (Character* character) {
    DrawRectangle(character->dimension.x, character->dimension.y, 20, 20, RED);
}

void updateCharacter (Character* character) {
    
}