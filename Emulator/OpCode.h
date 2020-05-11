//
// Created by Spencer Dixon on 5/11/20.
//

#pragma once

#include <SD/Types.h>
#include <SD/LogStream.h>

enum class OpCode : u8 {
    // Misc
    NoOp = 0x00,
    Halt = 0x76,

    // Load
    Load_A_D8 = 0x3e,
    Load_B_D8 = 0x06,
    Load_H_D8 = 0x26,
    Load_L_D8 = 0x2e,
    Load_HL_Addr_B = 0x70,
    Load_HL_Addr_A = 0x77,
    Load_HL_Addr_D8 = 0x36,
    Load_A_HL_Addr = 0x7e,
    Load_A_DE_Addr = 0x1a,
    Load_HL_D16 = 0x21,
    Load_DE_D16 = 0x11,
    Load_Inc_HL_Addr_A = 0x22,

    // Maths
    Sub_D8 = 0xd6,
    Dec_A = 0x3d,
    Dec_B = 0x05,
    Inc_DE = 0x13,

    // Jump
    Jump_NZ = 0xc2,

    // Emulator Additions
    Debugger = 0xdd,     // TODO: use to set a breakpoint
    TestComplete = 0xdb, // TODO: use to signal end of a test
};

#define ENUMERATE_OPCODES           \
    __ENUMERATE(NoOp)               \
    __ENUMERATE(Halt)               \
    __ENUMERATE(Load_A_D8)          \
    __ENUMERATE(Load_B_D8)          \
    __ENUMERATE(Load_H_D8)          \
    __ENUMERATE(Load_L_D8)          \
    __ENUMERATE(Load_HL_Addr_B)     \
    __ENUMERATE(Load_HL_Addr_A)     \
    __ENUMERATE(Load_HL_Addr_D8)    \
    __ENUMERATE(Load_A_HL_Addr)     \
    __ENUMERATE(Load_A_DE_Addr)     \
    __ENUMERATE(Load_HL_D16)        \
    __ENUMERATE(Load_DE_D16)        \
    __ENUMERATE(Load_Inc_HL_Addr_A) \
    __ENUMERATE(Sub_D8)             \
    __ENUMERATE(Dec_A)              \
    __ENUMERATE(Dec_B)              \
    __ENUMERATE(Inc_DE)             \
    __ENUMERATE(Jump_NZ)            \
    __ENUMERATE(Debugger)           \
    __ENUMERATE(TestComplete)

inline void print_opcode(const OpCode& code)
{
    switch (code) {
#define __ENUMERATE(x) \
    case OpCode::x:    \
        dbg() << #x;   \
        break;
        ENUMERATE_OPCODES
#undef __ENUMERATE
    }
}

// TODO(scd): is_op_code() -> bool
// Can be used in the hex_dump to color the op code a different color