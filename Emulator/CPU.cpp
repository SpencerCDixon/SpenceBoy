//
// Created by Spencer Dixon on 5/6/20.
//

#include "CPU.h"
#include <SD/Assertions.h>

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
    dbg() << "a:   " << reg.a << "    f:  " << reg.f << "\n"
          << "b:   " << reg.b << "    c:  " << reg.c << "\n"
          << "d:   " << reg.d << "    e:  " << reg.e << "\n"
          << "h:   " << reg.h << "  l:  " << reg.l << "\n"
          << "sp:  " << reg.stack_ptr << "    pc: " << reg.program_counter;
}

void CPU::load_rom(const char* rom_path)
{
    if (m_verbose_logging)
        dbg() << "CPU::load_rom() loading rom into memory from path: " << rom_path;

    FILE* fp;
    fp = fopen(rom_path, "rb");
    perror_exit_if(fp == nullptr, "CPU::load_rom() Error: ");

    struct stat st;
    perror_exit_if(stat(rom_path, &st), "stat()");

    if (m_verbose_logging)
        dbg() << "size of ROM file is: " << st.st_size;

    // For now, ROM sizes should always be a specific size. If not, fail hard and fix the ROM!
    ASSERT(st.st_size == MAX_ROM_SIZE);

    m_rom = (u8*)malloc((size_t)st.st_size);
    fread(m_rom, st.st_size, 1, fp);
}

bool CPU::step()
{
    // TODO: handle interrupts?
    OpCode op_code = static_cast<OpCode>(fetch_and_inc());

    if (m_verbose_logging) {
        dbg() << "-----------------";
        print_opcode(op_code);
    }

    switch (op_code) {
    case OpCode::NoOp:
        break; // Noop, do nothing!
    case OpCode::Complement_A: // 4 cycles
        m_registers.a = ~m_registers.a;
        break;
    case OpCode::Load_A_D8:
        m_registers.a = fetch_and_inc();
        break;
    case OpCode::Load_B_D8:
        m_registers.b = fetch_and_inc();
        break;
    case OpCode::Load_C_D8:
        m_registers.c = fetch_and_inc();
        break;
    case OpCode::Load_D_D8:
        m_registers.d = fetch_and_inc();
        break;
    case OpCode::Load_E_D8:
        m_registers.e = fetch_and_inc();
        break;
    case OpCode::Load_B_B:
        m_registers.b = m_registers.b;
        break;
    case OpCode::Load_B_C:
        m_registers.b = m_registers.c;
        break;
    case OpCode::Load_B_D:
        m_registers.b = m_registers.d;
        break;
    case OpCode::Load_D_B:
        m_registers.d = m_registers.b;
        break;
    case OpCode::Load_D_C:
        m_registers.d = m_registers.c;
        break;
    case OpCode::Load_D_D:
        m_registers.d = m_registers.d;
        break;
    case OpCode::Load_H_B:
        m_registers.h = m_registers.b;
        break;
    case OpCode::Load_H_C:
        m_registers.h = m_registers.c;
        break;
    case OpCode::Load_H_D:
        m_registers.h = m_registers.d;
        break;
    case OpCode::Load_H_D8:
        m_registers.h = fetch_and_inc();
        break;
    case OpCode::Load_L_D8:
        m_registers.l = fetch_and_inc();
        break;
    case OpCode::Load_A_HL_Addr: // 8 cycles
        m_registers.a = read(get_hl());
        break;
    case OpCode::Load_A_DE_Addr: // 8 cycles
        m_registers.a = read(get_de());
        break;
    case OpCode::Load_HL_Addr_B:
        write(get_hl(), m_registers.b);
        break;
    case OpCode::Load_HL_Addr_A:
        write(get_hl(), m_registers.a);
        break;
    case OpCode::Load_BC_Addr_A:
        write(get_bc(), m_registers.a);
        break;
    case OpCode::Load_DE_Addr_A:
        write(get_de(), m_registers.a);
        break;
    case OpCode::Load_HL_Addr_C:
        write(get_hl(), m_registers.c);
        break;
    case OpCode::Load_HL_Addr_D8: // 12 cycles. Flags: - - - -
        write(get_hl(), fetch_and_inc());
        break;
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
            m_registers.program_counter = fetch_and_inc_a16();
        }
        break;
    case OpCode::Jump_A16:
        m_registers.program_counter = fetch_and_inc_a16();
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
    case OpCode::Load_SP_D16: // 12 cycles. Flags - - - -
        m_registers.stack_ptr = fetch_and_inc_a16();
        break;
    case OpCode::Inc_DE: // 8 cycles. Flags: - - - -
        inc_de();
        break;
    case OpCode::Inc_BC: // 8 cycles. Flags: - - - -
        inc_bc();
        break;
    case OpCode::Inc_HL: // 8 cycles. Flags: - - - -
        inc_hl();
        break;
    case OpCode::Inc_SP: // 8 cycles. Flags: - - - -
        inc_sp();
        break;
    case OpCode::Dec_HL: // 8 cycles. Flags: - - - -
        dec_hl();
        break;
    case OpCode::Load_Inc_HL_Addr_A: // 8 cycles. Flags: - - - -
        write(get_hl(), m_registers.a);
        inc_hl();
        break;
    case OpCode::Load_Dec_HL_Addr_A: // 8 cycles. Flags: - - - -
        write(get_hl(), m_registers.a);
        dec_hl();
        break;
    case OpCode::Halt:
//        hex_dump("WRAM", m_wram, 32, WRAM_START);
        return false;
    case OpCode::TestComplete:
        //        hex_dump("VRAM", m_vram, VRAM_SIZE, VRAM_START);
        return false;
    case OpCode::Prefix_CB: {
        PrefixOpCode prefix_op_code = static_cast<PrefixOpCode>(fetch_and_inc());
        handle_prefix_op_code(prefix_op_code);
        break;
    }
    default:
        if (is_opcode(op_code)) {
            printf("[ " RED "FATAL" RESET " ] " "Missing implementation for the following op code: ");
            print_opcode(op_code);
        } else {
            printf("missing op code: %x", (u8)op_code);
        }
        ASSERT_NOT_REACHED();
        break;
    }

    if (m_verbose_logging) {
        dbg() << "-----------------";
        dbg() << "Registers:";
        print_registers(m_registers);
        dbg() << "-----------------\n";
    }

    return true;
}
void CPU::handle_prefix_op_code(const PrefixOpCode& op_code)
{
    switch (op_code) {
    case PrefixOpCode::SLA_B:
        set_half_carry_flag(false);
        set_subtract_flag(false);
        set_carry_flag(will_carry_from_left_shift(m_registers.b));
        m_registers.b = (m_registers.b << 1);
        set_zero_flag(m_registers.b == 0);
        break;
    default:
        if (is_prefix_opcode(op_code)) {
            printf("[ " RED "FATAL" RESET " ] " "Missing implementation for the following op code: ");
            print_prefix_opcode(op_code);
        } else {
            printf("missing prefix op code: %x", (u8)op_code);
        }
        ASSERT_NOT_REACHED();
    }
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
        return m_rom[idx];
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
        m_io_registers[idx] = data;
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