#include "character.h"
#include "raymath.h"

Character createCharacter (const char* textureSrc, Rectangle srcDimension, Rectangle dimension) {
    Character character = {
        .drawing.draw = drawCharacter,
        .texture = LoadTexture(textureSrc),
        .srcDimension = srcDimension,
        .characterPos = dimension,
        .initialColumn = 0,
        .currentColumn = character.initialColumn,
        .row = 0,
        .cropWidth = 256/8,
        .cropHeight = 128/4,
        .characterState = Idle,
        .updateAnimation = updateCharacterAnimation,
        .updateTime = 0,
        .maxUpdateTime = 0.1,
    };
    return character;
};

Player createPlayer(const char* textureSrc, Rectangle srcDimension, Rectangle dimension)
{
    Player player = {
        .character = createCharacter(textureSrc, srcDimension, dimension)
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
        (Rectangle) {character->characterPos.x, character->characterPos.y, 20, 20},
        (Vector2){0, 0}, 0, WHITE
    );
};
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
Vector2 getPlayerPos(Vector2 pos)
{
    return pos;
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
    player->character.characterPos.x += direction.x;
    player->character.characterPos.y += direction.y;
    if (direction.y != 0 || direction.x != 0) {
        player->character.characterState = Walking;
    }
    else {
        player->character.characterState = Idle;
    }
}