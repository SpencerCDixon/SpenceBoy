//
// Created by Spencer Dixon on 5/11/20.
//

#pragma once

#include <SD/LogStream.h>
#include <SD/Types.h>

// TODO:(scd) add cycle count to enumeration

#define ENUMERATE_OPCODES                 \
    __ENUMERATE(0x00, NoOp)               \
    __ENUMERATE(0x76, Halt)               \
    __ENUMERATE(0x3e, Load_A_D8)          \
    __ENUMERATE(0x06, Load_B_D8)          \
    __ENUMERATE(0x26, Load_H_D8)          \
    __ENUMERATE(0x2e, Load_L_D8)          \
    __ENUMERATE(0x70, Load_HL_Addr_B)     \
    __ENUMERATE(0x77, Load_HL_Addr_A)     \
    __ENUMERATE(0x36, Load_HL_Addr_D8)    \
    __ENUMERATE(0x7e, Load_A_HL_Addr)     \
    __ENUMERATE(0x1a, Load_A_DE_Addr)     \
    __ENUMERATE(0x21, Load_HL_D16)        \
    __ENUMERATE(0x11, Load_DE_D16)        \
    __ENUMERATE(0x22, Load_Inc_HL_Addr_A) \
    __ENUMERATE(0xd6, Sub_D8)             \
    __ENUMERATE(0x3d, Dec_A)              \
    __ENUMERATE(0x05, Dec_B)              \
    __ENUMERATE(0x13, Inc_DE)             \
    __ENUMERATE(0xc2, Jump_NZ)            \
    __ENUMERATE(0xc3, Jump_A16 /*, 3 */)  \
    __ENUMERATE(0xdd, Debugger)           \
    __ENUMERATE(0xdb, TestComplete)

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