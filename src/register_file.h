#pragma once
#include <cstdint>
#include <array>
#include <stdexcept>

class RegisterFile {
public:
    std::array<uint16_t, 8> regs = {0, 0, 0xEFFE, 0, 0, 0, 0, 0};

    uint16_t read(uint8_t idx) const {
        return regs[idx & 0x7];   //ensures index never goes out of range(3 bits)
    }

    void write(uint8_t idx, uint16_t val) {
        regs[idx & 0x7] = val;
    }

    void reset() {
        regs = {0, 0, 0xEFFE, 0, 0, 0, 0, 0};   //restore reset state
    }
};