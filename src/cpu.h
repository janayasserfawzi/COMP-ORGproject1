#pragma once
#include <cstdint>
#include <functional>
#include <string>
#include "system_memory.h"
#include "register_file.h"

struct CPUState {
    uint16_t regs[8]= {0, 0, 0xEFFE, 0, 0, 0, 0, 0}; //x0..x7, ALL writable, x0 is NOT zero, x2(sp) = 0xEFFE
    uint16_t pc= 0x0020;   //reset entry point
    bool halted  = false;     //excution state, set it to true to stop looping
};

//Person 2 implements these. Person 1 calls them. Person 3 stubs them in tests.
struct SimEvents {
    std::function<void(const std::string&)> on_print  = [](auto){}; //handles the output for print calls
    std::function<void()>                   on_halt   = []{};  //handles halt events
};

class CPU {
public:
    RegisterFile rf;       //8 registers
    uint16_t     pc;       //program counter
    Memory&      mem;      //reference to the shared memory object
    SimEvents&   events;   //reference to the shared events object
    bool         halted;

    //sets reset state
    CPU(Memory& memory, SimEvents& ev)
        : mem(memory), events(ev)
    {
        reset();
    }

    void reset() {
        rf.reset();    //all regs to 0x0000 and x2(sp) = 0xEFFE
        pc = 0x0020;    //starting point
        halted = false;
    }

    //read SP (x2) by name instead of index
    uint16_t sp() const { return rf.read(2); }

    //read the 16-bit instruction at PC, then advance PC by 2
    //PC is advanced BEFORE decode runs
    uint16_t fetch() {
        uint16_t word = mem.read16(pc);
        pc += 2;            //16 bits= 2 bytes
        return word;
    }

    //fetch one instruction then decode and execute it
    void step() {
        if (halted) return;
        uint16_t word = fetch();
        decode_and_execute(word);// IMPLEMENTED BY TEAMMATES
    }

private:
    // teammates fill this in(they extract bits and call rf.read/write, mem.read/write)
    void decode_and_execute(uint16_t word);
};