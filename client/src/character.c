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
        (Rectangle) {0, 0, 20, 20},
        (Rectangle) {character->characterPos.x, 80, 20, 20},
        (Vector2){0, 0}, 0, WHITE
    );
};
void updateCharacterAnimation(void* self)
{
    Character* character = (Character*) self;
    switch (character->characterState)
    {
    case Idle:
        character->row = 0;
        character->maxColumn = 5;
        break;
    case Walking:
        character->row = 1;
        character->maxColumn = 8;
        break;
    default:
        break;
    }

}
Vector2 getPlayerPos(Vector2 pos)
{
    return pos;
}
void playerTick(void* self)
{
    Player* player = (Player*) self;
    if (IsKeyDown(KEY_D))
    {
        player->character.characterPos.x += 1;
    }
}