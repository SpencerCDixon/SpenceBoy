//
// Created by Spencer Dixon on 5/11/20.
//

#pragma once

#include <SD/LogStream.h>
#include <SD/Types.h>

// TODO:(scd) add cycle count to enumeration

#define ENUMERATE_OPCODES                    \
    __ENUMERATE(0x00, NoOp)                  \
    __ENUMERATE(0x76, Halt)                  \
    __ENUMERATE(0x2f, Complement_A /*, 4 */) \
    __ENUMERATE(0x3e, Load_A_D8)             \
    __ENUMERATE(0x06, Load_B_D8)             \
    __ENUMERATE(0x0e, Load_C_D8)             \
    __ENUMERATE(0x16, Load_D_D8)             \
    __ENUMERATE(0x1e, Load_E_D8)             \
    __ENUMERATE(0x26, Load_H_D8)             \
    __ENUMERATE(0x2e, Load_L_D8)             \
    __ENUMERATE(0x70, Load_HL_Addr_B)        \
    __ENUMERATE(0x71, Load_HL_Addr_C)        \
    __ENUMERATE(0x77, Load_HL_Addr_A)        \
    __ENUMERATE(0x02, Load_BC_Addr_A)        \
    __ENUMERATE(0x12, Load_DE_Addr_A)        \
    __ENUMERATE(0x36, Load_HL_Addr_D8)       \
    __ENUMERATE(0x7e, Load_A_HL_Addr)        \
    __ENUMERATE(0x1a, Load_A_DE_Addr)        \
    __ENUMERATE(0x21, Load_HL_D16)           \
    __ENUMERATE(0x11, Load_DE_D16)           \
    __ENUMERATE(0x40, Load_B_B)              \
    __ENUMERATE(0x41, Load_B_C)              \
    __ENUMERATE(0x42, Load_B_D)              \
    __ENUMERATE(0x50, Load_D_B)              \
    __ENUMERATE(0x51, Load_D_C)              \
    __ENUMERATE(0x52, Load_D_D)              \
    __ENUMERATE(0x60, Load_H_B)              \
    __ENUMERATE(0x61, Load_H_C)              \
    __ENUMERATE(0x62, Load_H_D)              \
    __ENUMERATE(0x22, Load_Inc_HL_Addr_A)    \
    __ENUMERATE(0x32, Load_Dec_HL_Addr_A)    \
    __ENUMERATE(0xd6, Sub_D8)                \
    __ENUMERATE(0x3d, Dec_A)                 \
    __ENUMERATE(0x05, Dec_B)                 \
    __ENUMERATE(0x2b, Dec_HL)                \
    __ENUMERATE(0x13, Inc_DE)                \
    __ENUMERATE(0xc2, Jump_NZ)               \
    __ENUMERATE(0xc3, Jump_A16 /*, 3 */)     \
    __ENUMERATE(0xdd, Debugger)              \
    __ENUMERATE(0xdb, TestComplete)

//__ENUMERATE(0x8F, ADC_A_A, 4)
//__ENUMERATE(0x88, ADC_A_B, 4)
//__ENUMERATE(0x89, ADC_A_C, 4)
//__ENUMERATE(0x8A, ADC_A_D, 4)
//__ENUMERATE(0x8B, ADC_A_E, 4)
//__ENUMERATE(0x8C, ADC_A_H, 4)
//__ENUMERATE(0x8E, ADC_A_HL, 8)
//__ENUMERATE(0x8D, ADC_A_L, 4)
//__ENUMERATE(0xCE, ADC_A_d8, 8)
//__ENUMERATE(0x87, ADD_A_A, 4)
//__ENUMERATE(0x80, ADD_A_B, 4)
//__ENUMERATE(0x81, ADD_A_C, 4)
//__ENUMERATE(0x82, ADD_A_D, 4)
//__ENUMERATE(0x83, ADD_A_E, 4)
//__ENUMERATE(0x84, ADD_A_H, 4)
//__ENUMERATE(0x86, ADD_A_HL, 8)
//__ENUMERATE(0x85, ADD_A_L, 4)
//__ENUMERATE(0xC6, ADD_A_d8, 8)
//__ENUMERATE(0x09, ADD_HL_BC, 8)
//__ENUMERATE(0x19, ADD_HL_DE, 8)
//__ENUMERATE(0x29, ADD_HL_HL, 8)
//__ENUMERATE(0x39, ADD_HL_SP, 8)
//__ENUMERATE(0xE8, ADD_SP_r8, 16)
//__ENUMERATE(0xA7, AND_A, 4)
//__ENUMERATE(0xA0, AND_B, 4)
//__ENUMERATE(0xA1, AND_C, 4)
//__ENUMERATE(0xA2, AND_D, 4)
//__ENUMERATE(0xA3, AND_E, 4)
//__ENUMERATE(0xA4, AND_H, 4)
//__ENUMERATE(0xA6, AND_HL, 8)
//__ENUMERATE(0xA5, AND_L, 4)
//__ENUMERATE(0xE6, AND_d8, 8)
//__ENUMERATE(0xDC, CALL_C_a16, 36)
//__ENUMERATE(0xD4, CALL_NC_a16, 36)
//__ENUMERATE(0xC4, CALL_NZ_a16, 36)
//__ENUMERATE(0xCC, CALL_Z_a16, 36)
//__ENUMERATE(0xCD, CALL_a16, 24)
//__ENUMERATE(0x3F, CCF, 4)
//__ENUMERATE(0x2F, CPL, 4)
//__ENUMERATE(0xBF, CP_A, 4)
//__ENUMERATE(0xB8, CP_B, 4)
//__ENUMERATE(0xB9, CP_C, 4)
//__ENUMERATE(0xBA, CP_D, 4)
//__ENUMERATE(0xBB, CP_E, 4)
//__ENUMERATE(0xBC, CP_H, 4)
//__ENUMERATE(0xBE, CP_HL, 8)
//__ENUMERATE(0xBD, CP_L, 4)
//__ENUMERATE(0xFE, CP_d8, 8)
//__ENUMERATE(0x27, DAA, 4)
//__ENUMERATE(0x3D, DEC_A, 4)
//__ENUMERATE(0x05, DEC_B, 4)
//__ENUMERATE(0x0B, DEC_BC, 8)
//__ENUMERATE(0x0D, DEC_C, 4)
//__ENUMERATE(0x15, DEC_D, 4)
//__ENUMERATE(0x1B, DEC_DE, 8)
//__ENUMERATE(0x1D, DEC_E, 4)
//__ENUMERATE(0x25, DEC_H, 4)
//__ENUMERATE(0x35, DEC_HL, 12)
//__ENUMERATE(0x2B, DEC_HL, 8)
//__ENUMERATE(0x2D, DEC_L, 4)
//__ENUMERATE(0x3B, DEC_SP, 8)
//__ENUMERATE(0xF3, DI, 4)
//__ENUMERATE(0xFB, EI, 4)
//__ENUMERATE(0x76, HALT, 4)
//__ENUMERATE(0xD3, ILLEGAL_D3, 4)
//__ENUMERATE(0xDB, ILLEGAL_DB, 4)
//__ENUMERATE(0xDD, ILLEGAL_DD, 4)
//__ENUMERATE(0xE3, ILLEGAL_E3, 4)
//__ENUMERATE(0xE4, ILLEGAL_E4, 4)
//__ENUMERATE(0xEB, ILLEGAL_EB, 4)
//__ENUMERATE(0xEC, ILLEGAL_EC, 4)
//__ENUMERATE(0xED, ILLEGAL_ED, 4)
//__ENUMERATE(0xF4, ILLEGAL_F4, 4)
//__ENUMERATE(0xFC, ILLEGAL_FC, 4)
//__ENUMERATE(0xFD, ILLEGAL_FD, 4)
//__ENUMERATE(0x3C, INC_A, 4)
//__ENUMERATE(0x04, INC_B, 4)
//__ENUMERATE(0x03, INC_BC, 8)
//__ENUMERATE(0x0C, INC_C, 4)
//__ENUMERATE(0x14, INC_D, 4)
//__ENUMERATE(0x13, INC_DE, 8)
//__ENUMERATE(0x1C, INC_E, 4)
//__ENUMERATE(0x24, INC_H, 4)
//__ENUMERATE(0x23, INC_HL, 8)
//__ENUMERATE(0x34, INC_HL, 12)
//__ENUMERATE(0x2C, INC_L, 4)
//__ENUMERATE(0x33, INC_SP, 8)
//__ENUMERATE(0xDA, JP_C_a16, 28)
//__ENUMERATE(0xE9, JP_HL, 4)
//__ENUMERATE(0xD2, JP_NC_a16, 28)
//__ENUMERATE(0xC2, JP_NZ_a16, 28)
//__ENUMERATE(0xCA, JP_Z_a16, 28)
//__ENUMERATE(0xC3, JP_a16, 16)
//__ENUMERATE(0x38, JR_C_r8, 20)
//__ENUMERATE(0x30, JR_NC_r8, 20)
//__ENUMERATE(0x20, JR_NZ_r8, 20)
//__ENUMERATE(0x28, JR_Z_r8, 20)
//__ENUMERATE(0x18, JR_r8, 12)
//__ENUMERATE(0xF0, LDH_A_a8, 12)
//__ENUMERATE(0xE0, LDH_a8_A, 12)
//__ENUMERATE(0x7F, LD_A_A, 4)
//__ENUMERATE(0x78, LD_A_B, 4)
//__ENUMERATE(0x0A, LD_A_BC, 8)
//__ENUMERATE(0x79, LD_A_C, 4)
//__ENUMERATE(0xF2, LD_A_C, 8)
//__ENUMERATE(0x7A, LD_A_D, 4)
//__ENUMERATE(0x1A, LD_A_DE, 8)
//__ENUMERATE(0x7B, LD_A_E, 4)
//__ENUMERATE(0x7C, LD_A_H, 4)
//__ENUMERATE(0x7E, LD_A_HL, 8)
//__ENUMERATE(0x2A, LD_A_HL, 8)
//__ENUMERATE(0x3A, LD_A_HL, 8)
//__ENUMERATE(0x7D, LD_A_L, 4)
//__ENUMERATE(0xFA, LD_A_a16, 16)
//__ENUMERATE(0x3E, LD_A_d8, 8)
//__ENUMERATE(0x02, LD_BC_A, 8)
//__ENUMERATE(0x01, LD_BC_d16, 12)
//__ENUMERATE(0x47, LD_B_A, 4)
//__ENUMERATE(0x40, LD_B_B, 4)
//__ENUMERATE(0x41, LD_B_C, 4)
//__ENUMERATE(0x42, LD_B_D, 4)
//__ENUMERATE(0x43, LD_B_E, 4)
//__ENUMERATE(0x44, LD_B_H, 4)
//__ENUMERATE(0x46, LD_B_HL, 8)
//__ENUMERATE(0x45, LD_B_L, 4)
//__ENUMERATE(0x06, LD_B_d8, 8)
//__ENUMERATE(0xE2, LD_C_A, 8)
//__ENUMERATE(0x4F, LD_C_A, 4)
//__ENUMERATE(0x48, LD_C_B, 4)
//__ENUMERATE(0x49, LD_C_C, 4)
//__ENUMERATE(0x4A, LD_C_D, 4)
//__ENUMERATE(0x4B, LD_C_E, 4)
//__ENUMERATE(0x4C, LD_C_H, 4)
//__ENUMERATE(0x4E, LD_C_HL, 8)
//__ENUMERATE(0x4D, LD_C_L, 4)
//__ENUMERATE(0x0E, LD_C_d8, 8)
//__ENUMERATE(0x12, LD_DE_A, 8)
//__ENUMERATE(0x11, LD_DE_d16, 12)
//__ENUMERATE(0x57, LD_D_A, 4)
//__ENUMERATE(0x50, LD_D_B, 4)
//__ENUMERATE(0x51, LD_D_C, 4)
//__ENUMERATE(0x52, LD_D_D, 4)
//__ENUMERATE(0x53, LD_D_E, 4)
//__ENUMERATE(0x54, LD_D_H, 4)
//__ENUMERATE(0x56, LD_D_HL, 8)
//__ENUMERATE(0x55, LD_D_L, 4)
//__ENUMERATE(0x16, LD_D_d8, 8)
//__ENUMERATE(0x5F, LD_E_A, 4)
//__ENUMERATE(0x58, LD_E_B, 4)
//__ENUMERATE(0x59, LD_E_C, 4)
//__ENUMERATE(0x5A, LD_E_D, 4)
//__ENUMERATE(0x5B, LD_E_E, 4)
//__ENUMERATE(0x5C, LD_E_H, 4)
//__ENUMERATE(0x5E, LD_E_HL, 8)
//__ENUMERATE(0x5D, LD_E_L, 4)
//__ENUMERATE(0x1E, LD_E_d8, 8)
//__ENUMERATE(0x22, LD_HL_A, 8)
//__ENUMERATE(0x32, LD_HL_A, 8)
//__ENUMERATE(0x77, LD_HL_A, 8)
//__ENUMERATE(0x70, LD_HL_B, 8)
//__ENUMERATE(0x71, LD_HL_C, 8)
//__ENUMERATE(0x72, LD_HL_D, 8)
//__ENUMERATE(0x73, LD_HL_E, 8)
//__ENUMERATE(0x74, LD_HL_H, 8)
//__ENUMERATE(0x75, LD_HL_L, 8)
//__ENUMERATE(0xF8, LD_HL_SP, 12)
//__ENUMERATE(0x21, LD_HL_d16, 12)
//__ENUMERATE(0x36, LD_HL_d8, 12)
//__ENUMERATE(0x67, LD_H_A, 4)
//__ENUMERATE(0x60, LD_H_B, 4)
//__ENUMERATE(0x61, LD_H_C, 4)
//__ENUMERATE(0x62, LD_H_D, 4)
//__ENUMERATE(0x63, LD_H_E, 4)
//__ENUMERATE(0x64, LD_H_H, 4)
//__ENUMERATE(0x66, LD_H_HL, 8)
//__ENUMERATE(0x65, LD_H_L, 4)
//__ENUMERATE(0x26, LD_H_d8, 8)
//__ENUMERATE(0x6F, LD_L_A, 4)
//__ENUMERATE(0x68, LD_L_B, 4)
//__ENUMERATE(0x69, LD_L_C, 4)
//__ENUMERATE(0x6A, LD_L_D, 4)
//__ENUMERATE(0x6B, LD_L_E, 4)
//__ENUMERATE(0x6C, LD_L_H, 4)
//__ENUMERATE(0x6E, LD_L_HL, 8)
//__ENUMERATE(0x6D, LD_L_L, 4)
//__ENUMERATE(0x2E, LD_L_d8, 8)
//__ENUMERATE(0xF9, LD_SP_HL, 8)
//__ENUMERATE(0x31, LD_SP_d16, 12)
//__ENUMERATE(0xEA, LD_a16_A, 16)
//__ENUMERATE(0x08, LD_a16_SP, 20)
//__ENUMERATE(0x00, NOP, 4)
//__ENUMERATE(0xB7, OR_A, 4)
//__ENUMERATE(0xB0, OR_B, 4)
//__ENUMERATE(0xB1, OR_C, 4)
//__ENUMERATE(0xB2, OR_D, 4)
//__ENUMERATE(0xB3, OR_E, 4)
//__ENUMERATE(0xB4, OR_H, 4)
//__ENUMERATE(0xB6, OR_HL, 8)
//__ENUMERATE(0xB5, OR_L, 4)
//__ENUMERATE(0xF6, OR_d8, 8)
//__ENUMERATE(0xF1, POP_AF, 12)
//__ENUMERATE(0xC1, POP_BC, 12)
//__ENUMERATE(0xD1, POP_DE, 12)
//__ENUMERATE(0xE1, POP_HL, 12)
//__ENUMERATE(0xCB, PREFIX, 4)
//__ENUMERATE(0xF5, PUSH_AF, 16)
//__ENUMERATE(0xC5, PUSH_BC, 16)
//__ENUMERATE(0xD5, PUSH_DE, 16)
//__ENUMERATE(0xE5, PUSH_HL, 16)
//__ENUMERATE(0xC9, RET, 16)
//__ENUMERATE(0xD9, RETI, 16)
//__ENUMERATE(0xD8, RET_C, 28)
//__ENUMERATE(0xD0, RET_NC, 28)
//__ENUMERATE(0xC0, RET_NZ, 28)
//__ENUMERATE(0xC8, RET_Z, 28)
//__ENUMERATE(0x17, RLA, 4)
//__ENUMERATE(0x07, RLCA, 4)
//__ENUMERATE(0x1F, RRA, 4)
//__ENUMERATE(0x0F, RRCA, 4)
//__ENUMERATE(0xC7, RST_00H, 16)
//__ENUMERATE(0xCF, RST_08H, 16)
//__ENUMERATE(0xD7, RST_10H, 16)
//__ENUMERATE(0xDF, RST_18H, 16)
//__ENUMERATE(0xE7, RST_20H, 16)
//__ENUMERATE(0xEF, RST_28H, 16)
//__ENUMERATE(0xF7, RST_30H, 16)
//__ENUMERATE(0xFF, RST_38H, 16)
//__ENUMERATE(0x9F, SBC_A_A, 4)
//__ENUMERATE(0x98, SBC_A_B, 4)
//__ENUMERATE(0x99, SBC_A_C, 4)
//__ENUMERATE(0x9A, SBC_A_D, 4)
//__ENUMERATE(0x9B, SBC_A_E, 4)
//__ENUMERATE(0x9C, SBC_A_H, 4)
//__ENUMERATE(0x9E, SBC_A_HL, 8)
//__ENUMERATE(0x9D, SBC_A_L, 4)
//__ENUMERATE(0xDE, SBC_A_d8, 8)
//__ENUMERATE(0x37, SCF, 4)
//__ENUMERATE(0x10, STOP, 4)
//__ENUMERATE(0x97, SUB_A, 4)
//__ENUMERATE(0x90, SUB_B, 4)
//__ENUMERATE(0x91, SUB_C, 4)
//__ENUMERATE(0x92, SUB_D, 4)
//__ENUMERATE(0x93, SUB_E, 4)
//__ENUMERATE(0x94, SUB_H, 4)
//__ENUMERATE(0x96, SUB_HL, 8)
//__ENUMERATE(0x95, SUB_L, 4)
//__ENUMERATE(0xD6, SUB_d8, 8)
//__ENUMERATE(0xAF, XOR_A, 4)
//__ENUMERATE(0xA8, XOR_B, 4)
//__ENUMERATE(0xA9, XOR_C, 4)
//__ENUMERATE(0xAA, XOR_D, 4)
//__ENUMERATE(0xAB, XOR_E, 4)
//__ENUMERATE(0xAC, XOR_H, 4)
//__ENUMERATE(0xAE, XOR_HL, 8)
//__ENUMERATE(0xAD, XOR_L, 4)
//__ENUMERATE(0xEE, XOR_d8, 8)

enum class OpCode : u8 {
#define __ENUMERATE(hex, name) name = hex,
    ENUMERATE_OPCODES
#undef __ENUMERATE
};

inline void print_opcode(const OpCode& code)
{
    switch (code) {
#define __ENUMERATE(_, name) \
    case OpCode::name:       \
        dbg() << #name;      \
        break;
        ENUMERATE_OPCODES
#undef __ENUMERATE
    }
}

inline bool is_opcode(const OpCode& code)
{
    switch (code) {
#define __ENUMERATE(_, name) \
    case OpCode::name:       \
        return true;
        ENUMERATE_OPCODES
#undef __ENUMERATE
    }
    return false;
}

// TODO(scd): is_op_code() -> bool
// Can be used in the hex_dump to color the op code a different color