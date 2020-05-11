//
// Created by Spencer Dixon on 5/6/20.
//

#pragma once

#include <SD/LogStream.h>
#include <SD/Types.h>

#include <stdlib.h>

enum class OpCode: u8 {
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
    Debugger = 0xdd, // TODO: use to set a breakpoint
    TestComplete = 0xdb, // TODO: use to signal end of a test
};

OpCode decode(u8 byte);

struct Registers {
    u8 a { 0 };
    u8 f { 0 };
    u8 b { 0 };
    u8 c { 0 };
    u8 d { 0 };
    u8 e { 0 };
    u8 h { 0 };
    u8 l { 0 };
    u16 stack_ptr { 0 };
    u16 program_counter { 0 };
};

void print_registers(const Registers& reg);
void print_opcode(const OpCode* code);

class CPU {

public:
    CPU()
        : m_registers({ 0 })
    {
        m_wram = (u8*)calloc(KB * 32, sizeof(u8));
        m_vram = (u8*)calloc(KB * 16, sizeof(u8));
        m_io_registers = (u8*)calloc(112, sizeof(u8));
    }

    ~CPU()
    {
        if (m_rom)
            free(m_rom);

        free(m_wram);
        free(m_vram);
    }

    void load_rom(const char* rom_path);
    void step();

    u8 read(u16 address);
    void write(u16 address, u8 data);

    u8* v_ram() { return m_vram; }

private:
    u8 fetch_and_inc();

    // TODO(scd): change m_register.f based on should_set
    void set_zero_flag(bool should_set);
    void set_carry_flag(bool should_set);
    void set_half_carry_flag(bool should_set);
    void set_subtract_flag(bool should_set);

private:
    Registers m_registers;

    // TODO(scd): MMU / Bus which resolves pointers to the correct address bank.
    // Memory is very different from CPU and should belong in different places.
    u8* m_rom { nullptr };
    u8* m_wram { nullptr };
    u8* m_vram { nullptr };
    u8* m_io_registers { nullptr };
};