#include "game.h"

#include "character.h"

#include <raylib.h>
#include <stdio.h>

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 640
#define WORLD_TILE_SIZE 16.0f
#define CAMERA_ZOOM 2.0f

static const char *findResource(const char *name, char *buffer, size_t bufferSize) {
    const char *prefixes[] = {"resources/", "../resources/", "client/resources/"};
    for (size_t i = 0; i < sizeof(prefixes) / sizeof(prefixes[0]); i++) {
        snprintf(buffer, bufferSize, "%s%s", prefixes[i], name);
        if (FileExists(buffer)) return buffer;
    }
    return NULL;
}

void runGame(void) {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Office Gang 2");
    if (!IsWindowReady()) return;

    char worldPath[512];
    const char *resolvedWorldPath = findResource("center_world.png", worldPath, sizeof(worldPath));
    Texture2D world = resolvedWorldPath ? LoadTexture(resolvedWorldPath) : (Texture2D){0};
    if (world.id == 0) {
        TraceLog(LOG_ERROR, "Could not find resources/center_world.png");
        CloseWindow();
        return;
    }
    SetTextureFilter(world, TEXTURE_FILTER_POINT);

    /* Character frame 0,0 is a 32x32 source cell, displayed as one 16x16 tile. */
    Vector2 startPosition = {0.0f, 0.0f};
    char characterPath[512];
    const char *resolvedCharacterPath = findResource("character.png", characterPath,
                                                       sizeof(characterPath));
    Character player = resolvedCharacterPath
        ? createCharacter(resolvedCharacterPath, 0, 0, startPosition)
        : (Character){0};
    if (player.texture.id == 0) {
        TraceLog(LOG_ERROR, "Could not find resources/character.png");
        UnloadTexture(world);
        CloseWindow();
        return;
    }

    while (!WindowShouldClose()) {
        updateCharacter(&player, GetFrameTime(), world.width, world.height);

        Camera2D camera = {0};
        camera.target = (Vector2){
            player.worldPosition.x + WORLD_TILE_SIZE * 0.5f,
            player.worldPosition.y + WORLD_TILE_SIZE * 0.5f,
        };
        camera.offset = (Vector2){GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f};
        camera.rotation = 0.0f;
        camera.zoom = CAMERA_ZOOM;

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
        DrawTexture(world, 0, 0, WHITE);
        drawCharacter(&player);
        EndMode2D();
        EndDrawing();
    }

    unloadCharacter(&player);
    UnloadTexture(world);
    CloseWindow();
}
