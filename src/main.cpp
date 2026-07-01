#include "raylib.h"

int main() {
    const int screenWidth = 320;
    const int screenHeight = 240;

    InitWindow(screenWidth, screenHeight, "ZX16 Simulator");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);
        DrawText("ZX16 Simulator", 90, 95, 20, RAYWHITE);
        DrawText("Empty GUI shell", 95, 125, 16, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
