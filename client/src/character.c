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
    return player;
}
void drawCharacter(void* self)
{
    Character* character = (Character *) self;
    DrawTexturePro(
        character->texture,
        (Rectangle) {0, 0, 20, 20},
        (Rectangle) {0, 80, 20, 20},
        (Vector2){0, 0}, 0, WHITE
    );
};