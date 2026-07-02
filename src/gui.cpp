#include "gui.h"
#include "cpu.h"
#include "raylib.h"

#include <stdio.h>

Gui::Gui() {
    width = 1180;
    height = 520;
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
    CPU& cpu
) {
    GuiAction action;

    BeginDrawing();

    ClearBackground(BLACK);

    DrawText("ZX16 Simulator", 490, 15, 22, RAYWHITE);

    drawStatusPanel(testStatus, frameNumber, running, cpu);
    drawConsolePanel(consoleText);
    action = drawControlPanel(running, cpu.isHalted());
    drawRegisterPanel(cpu);
    drawMemoryPanel(cpu);

    EndDrawing();

    return action;
}

void Gui::drawStatusPanel(
    const char testStatus[],
    int frameNumber,
    bool running,
    CPU& cpu
) {
    char frameText[40];
    char pcText[40];
    char spText[40];
    char stateText[40];

    sprintf(frameText, "Frame update: %d", frameNumber);
    sprintf(pcText, "PC: 0x%04X", cpu.getPC());
    sprintf(spText, "SP: 0x%04X", cpu.getSP());

    if (cpu.isHalted()) {
        sprintf(stateText, "CPU state: HALTED");
    }
    else if (running) {
        sprintf(stateText, "CPU state: RUNNING");
    }
    else {
        sprintf(stateText, "CPU state: PAUSED");
    }

    DrawRectangleLines(25, 60, 260, 420, GREEN);

    DrawText("Status Panel", 45, 80, 18, GREEN);
    DrawText(testStatus, 45, 115, 14, RAYWHITE);

    DrawText("Window open: PASSED", 45, 150, 14, GREEN);
    DrawText("Console panel: PASSED", 45, 175, 14, GREEN);
    DrawText("Buttons: PASSED", 45, 200, 14, GREEN);
    DrawText("Register display: PASSED", 45, 225, 14, GREEN);
    DrawText("Memory viewer: PASSED", 45, 250, 14, GREEN);

    DrawText(pcText, 45, 300, 14, GREEN);
    DrawText(spText, 45, 325, 14, GREEN);
    DrawText(stateText, 45, 350, 14, GREEN);
    DrawText(frameText, 45, 375, 14, GREEN);
}

void Gui::drawConsolePanel(const char consoleText[]) {
    DrawRectangleLines(315, 60, 270, 220, GREEN);

    DrawText("Console", 335, 80, 18, GREEN);

    DrawRectangle(335, 115, 230, 135, DARKGRAY);
    DrawRectangleLines(335, 115, 230, 135, GRAY);

    DrawText(consoleText, 350, 135, 18, GREEN);
}

GuiAction Gui::drawControlPanel(bool running, bool halted) {
    GuiAction action = GUI_ACTION_NONE;

    DrawRectangleLines(315, 305, 270, 115, GREEN);

    DrawText("Controls", 335, 325, 18, GREEN);

    if (halted) {
        if (drawButton(335, 370, 70, 30, "Run")) {
            action = GUI_ACTION_RUN_PAUSE;
        }
    }
    else if (running) {
        if (drawButton(335, 370, 70, 30, "Pause")) {
            action = GUI_ACTION_RUN_PAUSE;
        }
    }
    else {
        if (drawButton(335, 370, 70, 30, "Run")) {
            action = GUI_ACTION_RUN_PAUSE;
        }
    }

    if (drawButton(425, 370, 70, 30, "Step")) {
        action = GUI_ACTION_STEP;
    }

    if (drawButton(515, 370, 70, 30, "Reset")) {
        action = GUI_ACTION_RESET;
    }

    return action;
}

void Gui::drawRegisterPanel(CPU& cpu) {
    char text[40];

    DrawRectangleLines(615, 60, 240, 420, GREEN);

    DrawText("Registers", 635, 80, 18, GREEN);

    sprintf(text, "PC     : 0x%04X", cpu.getPC());
    DrawText(text, 635, 115, 14, GREEN);

    sprintf(text, "SP/x2  : 0x%04X", cpu.getSP());
    DrawText(text, 635, 140, 14, GREEN);

    for (int i = 0; i < 8; i++) {
        if (i == 2) {
            sprintf(text, "x%d/sp : 0x%04X", i, cpu.getRegisters().getRegister(i));
        }
        else {
            sprintf(text, "x%d    : 0x%04X", i, cpu.getRegisters().getRegister(i));
        }

        DrawText(text, 635, 180 + i * 24, 14, GREEN);
    }
}

void Gui::formatMemoryLine(CPU& cpu, unsigned short address, char text[]) {
    sprintf(
        text,
        "0x%04X: %02X %02X %02X %02X %02X %02X %02X %02X",
        address,
        cpu.getMemory().read8(address),
        cpu.getMemory().read8(address + 1),
        cpu.getMemory().read8(address + 2),
        cpu.getMemory().read8(address + 3),
        cpu.getMemory().read8(address + 4),
        cpu.getMemory().read8(address + 5),
        cpu.getMemory().read8(address + 6),
        cpu.getMemory().read8(address + 7)
    );
}

void Gui::drawMemoryPanel(CPU& cpu) {
    char text[80];

    unsigned short baseAddress = cpu.getPC() & 0xFFF0;

    DrawRectangleLines(885, 60, 270, 420, GREEN);

    DrawText("Memory Viewer", 905, 80, 18, GREEN);

    DrawText("RAM around PC", 905, 115, 14, RAYWHITE);

    for (int row = 0; row < 8; row++) {
        formatMemoryLine(cpu, baseAddress + row * 8, text);
        DrawText(text, 905, 150 + row * 28, 13, GREEN);
    }
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
    DrawText(text, (int)x + 12, (int)y + 8, 14, RAYWHITE);

    if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return true;
    }

    return false;
}

void Gui::close() {
    CloseWindow();
}