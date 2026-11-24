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
    void (*tick)(void* self);
    void (*updateAnimation)(void* self);
    int initialColumn;
    int maxColumn;
    int row;
} Character;
typedef struct Player {
    Character character;
    Vector2 (*getPlayerPos)(Vector2 pos);
} Player;
typedef struct AIPlayer {
    Character character;
} AIPlayer;
Character createCharacter(const char* textureSrc, Rectangle srcDimension, Rectangle dimension);
void drawCharacter(void* self);
Player createPlayer(const char* textureSrc, Rectangle srcDimension, Rectangle dimension);
void playerTick(void* self);
void updateCharacterAnimation(void* self);
Vector2 getPlayerPos(Vector2 pos);

#endif