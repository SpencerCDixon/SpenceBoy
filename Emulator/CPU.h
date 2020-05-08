//
// Created by Spencer Dixon on 5/6/20.
//

#pragma once

#include <SD/LogStream.h>
#include <SD/Types.h>

enum class OpCode {
    Load_A_D8,
    Dec_A,
    Jump_NZ,
    Halt,
    Unknown,
};

OpCode from_byte(const u8& byte);

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
    }

    ~CPU()
    {
        if (m_rom)
            delete m_rom;
    }

    void load_rom(const char* rom_path);
    void step();

private:
    // next_byte reads the next byte from ROM and increments the program counter
    u8 fetch_and_inc();

private:
    Registers m_registers;
    char* m_rom { nullptr };
};