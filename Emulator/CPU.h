//
// Created by Spencer Dixon on 5/6/20.
//

#pragma once

#include <SD/LogStream.h>
#include <SD/Types.h>

#include <stdlib.h>

enum class OpCode {
    NoOp,
    Load_A_D8,
    Load_B_D8,
    Load_H_D8,
    Load_L_D8,
    Load_HL_Addr_B,
    Load_A_HL_Addr,
    Dec_A,
    Jump_NZ,
    Halt,
    Unknown,
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
        m_ram = (char*)calloc(KB * 32, sizeof(u8));
    }

    ~CPU()
    {
        if (m_rom)
            free(m_rom);

        free(m_ram);
    }

    void load_rom(const char* rom_path);
    void step();

    u8 read(u16 address);
    void write(u16 address, u8 data);

private:
    u8 fetch_and_inc();

private:
    Registers m_registers;
    char* m_rom { nullptr };
    char* m_ram { nullptr };
};