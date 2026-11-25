#ifndef CHARACTER_H
#define CHARACTER_H

#include <raylib.h>
#include "drawing.h"

enum CharacterState {
    Attacking,
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
    Vector2 characterPos;
    Vector2 characterScreenPos;
    enum CharacterState characterState;
    enum CharacterDirectionState characterDirectionState; 
    void (*tick)(void* self, float deltaTime);
    void (*updateAnimation)(void* self, bool isUpdateTime);
    Vector2 (*getPos)(void* self);
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
    Vector2 (*getPlayerPosForRender)(struct Player* player);
} Player;
typedef struct AIPlayer {
    Character character;
} AIPlayer;
typedef struct OnlinePlayer {
    int onlineID;
    bool isActive;
    Player player;
} OnlinePlayer;
Character createCharacter(const char* textureSrc, Rectangle srcDimension, Vector2 pos);
void drawCharacter(void* self);
Player createPlayer(const char* textureSrc, Rectangle srcDimension, Vector2 Pos);
void playerTick(void* self, float deltaTime);
void updateCharacterAnimation(void* self, bool isUpdateTime);
Vector2 getCharacterPos(void* self);
Vector2 getPlayerPosForRender(Player* player);
void drawOnlinePlayer(Player* player, OnlinePlayer* onlinePlayer);

#endif