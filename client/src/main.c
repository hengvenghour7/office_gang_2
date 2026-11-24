#include <raylib.h>
#include "game.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Office Gangstar 2");
    SetTargetFPS(60);

    Game game = createGame();

    while (!WindowShouldClose())
    {
        game.tick(&game);
    }

    CloseWindow();

    return 0;
}