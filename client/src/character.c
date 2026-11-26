#include "character.h"
#include "raymath.h"
#include "globalVar.h"

Character createCharacter (const char* textureSrc, Rectangle srcDimension, Vector2 pos) {
    Character character = {
        .drawing.draw = drawCharacter,
        .texture = LoadTexture(textureSrc),
        .characterPos = pos,
        .initialColumn = 0,
        .currentColumn = character.initialColumn,
        .row = 0,
        .cropWidth = 256/8,
        .cropHeight = 128/4,
        .characterScreenPos = (Vector2){GetScreenWidth()*0.5 + character.cropWidth*0.5, GetScreenHeight()*0.5},
        .characterState = Idle,
        .updateAnimation = updateCharacterAnimation,
        .updateTime = 0,
        .maxUpdateTime = 0.1,
        .getPos = getCharacterPos
    };
    return character;
};

Player createPlayer(const char* textureSrc, Rectangle srcDimension, Vector2 pos)
{
    Player player = {
        .character = createCharacter(textureSrc, srcDimension, pos),
        .getPlayerPosForRender = getPlayerPosForRender
    };
    player.character.tick = playerTick;
    return player;
}
void drawCharacter(void* self)
{
    Character* character = (Character *) self;
    DrawTexturePro(
        character->texture,
        (Rectangle) {character->currentColumn * character->cropWidth, character->row * character->cropHeight, character->cropWidth, character->cropHeight},
        (Rectangle) {character->characterScreenPos.x, character->characterScreenPos.y, character->cropWidth * CHARACTER_SCALE, character->cropHeight * CHARACTER_SCALE},
        (Vector2){0, 0}, 0, WHITE
    );
};
void drawOnlinePlayer(Player* player, OnlinePlayer* onlinePlayer)
{
    Character* character = &onlinePlayer->player.character;
    Vector2 drawPos = Vector2Add(getPlayerPosForRender(player), character->characterPos);
    DrawTexturePro(
        character->texture,
        (Rectangle) {character->currentColumn * character->cropWidth, character->row * character->cropHeight, character->cropWidth, character->cropHeight},
        (Rectangle) {drawPos.x, drawPos.y, character->cropWidth * CHARACTER_SCALE, character->cropHeight * CHARACTER_SCALE},
        (Vector2){0, 0}, 0, WHITE
    );
}
void updateCharacterAnimation(void* self, bool isUpdateTime)
{
    Character* character = (Character*) self;
    switch (character->characterState)
    {
    case Idle:
        character->row = 0;
        character->maxColumn = 4;
        break;
    case Walking:
        character->row = 1;
        character->maxColumn = 8;
        break;
    default:
        break;
    }
    if (!isUpdateTime) return;
    character->currentColumn++;
    if (character->currentColumn > character->maxColumn)
    {
        character->currentColumn = character->initialColumn;
    }
}
Vector2 getCharacterPos(void * self)
{
    Player* player = (Player*) self;
    return player->character.characterPos;
}
Vector2 getPlayerPosForRender(Player* player)
{
    return Vector2Scale(
        Vector2Subtract(player->character.characterPos, player->character.characterScreenPos), 
        -1.0
    );
}
void playerTick(void* self, float deltaTime)
{
    Player* player = (Player*) self;
    Vector2 direction = {0, 0};
    bool isUpdateTime = false;
    
    player->character.updateTime += deltaTime;
    if (player->character.updateTime > player->character.maxUpdateTime)
    {
        isUpdateTime = true;
        player->character.updateTime = 0;
    }

    player->character.updateAnimation(player, isUpdateTime);
    if (IsKeyDown(KEY_D))
    {
        direction.x = 1;
    }
    if (IsKeyDown(KEY_A))
    {
        direction.x = -1;
    }
    if (IsKeyDown(KEY_W))
    {
        direction.y = -1;
    }
    if (IsKeyDown(KEY_S))
    {
        direction.y = 1;
    }
    Vector2 normalizeDirection = Vector2Normalize(direction);
    player->character.characterPos = Vector2Add(player->character.characterPos, normalizeDirection);
    if (normalizeDirection.y != 0 || normalizeDirection.x != 0) {
        player->character.characterState = Walking;
    }
    else {
        player->character.characterState = Idle;
    }
}
