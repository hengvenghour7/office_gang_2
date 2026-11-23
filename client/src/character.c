#include "character.h"

#include <stddef.h>

#define CHARACTER_FRAME_SIZE 32
#define WORLD_TILE_SIZE 16
#define ANIMATION_FRAME_COUNT 8
#define ANIMATION_FRAME_DURATION 0.10f

static void selectFrame(Character *character) {
    character->source.x = (float)(character->animationFrame * CHARACTER_FRAME_SIZE);
    character->source.y = (float)(character->facingRow * CHARACTER_FRAME_SIZE);
}

Character createCharacter(const char *imagePath, int frameColumn, int frameRow,
                          Vector2 startPosition) {
    Character character = {0};
    character.texture = LoadTexture(imagePath);
    character.source = (Rectangle){
        .x = (float)(frameColumn * CHARACTER_FRAME_SIZE),
        .y = (float)(frameRow * CHARACTER_FRAME_SIZE),
        .width = CHARACTER_FRAME_SIZE,
        .height = CHARACTER_FRAME_SIZE,
    };
    character.worldPosition = startPosition;
    character.speed = 96.0f;
    character.animationFrame = 0;
    character.facingRow = 0;
    character.moving = false;

    if (character.texture.id != 0) {
        SetTextureFilter(character.texture, TEXTURE_FILTER_POINT);
    }
    return character;
}

void updateCharacter(Character *character, float deltaTime, int worldWidth, int worldHeight) {
    if (character == NULL) return;

    Vector2 direction = {0};
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) direction.x -= 1.0f;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) direction.x += 1.0f;
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) direction.y -= 1.0f;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) direction.y += 1.0f;

    character->moving = direction.x != 0.0f || direction.y != 0.0f;

    /* Sheet rows: down, left, right, up. Keep the last direction while idle. */
    if (direction.y > 0.0f) character->facingRow = 0;
    else if (direction.x < 0.0f) character->facingRow = 1;
    else if (direction.x > 0.0f) character->facingRow = 2;
    else if (direction.y < 0.0f) character->facingRow = 3;

    /* Normalize diagonal movement so it is not faster than horizontal movement. */
    if (direction.x != 0.0f && direction.y != 0.0f) {
        direction.x *= 0.70710678f;
        direction.y *= 0.70710678f;
    }

    character->worldPosition.x += direction.x * character->speed * deltaTime;
    character->worldPosition.y += direction.y * character->speed * deltaTime;

    if (character->moving) {
        character->animationTimer += deltaTime;
        while (character->animationTimer >= ANIMATION_FRAME_DURATION) {
            character->animationTimer -= ANIMATION_FRAME_DURATION;
            character->animationFrame =
                (character->animationFrame + 1) % ANIMATION_FRAME_COUNT;
        }
    } else {
        character->animationTimer = 0.0f;
        character->animationFrame = 0;
    }
    selectFrame(character);

    /* Keep the character's 16x16 world footprint on the map. */
    float maxX = (float)worldWidth - WORLD_TILE_SIZE;
    float maxY = (float)worldHeight - WORLD_TILE_SIZE;
    if (character->worldPosition.x < 0.0f) character->worldPosition.x = 0.0f;
    if (character->worldPosition.y < 0.0f) character->worldPosition.y = 0.0f;
    if (character->worldPosition.x > maxX) character->worldPosition.x = maxX;
    if (character->worldPosition.y > maxY) character->worldPosition.y = maxY;
}

void drawCharacter(const Character *character) {
    if (character == NULL || character->texture.id == 0) return;

    /* Camera2D applies the shared 4x zoom after this 16x16 world-space draw. */
    Rectangle destination = {
        .x = character->worldPosition.x,
        .y = character->worldPosition.y,
        .width = WORLD_TILE_SIZE * 2,
        .height = WORLD_TILE_SIZE * 2,
    };
    DrawTexturePro(character->texture, character->source, destination,
                   (Vector2){0}, 0.0f, WHITE);
}

void unloadCharacter(Character *character) {
    if (character != NULL && character->texture.id != 0) {
        UnloadTexture(character->texture);
        character->texture = (Texture2D){0};
    }
}
