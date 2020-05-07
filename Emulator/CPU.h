//
// Created by Spencer Dixon on 5/6/20.
//

#pragma once

#include <SD/LogStream.h>
#include <SD/Types.h>

enum class OpCode {
    LoadAD8,
    Halt,
};

struct Instruction {
    OpCode op_code;
};

class CPU {

public:
    CPU(const char* rom_path)
        : m_rom_path(rom_path)
    {
        m_instructions = new Instruction[100];
    }

    ~CPU()
    {
        delete[] m_instructions;
    }

    void parse_rom();

    void print_op_codes()
    {
        for (int i = 0; i < m_instruction_count + 1; ++i) {
            print_op_code(m_instructions[i].op_code);
        }
    }

    void print_op_code(const OpCode& code)
    {
        switch (code) {
        case OpCode::LoadAD8:
            dbg() << "LoadAD8";
            break;
        case OpCode::Halt:
            dbg() << "Halt";
            break;
        }
    }

private:
    void add_instruction(const OpCode& opcode);

private:
    const char* m_rom_path;
    Instruction* m_instructions;
    u16 m_instruction_count { 0 };
};