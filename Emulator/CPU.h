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
};

// FIXME: I really want some sort of Option<u16> for the operands
struct Instruction {
    OpCode op_code;
    u16 left_operand;
    u16 right_operand;
    // FIXME: wouldn't be necessary with Option<>
    u8 operand_count; // 0 - just op code, 1 - left operand, 2 - right operand
};

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
        case OpCode::Load_A_D8:
            dbg() << "Load_A_D8";
            break;
        case OpCode::Halt:
            dbg() << "Halt";
            break;
        case OpCode::Dec_A:
            dbg() << "Dec_A";
            break;
        case OpCode::Jump_NZ:
            dbg() << "Jump_NZ";
            break;
        }
    }

private:
    void add_instruction(const OpCode& opcode);

    // TODO:
//    void add_instruction(const Instruction& instruction);

private:
    const char* m_rom_path;
    Instruction* m_instructions;
    u16 m_instruction_count { 0 }; // FIXME: Is this needed?

    // TODO: Add registers!
};