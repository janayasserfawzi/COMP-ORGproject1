#include "raylib.h"
#include "memory.h"
#include "register_file.h"
#include "cpu.h"
#include "program_loader.h"
#include "instruction_decoder.h"

#include <assert.h>
#include <stdio.h>

void testMemory() {
    Memory memory;

    memory.write8(0x0000, 0x12);
    memory.write8(0x0020, 0xAB);
    memory.write8(0xF000, 0x05);

    assert(memory.read8(0x0000) == 0x12);
    assert(memory.read8(0x0020) == 0xAB);
    assert(memory.read8(0xF000) == 0x05);

    memory.write16(0x1000, 0xABCD);

    assert(memory.read16(0x1000) == 0xABCD);
    assert(memory.read8(0x1000) == 0xCD);
    assert(memory.read8(0x1001) == 0xAB);

    memory.write8(0x2000, 0x34);
    memory.write8(0x2001, 0x12);

    assert(memory.read16(0x2000) == 0x1234);

    memory.write16(0xEFFC, 0xBEEF);

    assert(memory.read16(0xEFFC) == 0xBEEF);
    assert(memory.read8(0xEFFC) == 0xEF);
    assert(memory.read8(0xEFFD) == 0xBE);

    printf("[PASS] Memory read/write tests passed\n");
}

void testRegisterFile() {
    RegisterFile registers;

    assert(registers.getRegister(0) == 0x0000);
    assert(registers.getRegister(1) == 0x0000);
    assert(registers.getRegister(2) == 0xEFFE);
    assert(registers.getRegister(3) == 0x0000);
    assert(registers.getRegister(4) == 0x0000);
    assert(registers.getRegister(5) == 0x0000);
    assert(registers.getRegister(6) == 0x0000);
    assert(registers.getRegister(7) == 0x0000);

    registers.setRegister(0, 0x1111);
    registers.setRegister(1, 0x2222);
    registers.setRegister(2, 0x3333);
    registers.setRegister(3, 0x4444);
    registers.setRegister(4, 0x5555);
    registers.setRegister(5, 0x6666);
    registers.setRegister(6, 0x7777);
    registers.setRegister(7, 0x8888);

    assert(registers.getRegister(0) == 0x1111);
    assert(registers.getRegister(1) == 0x2222);
    assert(registers.getRegister(2) == 0x3333);
    assert(registers.getRegister(3) == 0x4444);
    assert(registers.getRegister(4) == 0x5555);
    assert(registers.getRegister(5) == 0x6666);
    assert(registers.getRegister(6) == 0x7777);
    assert(registers.getRegister(7) == 0x8888);

    // x0 is normal in ZX16, not hardwired to zero
    registers.setRegister(0, 0xABCD);
    assert(registers.getRegister(0) == 0xABCD);

    registers.reset();

    assert(registers.getRegister(0) == 0x0000);
    assert(registers.getRegister(1) == 0x0000);
    assert(registers.getRegister(2) == 0xEFFE);
    assert(registers.getRegister(3) == 0x0000);
    assert(registers.getRegister(4) == 0x0000);
    assert(registers.getRegister(5) == 0x0000);
    assert(registers.getRegister(6) == 0x0000);
    assert(registers.getRegister(7) == 0x0000);

    printf("[PASS] Register file tests passed\n");
}

void testCPUReset() {
    CPU cpu;

    assert(cpu.getPC() == 0x0020);
    assert(cpu.getSP() == 0xEFFE);

    cpu.setPC(0x1234);
    cpu.setSP(0xABCD);

    assert(cpu.getPC() == 0x1234);
    assert(cpu.getSP() == 0xABCD);

    cpu.reset();

    assert(cpu.getPC() == 0x0020);
    assert(cpu.getSP() == 0xEFFE);

    printf("[PASS] CPU reset test passed\n");
}

void createLoaderTestFile(const char filename[]) {
    FILE* file = fopen(filename, "wb");

    assert(file != 0);

    // Fake .bin bytes for testing
    unsigned char data[6] = {
        0x12, 0x34, 0xAB, 0xCD, 0x00, 0xFF
    };

    for (int i = 0; i < 6; i++) {
        fputc(data[i], file);
    }

    fclose(file);
}

void testProgramLoader() {
    Memory memory;

    const char filename[] = "loader_test.bin";

    unsigned char expected[6] = {
        0x12, 0x34, 0xAB, 0xCD, 0x00, 0xFF
    };

    createLoaderTestFile(filename);

    int bytesLoaded = ProgramLoader::loadBin(memory, filename);

    assert(bytesLoaded == 6);

    // Program must be loaded starting at 0x0020
    for (int i = 0; i < 6; i++) {
        assert(memory.read8(0x0020 + i) == expected[i]);
    }

    // Make sure bytes before and after were not changed
    assert(memory.read8(0x001F) == 0x00);
    assert(memory.read8(0x0026) == 0x00);

    remove(filename);

    printf("[PASS] Program loader test passed\n");
}

void testFetch() {
    CPU cpu;

    // CPU starts fetching from PC = 0x0020
    assert(cpu.getPC() == 0x0020);

    // Put fake 16-bit instruction in RAM at PC
    cpu.getMemory().write16(0x0020, 0xABCD);

    // Fetch should read the instruction from RAM
    unsigned short instruction = cpu.fetch();

    assert(instruction == 0xABCD);

    // Fetch should move PC to the next instruction
    assert(cpu.getPC() == 0x0022);

    // Test another instruction using raw bytes
    cpu.getMemory().write8(0x0022, 0x34);
    cpu.getMemory().write8(0x0023, 0x12);

    instruction = cpu.fetch();

    assert(instruction == 0x1234);
    assert(cpu.getPC() == 0x0024);

    printf("[PASS] Fetch test passed\n");
}

void testSequentialPC() {
    CPU cpu;

    // Three fake 16-bit instructions in consecutive memory locations
    cpu.getMemory().write16(0x0020, 0x1111);
    cpu.getMemory().write16(0x0022, 0x2222);
    cpu.getMemory().write16(0x0024, 0x3333);

    // First step
    assert(cpu.getPC() == 0x0020);
    assert(cpu.step() == 0x1111);
    assert(cpu.getLastInstruction() == 0x1111);
    assert(cpu.getPC() == 0x0022);

    // Second step
    assert(cpu.step() == 0x2222);
    assert(cpu.getLastInstruction() == 0x2222);
    assert(cpu.getPC() == 0x0024);

    // Third step
    assert(cpu.step() == 0x3333);
    assert(cpu.getLastInstruction() == 0x3333);
    assert(cpu.getPC() == 0x0026);

    printf("[PASS] Sequential PC test passed\n");
}

void testInstructionDecoder() {
    InstructionDecoder decoder;
    DecodedInstruction decoded;

    // Opcode 000 -> R-Type
    decoded = decoder.decode(0x0000);
    assert(decoded.word == 0x0000);
    assert(decoded.opcode == 0);
    assert(decoded.format == ZX16::R_TYPE);

    // Opcode 001 -> I-Type
    decoded = decoder.decode(0x0001);
    assert(decoded.word == 0x0001);
    assert(decoded.opcode == 1);
    assert(decoded.format == ZX16::I_TYPE);

    // Opcode 010 -> B-Type
    decoded = decoder.decode(0x0002);
    assert(decoded.word == 0x0002);
    assert(decoded.opcode == 2);
    assert(decoded.format == ZX16::B_TYPE);

    // Opcode 011 -> S-Type
    decoded = decoder.decode(0x0003);
    assert(decoded.word == 0x0003);
    assert(decoded.opcode == 3);
    assert(decoded.format == ZX16::S_TYPE);

    // Opcode 100 -> L-Type
    decoded = decoder.decode(0x0004);
    assert(decoded.word == 0x0004);
    assert(decoded.opcode == 4);
    assert(decoded.format == ZX16::L_TYPE);

    // Opcode 101 -> J-Type
    decoded = decoder.decode(0x0005);
    assert(decoded.word == 0x0005);
    assert(decoded.opcode == 5);
    assert(decoded.format == ZX16::J_TYPE);

    // Opcode 110 -> U-Type
    decoded = decoder.decode(0x0006);
    assert(decoded.word == 0x0006);
    assert(decoded.opcode == 6);
    assert(decoded.format == ZX16::U_TYPE);

    // Opcode 111 -> SYS-Type
    decoded = decoder.decode(0x0007);
    assert(decoded.word == 0x0007);
    assert(decoded.opcode == 7);
    assert(decoded.format == ZX16::SYS_TYPE);

    // Check opcode extraction from a bigger machine code
    decoded = decoder.decode(0xABCD);
    assert(decoded.word == 0xABCD);
    assert(decoded.opcode == 5);
    assert(decoded.format == ZX16::J_TYPE);

    printf("[PASS] Instruction decoder test passed\n");
}

int main() {
    testMemory();
    testRegisterFile();
    testCPUReset();
    testProgramLoader();
    testFetch();
    testSequentialPC();
    testInstructionDecoder();

    InitWindow(320, 240, "ZX16 Simulator");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawText("ZX16 Simulator", 90, 15, 20, RAYWHITE);
        DrawText("Memory read/write test: PASSED", 45, 45, 16, GREEN);
        DrawText("Register file test: PASSED", 55, 70, 16, GREEN);
        DrawText("CPU reset test: PASSED", 70, 95, 16, GREEN);
        DrawText("Program loader test: PASSED", 55, 120, 16, GREEN);
        DrawText("Fetch test: PASSED", 90, 145, 16, GREEN);
        DrawText("Sequential PC test: PASSED", 65, 170, 16, GREEN);
        DrawText("Instruction decoder test: PASSED", 45, 195, 16, GREEN);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}