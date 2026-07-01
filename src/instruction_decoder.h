#ifndef INSTRUCTION_DECODER_H
#define INSTRUCTION_DECODER_H

namespace ZX16 {
    enum InstructionFormat {
        R_TYPE = 0,
        I_TYPE = 1,
        B_TYPE = 2,
        S_TYPE = 3,
        L_TYPE = 4,
        J_TYPE = 5,
        U_TYPE = 6,
        SYS_TYPE = 7
    };
}

struct DecodedInstruction {
    unsigned short word;        // Full 16-bit instruction
    unsigned char opcode;       // Bits [2:0]
    ZX16::InstructionFormat format;
};

class InstructionDecoder {
public:
    DecodedInstruction decode(unsigned short word);

private:
    unsigned char extractOpcode(unsigned short word);
    ZX16::InstructionFormat getFormat(unsigned char opcode);
};

#endif