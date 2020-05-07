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
        if (buff[i] == 0x3e) {
            dbg() << "LD OpCode";
            add_instruction(OpCode::LoadAD8);
        } else if (buff[i] == 0x76) {
            add_instruction(OpCode::Halt);
        }
        printf("%x ", buff[i]);
    }
    printf("finished parsing first 10 bytes");
}

void CPU::add_instruction(const OpCode& opcode)
{
    // TODO: check if count > buffer size and handle properly
    Instruction in { std::move(opcode) };
    m_instructions[m_instruction_count] = in;
    m_instruction_count++;
}
