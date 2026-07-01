#ifndef CPU_H
#define CPU_H

#include "memory.h"
#include "register_file.h"

class CPU {
public:
    CPU();

    // Set CPU back to project reset state
    void reset();

    // Read/write program counter
    unsigned short getPC();
    void setPC(unsigned short value);

    // Read/write SP by name instead of using register index 2
    unsigned short getSP();
    void setSP(unsigned short value);

    // Access CPU memory and registers without pointers
    Memory& getMemory();
    RegisterFile& getRegisters();

private:
    Memory memory;              // 64 KB RAM
    RegisterFile registers;     // 8 registers x0..x7

    unsigned short pc;          // Program counter
};

#endif