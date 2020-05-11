//
// Created by Spencer Dixon on 5/6/20.
//

#include "CPU.h"
#include <SD/Assertions.h>
#include <SD/Bytes.h>

#include <stdio.h>
#include <sys/stat.h>
#include <utility>

// Andreas: Should all of these go in the header file and not cpp file?
// -> Only put things in header file if it needs to be shared
constexpr u16 MAX_ROM_SIZE = KB * 32;

// Flags
constexpr u8 FLAG_ZERO = 0b10000000;
constexpr u8 FLAG_SUBTRACT = 0b01000000;
constexpr u8 FLAG_HALF_CARRY = 0b00100000;
constexpr u8 FLAG_CARRY = 0b00010000;

// Memory Locations
constexpr u16 ROM_START = 0x0000;
constexpr u16 ROM_END = 0x3FFF;
constexpr u16 WRAM_START = 0xC000;
constexpr u16 WRAM_END = 0xCFFF;
constexpr u16 VRAM_START = 0x8000;
constexpr u16 VRAM_END = 0xA000; // Isn't GBC supposed to have 16k VRAM? Look into this...
constexpr u16 IO_START = 0xFF00;
constexpr u16 IO_END = 0xFF7F;
// ERAM: A000 - BFFF
// OAM:  FE00 - FE9F
// HRAM: TBD

// TODO: Use the logging interface for these
void print_registers(const Registers& reg)
{
    dbg() << "a:   " << reg.a << "  f:  " << reg.f << "\n"
          << "b:   " << reg.b << "  c:  " << reg.c << "\n"
          << "d:   " << reg.d << "  e:  " << reg.e << "\n"
          << "h:   " << reg.h << "  l:  " << reg.l << "\n"
          << "sp:  " << reg.stack_ptr << "  pc: " << reg.program_counter;
}

//#define ENUMERATE_OPCODES \
//    __ENUMERATE(NoOp) \
//    __ENUMERATE(Load_A_D8)
//
//#define __ENUMERATE(x) \
//    case OpCode::x: \
//        dbg() << #x;
//ENUMERATE_OPCODES
//#undef __ENUMERATE
//
//enum OpCode {
//#define __ENUMERATE(x) \
//    x,
//        ENUMERATE_OPCODES
//};

// Andreas: This is super annoying. What kind of C++ magic can I do which can use the enums
// opcode name to do the print. I.E. OpCode::NoOp gets printed as OpCode::NoOp and not 0/1/2, etc.?
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
    case OpCode::Load_A_HL_Addr:
        dbg() << "Load_A_HL_Addr";
        break;
    case OpCode::Load_A_DE_Addr:
        dbg() << "Load_A_DE_Addr";
        break;
    case OpCode::Halt:
        dbg() << "Halt";
        break;
    case OpCode::Dec_A:
        dbg() << "Dec_A";
        break;
    case OpCode::Dec_B:
        dbg() << "Dec_B";
        break;
    case OpCode::Jump_NZ:
        dbg() << "Jump_NZ";
        break;
    case OpCode::Sub_D8:
        dbg() << "Sub_D8";
        break;
    case OpCode::Load_HL_Addr_A:
        dbg() << "Load_HL_Addr_A";
        break;
    case OpCode::Load_HL_Addr_D8:
        dbg() << "Load_HL_Addr_D8";
        break;
    case OpCode::Load_HL_D16:
        dbg() << "Load_HL_D16";
        break;
    case OpCode::Load_DE_D16:
        dbg() << "Load_DE_D16";
        break;
    case OpCode::Inc_DE:
        dbg() << "Inc_DE";
        break;
    case OpCode::Load_Inc_HL_Addr_A:
        dbg() << "Load_Inc_HL_Addr_A";
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
    case 0x77:
        return OpCode::Load_HL_Addr_A;
    case 0x36:
        return OpCode::Load_HL_Addr_D8;
    case 0x7e:
        return OpCode::Load_A_HL_Addr;
    case 0x1a:
        return OpCode::Load_A_DE_Addr;
    case 0x21:
        return OpCode::Load_HL_D16;
    case 0x11:
        return OpCode::Load_DE_D16;
    case 0x76:
        return OpCode::Halt;
    case 0x3d:
        return OpCode::Dec_A;
    case 0x05:
        return OpCode::Dec_B;
    case 0xc2:
        return OpCode::Jump_NZ;
    case 0xd6:
        return OpCode::Sub_D8;
    case 0x13:
        return OpCode::Inc_DE;
    case 0x22:
        return OpCode::Load_Inc_HL_Addr_A;
    }

    // Useful in development. Fail on missing op codes and then implement
    printf("missing op code: %x", byte);
    ASSERT_NOT_REACHED();
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

    m_rom = (u8*)malloc((size_t)st.st_size);
    fread(m_rom, st.st_size, 1, fp);
}

void CPU::step()
{
    // TODO: handle interrupts?
    u8 next_byte = fetch_and_inc();
    OpCode op_code = decode(next_byte);

    dbg() << "-----------------";
    print_opcode(op_code);

    // TODO: Convert from switch statement to calling handlers to avoid annoying switch scoping issues :-(...
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
    case OpCode::Load_A_HL_Addr: // 8 cycles
    {
        u16 address_to_read_hl;

        address_to_read_hl = to_le_16_bit(m_registers.l, m_registers.h);
        m_registers.a = read(address_to_read_hl);

        break;
    }
    case OpCode::Load_A_DE_Addr: // 8 cycles
        u16 address_to_read_de;
        address_to_read_de = to_le_16_bit(m_registers.e, m_registers.d);
        m_registers.a = read(address_to_read_de);
        break;
    case OpCode::Load_HL_Addr_B:
        u16 address_to_write_b;
        address_to_write_b = to_le_16_bit(m_registers.l, m_registers.h);
        write(address_to_write_b, m_registers.b);
        break;
    case OpCode::Load_HL_Addr_A:
        u16 address_to_write_a;
        address_to_write_a = to_le_16_bit(m_registers.l, m_registers.h);
        write(address_to_write_a, m_registers.a);
        break;
    case OpCode::Load_HL_Addr_D8: // 12 cycles. Flags: - - - -
        u16 address_to_write_d8;
        address_to_write_d8 = to_le_16_bit(m_registers.l, m_registers.h);
        write(address_to_write_d8, fetch_and_inc());
        break;
    case OpCode::Halt:
//        hexDump("WRAM", (const char*)m_wram, (const int)KB * 32);
        hexDump("VRAM", (const char*)m_vram, (const int)KB * 16);
        ASSERT(false);
    case OpCode::Dec_A: // 4 cycles. Flags: Z 1 H -
        // Andreas: How can I determine if bit 3 to 4 changed properly? Having a hard time wrapping my head around this one
        // TODO: Half-carry flag. See: https://stackoverflow.com/questions/57958631/game-boy-half-carry-flag-and-16-bit-instructions-especially-opcode-0xe8
        // Pretty sure this is totally wrong :-(
        if (will_half_carry(m_registers.a, 1))
            m_registers.f |= FLAG_HALF_CARRY;

        m_registers.f |= FLAG_SUBTRACT;

        m_registers.a--;

        if (m_registers.a == 0)
            m_registers.f |= FLAG_ZERO;

        // TODO(scd): Make sure to unset flag if conditions are not true
        // TODO(scd): Make helper functions to set/unset flags
        break;
    case OpCode::Dec_B: // 4 cycles. Flags: Z 1 H -
        if (will_half_carry(m_registers.b, 1))
            m_registers.f |= FLAG_HALF_CARRY;

        m_registers.f |= FLAG_SUBTRACT;
        m_registers.b--;

        if (m_registers.b == 0)
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
    case OpCode::Sub_D8:
        m_registers.a -= fetch_and_inc();
        break;
    case OpCode::Load_HL_D16: // 12 cycles. Flags - - - -
        m_registers.l = fetch_and_inc();
        m_registers.h = fetch_and_inc();
        break;
    case OpCode::Load_DE_D16: // 12 cycles. Flags - - - -
        m_registers.e = fetch_and_inc();
        m_registers.d = fetch_and_inc();
        break;
    case OpCode::Inc_DE: // 8 cycles. Flags: - - - -
        // TODO(scd): Extract this out to:
        // 1. fetch DE
        // 2. inc DE
        // 3. store DE

        // Example:
//        u16 get_de() { return to_le_16_bit(m_registers.d, m_register.e); }
        // void set_de(u16 value);

        u16 inc_de;
        inc_de = to_le_16_bit(m_registers.d, m_registers.e);
        inc_de++;
        m_registers.e = (inc_de >> 8);
        m_registers.d = inc_de;
        break;
    case OpCode::Load_Inc_HL_Addr_A: // 8 cycles. Flags: - - - -
        u16 inc_hl;
        inc_hl = to_le_16_bit(m_registers.h, m_registers.l);
        m_registers.a = read(inc_hl);
        inc_hl++;
        m_registers.l = (inc_hl >> 8);
        m_registers.h = inc_hl;
        break;
    case OpCode::Unknown:
        printf("Unknown op_code that needs to be handled %x ", next_byte);
        break;
    }

    dbg() << "-----------------";
    dbg() << "Registers:";
    print_registers(m_registers);
    dbg() << "-----------------\n";
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
        u16 idx = address - WRAM_START;
        ASSERT(idx >= 0);
        return m_wram[idx];
    } else if (address >= VRAM_START && address < VRAM_END) {
        u16 idx = address - VRAM_START;
        ASSERT(idx >= 0);
        return m_vram[idx];
    } else if (address >= ROM_START && address < ROM_END) {
        return m_rom[address];
    } else if (address >= IO_START && address < IO_END) {
        u16 idx = address - IO_START;
        return m_rom[address];
    }

    ASSERT_NOT_REACHED();
    return 0;
}

void CPU::write(u16 address, u8 data)
{
    if (address >= WRAM_START && address < WRAM_END) {
        u16 idx = address - WRAM_START;
        ASSERT(idx >= 0);
        m_wram[idx] = data;
        return;
    } else if (address >= VRAM_START && address < VRAM_END) {
        u16 idx = address - VRAM_START;
        ASSERT(idx >= 0);
        m_vram[idx] = data;
        return;
    } else if (address >= ROM_START && address < ROM_END) {
        m_rom[address] = data;
        return;
    } else if (address >= IO_START && address < IO_END) {
        u16 idx = address - IO_START;
        m_io_registers[address] = data;
        return;
    } else {
        dbg() << "bad address: " << address;
    }

    // If we've reached here it means we're trying to write to memory that is not set up yet.
    ASSERT_NOT_REACHED();
}

// TODO: Abstract the address checking into a method which tells me two things:
// Hide all of this logic inside the MMU class which will simplify the CPU

// 1. which memory bank to access memory from
// 2. a normalized address into that bank
//
// -> Address { MemBank, NormalizedAddr }
// -> MemBank { WRAM, VRAM, etc. }