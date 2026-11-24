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
    void (*tick)(void* self, float deltaTime);
    void (*updateAnimation)(void* self, bool isUpdateTime);
    float updateTime;
    float maxUpdateTime;
    int initialColumn;
    int currentColumn;
    int maxColumn;
    int cropWidth;
    int cropHeight;
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
void playerTick(void* self, float deltaTime);
void updateCharacterAnimation(void* self, bool isUpdateTime);
Vector2 getPlayerPos(Vector2 pos);

#endif