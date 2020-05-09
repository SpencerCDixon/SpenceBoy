//
// Created by Spencer Dixon on 5/6/20.
//

#include "CPU.h"
#include <SD/Assertions.h>
#include <SD/Bytes.h>

#include <stdio.h>
#include <sys/stat.h>
#include <utility>

// Andreas: Should I use constexpr or DEFINE's for these sorts of things?
constexpr u16 MAX_ROM_SIZE = KB * 32;

// Flags
constexpr u8 FLAG_ZERO = 0b10000000;

// Memory Locations
constexpr u16 WRAM_START = 0xC000;
constexpr u16 WRAM_END = 0xCFFF;
// VRAM: 8000 - 9FFF
// ERAM: A000 - BFFF
// OAM:  FE00 - FE9F

/*

 ; Sample Assembly we're parsing to begin with:

Start:
	ld a, 3

loop:
	dec a
	jp nz, loop
	halt ;

 */

// TODO: Use the logging interface for these
void print_registers(const Registers& reg)
{
    dbg() << "a:   " << reg.a << "  f:  " << reg.f << "\n"
          << "b:   " << reg.b << "  c:  " << reg.c << "\n"
          << "d:   " << reg.d << "  e:  " << reg.e << "\n"
          << "h:   " << reg.h << "  l:  " << reg.l << "\n"
          << "sp:  " << reg.stack_ptr << "  pc: " << reg.program_counter;
}

void print_opcode(const OpCode& code)
{
    switch (code) {
    case OpCode::NoOp:
        dbg() << "NoOp";
        break;
    case OpCode::Load_A_D8:
        dbg() << "Load_A_D8";
        break;
    case OpCode::Load_B_D8:
        dbg() << "Load_B_D8";
        break;
    case OpCode::Load_H_D8:
        dbg() << "Load_H_D8";
        break;
    case OpCode::Load_L_D8:
        dbg() << "Load_L_D8";
        break;
    case OpCode::Load_HL_Addr_B:
        dbg() << "Load_HL_Addr_B";
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

OpCode decode(u8 byte)
{
    switch (byte) {
    case 0x00:
        return OpCode::NoOp;
    case 0x06:
        return OpCode::Load_B_D8;
    case 0x3e:
        return OpCode::Load_A_D8;
    case 0x26:
        return OpCode::Load_H_D8;
    case 0x2e:
        return OpCode::Load_L_D8;
    case 0x70:
        return OpCode::Load_HL_Addr_B;
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
    OpCode op_code = decode(next_byte);

    // TODO: Convert from switch statement to calling handlers to avoid annoying switch scoping issues.
    print_opcode(op_code);
    switch (op_code) {
    case OpCode::NoOp:
        break; // Noop, do nothing!
    case OpCode::Load_A_D8:
        m_registers.a = fetch_and_inc();
        break;
    case OpCode::Load_B_D8:
        m_registers.b = fetch_and_inc();
        break;
    case OpCode::Load_H_D8:
        m_registers.h = fetch_and_inc();
        break;
    case OpCode::Load_L_D8:
        m_registers.l = fetch_and_inc();
        break;
    case OpCode::Load_HL_Addr_B:
        u16 address_to_write;
        address_to_write = to_le_16_bit(m_registers.l, m_registers.h);
        write(address_to_write, m_registers.b);
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
            u16 address_to_jump = to_le_16_bit(fetch_and_inc(), fetch_and_inc());
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

u8 CPU::read(u16 address)
{

    if (address >= WRAM_START && address < WRAM_END) {
        u16 idx = WRAM_START - address;
        ASSERT(idx >= 0);
        return m_ram[idx];
    }

    return 0;
}

void CPU::write(u16 address, u8 data)
{
    if (address >= WRAM_START && address < WRAM_END) {
        u16 idx = address - WRAM_START;
        ASSERT(idx >= 0);
        m_ram[idx] = data;
        return;
    }

    // If we've reached here it means we're trying to write to memory that is not set up yet.
    ASSERT_NOT_REACHED();
}

// TODO: Abstract the address checking into a method which tells me two things:
// 1. which memory bank to access memory from
// 2. a normalized address into that bank
//
// -> Address { MemBank, NormalizedAddr }
// -> MemBank { WRAM, VRAM, etc. }