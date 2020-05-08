//
// Created by Spencer Dixon on 5/6/20.
//

#include "CPU.h"

#include <stdio.h>
#include <utility>

#define MAX_ROM_SIZE 32768

/*

 ; Sample Assembly we're parsing to begin with:

Start:
	ld a, 3

loop:
	dec a
	jp nz, loop
	halt ;

 */

void print_registers(const Registers& reg) {
    dbg() << "a: " << reg.a << " b: " << reg.b << "\n REST TODO :-)";
}

void CPU::parse_rom()
{
    dbg() << "CPU::parse_rom() parsing op codes from ROM path: " << m_rom_path;

    FILE* fp;
    fp = fopen(m_rom_path, "rb");
    if (fp == NULL) {
        perror("CPU::parse_rom() Error: ");
        return;
    }

    char buff[MAX_ROM_SIZE];
    fread(buff, 10, 1, fp);

    for (int i = 0; i < 10; ++i) {
        switch (buff[i]) {
        case '\x3e':
            add_instruction(OpCode::Load_A_D8);
            break;
        case '\x76':
            add_instruction(OpCode::Halt);
            break;
        case '\x3d':
            add_instruction(OpCode::Dec_A);
            break;
        case '\xc2': // FIXME: why does this need to be '\xc2' instead of 0xc2. Should I be using \x syntax?
            add_instruction(OpCode::Jump_NZ);
            break;
        }
        printf("%x ", buff[i]);
    }
    printf("\nfinished parsing first 10 bytes\n");
}

void CPU::add_instruction(const OpCode& opcode)
{
    // TODO: check if count > buffer size and handle properly
    Instruction in { std::move(opcode) };
    m_instructions[m_instruction_count] = in;
    m_instruction_count++;
}
