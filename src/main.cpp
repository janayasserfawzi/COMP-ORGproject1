#include "raylib.h"
#include "memory.h"

#include <assert.h>
#include <stdio.h>

void testMemory() {
    Memory memory;

    // Test 8-bit write/read
    memory.write8(0x0000, 0x12);
    memory.write8(0x0020, 0xAB);
    memory.write8(0xF000, 0x05);

    assert(memory.read8(0x0000) == 0x12);
    assert(memory.read8(0x0020) == 0xAB);
    assert(memory.read8(0xF000) == 0x05);

    // Test 16-bit write/read
    memory.write16(0x1000, 0xABCD);

    assert(memory.read16(0x1000) == 0xABCD);

    // Little-endian check
    assert(memory.read8(0x1000) == 0xCD);
    assert(memory.read8(0x1001) == 0xAB);

    // Manual byte write, then read as 16-bit
    memory.write8(0x2000, 0x34);
    memory.write8(0x2001, 0x12);

    assert(memory.read16(0x2000) == 0x1234);

    // Stack-area style test
    memory.write16(0xEFFC, 0xBEEF);

    assert(memory.read16(0xEFFC) == 0xBEEF);
    assert(memory.read8(0xEFFC) == 0xEF);
    assert(memory.read8(0xEFFD) == 0xBE);

    printf("[PASS] Memory read/write tests passed\n");
}

int main() {
    testMemory();

    InitWindow(320, 240, "ZX16 Simulator");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawText("ZX16 Simulator", 90, 90, 20, RAYWHITE);
        DrawText("Memory read/write test", 65, 120, 16, GRAY);
        DrawText("PASSED", 130, 145, 16, GREEN);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}