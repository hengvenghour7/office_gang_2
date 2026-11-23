#ifndef CHARACTER_H
#define CHARACTER_H

#include <raylib.h>
#include <stdbool.h>

typedef struct Character {
    Texture2D texture;
    Rectangle source;
    Vector2 worldPosition; /* top-left position, in world-image pixels */
    float speed;           /* world-image pixels per second */
    float animationTimer;
    int animationFrame;
    int facingRow;
    bool moving;
} Character;

Character createCharacter(const char *imagePath, int frameColumn, int frameRow,
                          Vector2 startPosition);
void updateCharacter(Character *character, float deltaTime, int worldWidth, int worldHeight);
void drawCharacter(const Character *character);
void unloadCharacter(Character *character);

#endif
