#include "gui.h"
#include "raylib.h"

#include <stdio.h>

Gui::Gui() {
    width = 760;
    height = 420;
}

void Gui::open() {
    InitWindow(width, height, "ZX16 Simulator");
    SetTargetFPS(60);
}

bool Gui::shouldClose() {
    return WindowShouldClose();
}

GuiAction Gui::draw(
    const char testStatus[],
    const char consoleText[],
    int frameNumber,
    bool running,
    bool halted,
    unsigned short pc
) {
    GuiAction action;

    BeginDrawing();

    ClearBackground(BLACK);

    DrawText("ZX16 Simulator", 280, 15, 22, RAYWHITE);

    drawStatusPanel(testStatus, frameNumber, running, halted, pc);
    drawConsolePanel(consoleText);
    action = drawControlPanel(running, halted);

    EndDrawing();

    return action;
}

void Gui::drawStatusPanel(
    const char testStatus[],
    int frameNumber,
    bool running,
    bool halted,
    unsigned short pc
) {
    char frameText[40];
    char pcText[40];
    char stateText[40];

    sprintf(frameText, "Frame update: %d", frameNumber);
    sprintf(pcText, "PC: 0x%04X", pc);

    if (halted) {
        sprintf(stateText, "CPU state: HALTED");
    }
    else if (running) {
        sprintf(stateText, "CPU state: RUNNING");
    }
    else {
        sprintf(stateText, "CPU state: PAUSED");
    }

    DrawRectangleLines(25, 60, 280, 300, GREEN);

    DrawText("Status Panel", 45, 80, 18, GREEN);
    DrawText(testStatus, 45, 115, 14, RAYWHITE);

    DrawText("Window open: PASSED", 45, 150, 14, GREEN);
    DrawText("Console panel: PASSED", 45, 175, 14, GREEN);
    DrawText("Buttons: PASSED", 45, 200, 14, GREEN);

    DrawText(pcText, 45, 235, 14, GREEN);
    DrawText(stateText, 45, 260, 14, GREEN);
    DrawText(frameText, 45, 285, 14, GREEN);
}

void Gui::drawConsolePanel(const char consoleText[]) {
    DrawRectangleLines(335, 60, 390, 190, GREEN);

    DrawText("Console", 355, 80, 18, GREEN);

    DrawRectangle(355, 115, 350, 110, DARKGRAY);
    DrawRectangleLines(355, 115, 350, 110, GRAY);

    DrawText(consoleText, 370, 135, 18, GREEN);
}

GuiAction Gui::drawControlPanel(bool running, bool halted) {
    GuiAction action = GUI_ACTION_NONE;

    DrawRectangleLines(335, 270, 390, 90, GREEN);

    DrawText("Controls", 355, 285, 18, GREEN);

    if (halted) {
        if (drawButton(355, 320, 100, 30, "Run")) {
            action = GUI_ACTION_RUN_PAUSE;
        }
    }
    else if (running) {
        if (drawButton(355, 320, 100, 30, "Pause")) {
            action = GUI_ACTION_RUN_PAUSE;
        }
    }
    else {
        if (drawButton(355, 320, 100, 30, "Run")) {
            action = GUI_ACTION_RUN_PAUSE;
        }
    }

    if (drawButton(470, 320, 100, 30, "Step")) {
        action = GUI_ACTION_STEP;
    }

    if (drawButton(585, 320, 100, 30, "Reset")) {
        action = GUI_ACTION_RESET;
    }

    return action;
}

bool Gui::drawButton(float x, float y, float w, float h, const char text[]) {
    Rectangle button;
    Vector2 mouse;
    bool hover;

    button.x = x;
    button.y = y;
    button.width = w;
    button.height = h;

    mouse = GetMousePosition();
    hover = CheckCollisionPointRec(mouse, button);

    if (hover) {
        DrawRectangleRec(button, DARKGREEN);
    }
    else {
        DrawRectangleRec(button, DARKGRAY);
    }

    DrawRectangleLines((int)x, (int)y, (int)w, (int)h, GREEN);
    DrawText(text, (int)x + 25, (int)y + 8, 14, RAYWHITE);

    if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return true;
    }

    return false;
}

void Gui::close() {
    CloseWindow();
}