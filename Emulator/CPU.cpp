//
// Created by Spencer Dixon on 5/6/20.
//

#include "CPU.h"
#include <SD/Assertions.h>
#include <SD/Bytes.h>

#include <stdio.h>
#include <sys/stat.h>
#include <utility>

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
// TODO: Use hex and decimal for faster debugging
void print_registers(const Registers& reg)
{
    dbg() << "a:   " << reg.a << "  f:  " << reg.f << "\n"
          << "b:   " << reg.b << "  c:  " << reg.c << "\n"
          << "d:   " << reg.d << "  e:  " << reg.e << "\n"
          << "h:   " << reg.h << "  l:  " << reg.l << "\n"
          << "sp:  " << reg.stack_ptr << "  pc: " << reg.program_counter;
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
    OpCode op_code = static_cast<OpCode>(fetch_and_inc());

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
        set_subtract_flag(true);
        set_half_carry_flag(will_half_carry(m_registers.a, 1));
        m_registers.a--;
        set_zero_flag(m_registers.a == 0);
        break;
    case OpCode::Dec_B: // 4 cycles. Flags: Z 1 H -
        set_subtract_flag(true);
        set_half_carry_flag(will_half_carry(m_registers.b, 1));
        m_registers.b--;
        set_zero_flag(m_registers.b == 0);
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
        inc_de = to_le_16_bit(m_registers.e, m_registers.d);
        inc_de++;
        m_registers.e = inc_de;
        m_registers.d = (inc_de >> 8);
        break;
    case OpCode::Load_Inc_HL_Addr_A: // 8 cycles. Flags: - - - -
        u16 inc_hl;
        inc_hl = to_le_16_bit(m_registers.l, m_registers.h);
        write(inc_hl, m_registers.a);
        inc_hl++;
        m_registers.l = inc_hl;
        m_registers.h = (inc_hl >> 8);
        break;
    default:
        printf("missing op code: %x", (u8)op_code);
        ASSERT_NOT_REACHED();
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
        u8 value = m_rom[address];
        dbg() << "VALUE: " << value;
        return value;
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

//
// Flag Setting
//
void CPU::set_zero_flag(bool should_set)
{
    if (should_set) {
        m_registers.f |= FLAG_ZERO;
    } else {
        m_registers.f &= ~FLAG_ZERO;
    }
}
void CPU::set_carry_flag(bool should_set)
{
    if (should_set) {
        m_registers.f |= FLAG_CARRY;
    } else {
        m_registers.f &= ~FLAG_CARRY;
    }
}
void CPU::set_half_carry_flag(bool should_set)
{
    if (should_set) {
        m_registers.f |= FLAG_HALF_CARRY;
    } else {
        m_registers.f &= ~FLAG_HALF_CARRY;
    }
}
void CPU::set_subtract_flag(bool should_set)
{
    if (should_set) {
        m_registers.f |= FLAG_SUBTRACT;
    } else {
        m_registers.f &= ~FLAG_SUBTRACT;
    }
}

// TODO: Abstract the address checking into a method which tells me two things:
// Hide all of this logic inside the MMU class which will simplify the CPU

// 1. which memory bank to access memory from
// 2. a normalized address into that bank
//
// -> Address { MemBank, NormalizedAddr }
// -> MemBank { WRAM, VRAM, etc. }