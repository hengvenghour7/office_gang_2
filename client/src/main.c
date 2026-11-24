#include <raylib.h>
#include "game.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Office Gangstar 2");
    Game game = createGame();
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();

            ClearBackground(RAYWHITE);
            game.tick(&game);

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}