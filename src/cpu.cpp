#include "cpu.h"

CPU::CPU() {
    reset();
}

void CPU::reset() {
    memory.reset();             // Clear all memory
    registers.reset();          // Clear registers and set x2/sp = 0xEFFE

    pc = 0x0020;                // Program starts at 0x0020
    registers.setRegister(2, 0xEFFE); // x2 is stack pointer
}

unsigned short CPU::getPC() {
    return pc;
}

void CPU::setPC(unsigned short value) {
    pc = value;
}

unsigned short CPU::getSP() {
    return registers.getRegister(2);  // x2 = sp
}

void CPU::setSP(unsigned short value) {
    registers.setRegister(2, value);  // x2 = sp
}

Memory& CPU::getMemory() {
    return memory;
}

RegisterFile& CPU::getRegisters() {
    return registers;
}