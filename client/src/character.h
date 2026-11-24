#ifndef CHARACTER_H
#define CHARACTER_H

#include <raylib.h>
#include "drawing.h"

enum CharacterState {
    Idle,
    Walking,
    Running
};
enum CharacterDirectionState {
    Left,
    Right,
    Up,
    Down,
};
typedef struct Character {
    Drawing drawing;
    Texture2D texture;
    Rectangle srcDimension;
    Rectangle characterPos;
    Rectangle characterScreenPos;
    enum CharacterState characterState;
    enum CharacterDirectionState characterDirectionState; 
    void (*tick)(int deltaTime);
} Character;
typedef struct Player {
    Character character;
} Player;
typedef struct AIPlayer {
    Character character;
} AIPlayer;
Character createCharacter(const char* textureSrc, Rectangle srcDimension, Rectangle dimension);
void drawCharacter(Character* character)
{
    DrawTexturePro(
        character->texture, 
        (Rectangle) {0, 0, 20, 20}, 
        (Rectangle) {0, 0, 20, 20},
        (Vector2){0, 0}, 0, WHITE
    );
};

#endif