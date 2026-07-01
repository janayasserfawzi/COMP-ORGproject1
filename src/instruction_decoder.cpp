#include "instruction_decoder.h"

DecodedInstruction InstructionDecoder::decode(unsigned short word) {
    DecodedInstruction instruction;

    instruction.word = word;
    instruction.opcode = extractOpcode(word);
    instruction.format = getFormat(instruction.opcode);

    return instruction;
}

unsigned char InstructionDecoder::extractOpcode(unsigned short word) {
    return word & 0x0007;       // Opcode is bits [2:0]
}

ZX16::InstructionFormat InstructionDecoder::getFormat(unsigned char opcode) {
    if (opcode == 0) {
        return ZX16::R_TYPE;
    }

    if (opcode == 1) {
        return ZX16::I_TYPE;
    }

    if (opcode == 2) {
        return ZX16::B_TYPE;
    }

    if (opcode == 3) {
        return ZX16::S_TYPE;
    }

    if (opcode == 4) {
        return ZX16::L_TYPE;
    }

    if (opcode == 5) {
        return ZX16::J_TYPE;
    }

    if (opcode == 6) {
        return ZX16::U_TYPE;
    }

    return ZX16::SYS_TYPE;
}