//
// Created by Spencer Dixon on 5/6/20.
//

#include "CPU.h"
#include <SD/Assertions.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <utility>

#define MAX_ROM_SIZE 32768
#define FLAG_ZERO 0b10000000

/*

 ; Sample Assembly we're parsing to begin with:

Start:
	ld a, 3

loop:
	dec a
	jp nz, loop
	halt ;

 */

void print_registers(const Registers& reg)
{
    dbg() << "a:  " << reg.a << "  f:  " << reg.f << "\n"
          << "b:  " << reg.b << "  c:  " << reg.c << "\n"
          << "d:  " << reg.d << "  e:  " << reg.e << "\n"
          << "h:  " << reg.h << "  l:  " << reg.l << "\n"
          << "sp: " << reg.stack_ptr << "  pc: " << reg.program_counter;
}

void print_opcode(const OpCode& code)
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
    case OpCode::Unknown:
        dbg() << "Unknown OpCode";
        break;
    }
}

OpCode from_byte(u8 byte)
{
    switch (byte) {
    case 0x3e:
        return OpCode::Load_A_D8;
    case 0x76:
        return OpCode::Halt;
    case 0x3d:
        return OpCode::Dec_A;
    case 0xc2:
        return OpCode::Jump_NZ;
    }

    return OpCode::Unknown;
}

void CPU::load_rom(const char* rom_path)
{
    dbg() << "CPU::load_rom() loading rom into memory from path: " << rom_path;

    FILE* fp;
    fp = fopen(rom_path, "rb");
    if (fp == NULL) {
        perror("CPU::load_rom() Error: ");
        return;
    }

    struct stat st;
    if (stat(rom_path, &st)) {
        perror("stat() ");
        return;
    }
    dbg() << "size of ROM file is: " << st.st_size;

    // For now, ROM sizes should always be a specific size. If not, fail hard and fix the ROM!
    ASSERT(st.st_size == MAX_ROM_SIZE);

    m_rom = (char*)malloc((size_t)st.st_size);
    fread(m_rom, st.st_size, 1, fp);
}

void CPU::step()
{
    // TODO: handle interrupts
    u8 next_byte = fetch_and_inc();
    OpCode op_code = from_byte(next_byte);

    print_opcode(op_code);
    switch (op_code) {
    case OpCode::Load_A_D8:
        m_registers.a = fetch_and_inc();
        break;
    case OpCode::Halt:
        ASSERT(false);
    case OpCode::Dec_A:
        m_registers.a--;
        if (m_registers.a == 0)
            m_registers.f |= FLAG_ZERO;
        break;
    case OpCode::Jump_NZ:
        if (m_registers.f & FLAG_ZERO) {
            m_registers.program_counter++;
            m_registers.program_counter++;
        } else {
            u16 address_to_jump;
            u8 lsb, msb;
            msb = 0;
            lsb = 0;

            lsb = fetch_and_inc();
            msb = fetch_and_inc();
            address_to_jump = msb << 8 | lsb;
            m_registers.program_counter = address_to_jump;
        }
        break;
    case OpCode::Unknown:
        printf("Unknown op_code that needs to be handled %x ", next_byte);
        break;
    }

    dbg() << "\n-----------------";
    dbg() << "Registers:";
    print_registers(m_registers);
    dbg() << "-----------------";
}

u8 CPU::fetch_and_inc()
{
    u8 next = m_rom[m_registers.program_counter];
    m_registers.program_counter++;
    return next;
}
