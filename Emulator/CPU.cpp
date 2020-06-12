//
// Created by Spencer Dixon on 5/6/20.
//

#include "CPU.h"
#include "Emulator.h"
#include "IODevice.h"
#include "MemoryMap.h"

#include <SD/Assertions.h>
#include <SD/File.h>

#include <inttypes.h>
#include <stdio.h>

// Useful to snag when debugging
//        hex_dump("WRAM", m_wram, WRAM_SIZE, WRAM_START);
//        hex_dump("VRAM", m_vram, VRAM_SIZE, VRAM_START);

// Flags
// clang-format off
constexpr u8 FLAG_ZERO       = 0b10000000;
constexpr u8 FLAG_SUBTRACT   = 0b01000000;
constexpr u8 FLAG_HALF_CARRY = 0b00100000;
constexpr u8 FLAG_CARRY      = 0b00010000;
// clang-format on

CPU::CPU(Emulator& emulator, bool verbose_logging)
    : m_emulator(emulator)
    , m_verbose_logging(verbose_logging)
    , m_registers({ 0 })
{
    // TODO: Some boot process that I can define however I like. This could be done in SDL
    // land and doesn't need to use normal GB rendering techniques. It must result in the program
    // counter being set at the proper location of 0x100.
    m_registers.stack_ptr = 0xfffe;
    m_registers.program_counter = 0x100;
}

CPU::~CPU()
{
}

StepResult CPU::step()
{
    // TODO: handle interrupts?
    StepResult result;
    OpCode op_code = static_cast<OpCode>(fetch_and_inc_8bit());

    switch (op_code) {
    case OpCode::NOP:
        break;
    case OpCode::CPL:
        m_registers.a = ~m_registers.a;
        break;
    case OpCode::LD_A_B:
        m_registers.a = m_registers.b;
        break;
    case OpCode::LD_A_E:
        m_registers.a = m_registers.e;
        break;
    case OpCode::LD_A_d8:
        m_registers.a = fetch_and_inc_8bit();
        break;
    case OpCode::LD_B_d8:
        m_registers.b = fetch_and_inc_8bit();
        break;
    case OpCode::LD_C_d8:
        m_registers.c = fetch_and_inc_8bit();
        break;
    case OpCode::LD_D_d8:
        m_registers.d = fetch_and_inc_8bit();
        break;
    case OpCode::LD_E_d8:
        m_registers.e = fetch_and_inc_8bit();
        break;
    case OpCode::LD_B_A:
        m_registers.b = m_registers.a;
        break;
    case OpCode::LD_B_B:
        m_registers.b = m_registers.b;
        break;
    case OpCode::LD_B_C:
        m_registers.b = m_registers.c;
        break;
    case OpCode::LD_B_D:
        m_registers.b = m_registers.d;
        break;
    case OpCode::LD_D_B:
        m_registers.d = m_registers.b;
        break;
    case OpCode::LD_D_C:
        m_registers.d = m_registers.c;
        break;
    case OpCode::LD_D_D:
        m_registers.d = m_registers.d;
        break;
    case OpCode::LD_E_A:
        m_registers.e = m_registers.a;
        break;
    case OpCode::LD_H_B:
        m_registers.h = m_registers.b;
        break;
    case OpCode::LD_H_C:
        m_registers.h = m_registers.c;
        break;
    case OpCode::LD_H_D:
        m_registers.h = m_registers.d;
        break;
    case OpCode::LD_H_d8:
        m_registers.h = fetch_and_inc_8bit();
        break;
    case OpCode::LD_L_d8:
        m_registers.l = fetch_and_inc_8bit();
        break;
    case OpCode::LD_A_HL_ADDR_INC:
        m_registers.a = read(get_hl());
        inc_hl();
        break;
    case OpCode::LD_A_HL_ADDR:
        m_registers.a = read(get_hl());
        break;
    case OpCode::LD_A_DE_ADDR:
        m_registers.a = read(get_de());
        break;
    case OpCode::LD_HL_ADDR_B:
        write(get_hl(), m_registers.b);
        break;
    case OpCode::LD_HL_ADDR_A:
        write(get_hl(), m_registers.a);
        break;
    case OpCode::LD_BC_ADDR_A:
        write(get_bc(), m_registers.a);
        break;
    case OpCode::LD_DE_ADDR_A:
        write(get_de(), m_registers.a);
        break;
    case OpCode::LD_HL_ADDR_C:
        write(get_hl(), m_registers.c);
        break;
    case OpCode::LD_HL_ADDR_d8:
        write(get_hl(), fetch_and_inc_8bit());
        break;
    case OpCode::LDH_a8_ADDR_A: {
        u16 address = IO_START + fetch_and_inc_8bit();
        write(address, m_registers.a);
        break;
    }
    case OpCode::LDH_A_a8_ADDR: {
        u16 address = IO_START + fetch_and_inc_8bit();
        m_registers.a = read(address);
        break;
    }
    case OpCode::DEC_A:
        dec_reg(&m_registers.a);
        break;
    case OpCode::DEC_B:
        dec_reg(&m_registers.b);
        break;
    case OpCode::DEC_C:
        dec_reg(&m_registers.c);
        break;
    case OpCode::DEC_D:
        dec_reg(&m_registers.d);
        break;
    case OpCode::DEC_E:
        dec_reg(&m_registers.e);
        break;
    case OpCode::DEC_H:
        dec_reg(&m_registers.h);
        break;
    case OpCode::DEC_L:
        dec_reg(&m_registers.l);
        break;
    case OpCode::DEC_HL:
        dec_hl();
        break;
    case OpCode::DEC_SP:
        dec_sp();
        break;
    case OpCode::DEC_BC:
        dec_bc();
        break;
    case OpCode::DEC_DE:
        dec_de();
        break;
    case OpCode::JP_NZ_a16: {
        u16 new_address = fetch_and_inc_16bit();
        if (!get_zero_flag())
            m_registers.program_counter = new_address;
        break;
    }
    case OpCode::JP_Z_a16: {
        u16 new_address = fetch_and_inc_16bit();
        if (get_zero_flag())
            m_registers.program_counter = new_address;
        break;
    }
    case OpCode::JP_a16:
        m_registers.program_counter = fetch_and_inc_16bit();
        break;
    case OpCode::JR_r8:
        m_registers.program_counter += static_cast<s8>(fetch_and_inc_8bit());
        break;
    case OpCode::JR_NZ_r8: {
        s8 offset = static_cast<s8>(fetch_and_inc_8bit());
        if (!get_zero_flag())
            m_registers.program_counter += offset;
        break;
    }
    case OpCode::SUB_d8:
        m_registers.a -= fetch_and_inc_8bit();
        break;
    case OpCode::LD_HL_d16:
        m_registers.l = fetch_and_inc_8bit();
        m_registers.h = fetch_and_inc_8bit();
        break;
    case OpCode::LD_DE_d16:
        m_registers.e = fetch_and_inc_8bit();
        m_registers.d = fetch_and_inc_8bit();
        break;
    case OpCode::LD_BC_d16:
        m_registers.c = fetch_and_inc_8bit();
        m_registers.b = fetch_and_inc_8bit();
        break;
    case OpCode::LD_SP_d16:
        m_registers.stack_ptr = fetch_and_inc_16bit();
        break;
    case OpCode::INC_DE:
        inc_de();
        break;
    case OpCode::INC_BC:
        inc_bc();
        break;
    case OpCode::INC_HL:
        inc_hl();
        break;
    case OpCode::INC_SP:
        inc_sp();
        break;
    case OpCode::INC_A:
        inc_reg(&m_registers.a);
        break;
    case OpCode::INC_B:
        inc_reg(&m_registers.b);
        break;
    case OpCode::INC_C:
        inc_reg(&m_registers.c);
        break;
    case OpCode::INC_D:
        inc_reg(&m_registers.d);
        break;
    case OpCode::INC_E:
        inc_reg(&m_registers.e);
        break;
    case OpCode::INC_H:
        inc_reg(&m_registers.h);
        break;
    case OpCode::INC_L:
        inc_reg(&m_registers.l);
        break;
    case OpCode::LD_HL_ADDR_INC_A:
        write(get_hl(), m_registers.a);
        inc_hl();
        break;
    case OpCode::LD_HL_ADDR_DEC_A:
        write(get_hl(), m_registers.a);
        dec_hl();
        break;
    case OpCode::AND_d8:
        and_with_a(fetch_and_inc_8bit());
        break;
    case OpCode::OR_B:
        or_with_a(m_registers.b);
        break;
    case OpCode::XOR_A:
        xor_reg(&m_registers.a);
        break;
    case OpCode::XOR_B:
        xor_reg(&m_registers.b);
        break;
    case OpCode::XOR_C:
        xor_reg(&m_registers.c);
        break;
    case OpCode::XOR_D:
        xor_reg(&m_registers.d);
        break;
    case OpCode::XOR_E:
        xor_reg(&m_registers.e);
        break;
    case OpCode::XOR_H:
        xor_reg(&m_registers.h);
        break;
    case OpCode::XOR_L:
        xor_reg(&m_registers.l);
        break;
    case OpCode::DI:
        m_interrupts_enabled = false;
        break;
    case OpCode::EI:
        m_interrupts_enabled = true;
        break;
    case OpCode::PUSH_AF:
        push(&m_registers.a, &m_registers.f);
        break;
    case OpCode::PUSH_BC:
        push(&m_registers.b, &m_registers.c);
        break;
    case OpCode::PUSH_DE:
        push(&m_registers.d, &m_registers.e);
        break;
    case OpCode::PUSH_HL:
        push(&m_registers.h, &m_registers.l);
        break;
    case OpCode::POP_AF:
        pop(&m_registers.a, &m_registers.f);
        break;
    case OpCode::POP_BC:
        pop(&m_registers.b, &m_registers.c);
        break;
    case OpCode::POP_DE:
        pop(&m_registers.d, &m_registers.e);
        break;
    case OpCode::POP_HL:
        pop(&m_registers.h, &m_registers.l);
        break;
    case OpCode::CALL_a16:
        push(m_registers.program_counter + 2);
        m_registers.program_counter = fetch_and_inc_16bit();
        break;
    case OpCode::RET:
        pop_return();
        break;
    case OpCode::TEST_COMPLETE:
    case OpCode::HALT:
        //        hex_dump("WRAM", m_wram, 5200, WRAM_START);
        //        hex_dump("VRAM", m_vram, VRAM_SIZE, VRAM_START);
        result.should_halt = true;
        break;
    case OpCode::PREFIX: {
        PrefixOpCode prefix_op_code = static_cast<PrefixOpCode>(fetch_and_inc_8bit());
        handle_prefix_op_code(prefix_op_code);
        result.cycles += cycles_for_prefix_opcode(prefix_op_code);
        break;
    }
    default:
        if (is_opcode(op_code)) {
            printf("[ " RED "FATAL" RESET " ] "
                   "Missing implementation for the following op code: ");
            print_opcode(op_code);
        } else {
            printf("missing op code: %x", (u8)op_code);
        }
        ASSERT_NOT_REACHED();
        break;
    }

    if (m_verbose_logging) {
        dbg() << to_string(op_code) << "   " << *this;
    }

    result.cycles += cycles_for_opcode(op_code);
    return result;
}

void CPU::handle_prefix_op_code(const PrefixOpCode& op_code)
{
    switch (op_code) {
    case PrefixOpCode::SLA_A:
        shift_left(&m_registers.a);
        break;
    case PrefixOpCode::SLA_B:
        shift_left(&m_registers.b);
        break;
    case PrefixOpCode::SLA_C:
        shift_left(&m_registers.c);
        break;
    case PrefixOpCode::SLA_D:
        shift_left(&m_registers.d);
        break;
    case PrefixOpCode::SLA_E:
        shift_left(&m_registers.e);
        break;
    case PrefixOpCode::SLA_H:
        shift_left(&m_registers.h);
        break;
    case PrefixOpCode::SLA_L:
        shift_left(&m_registers.l);
        break;
    case PrefixOpCode::SWAP_A:
        swap_reg(&m_registers.a);
        break;
    default:
        if (is_prefix_opcode(op_code)) {
            printf("[ " RED "FATAL" RESET " ] "
                   "Missing implementation for the following prefix op code: ");
            print_prefix_opcode(op_code);
        } else {
            printf("missing prefix op code: %x", (u8)op_code);
        }
        ASSERT_NOT_REACHED();
    }
}

//
// Memory Access
//

u8 CPU::fetch_and_inc_8bit()
{
    u8 next = emulator().mmu().rom()[m_registers.program_counter];
    m_registers.program_counter++;
    return next;
}

u16 CPU::fetch_and_inc_16bit()
{
    u8 b1 = fetch_and_inc_8bit();
    u8 b2 = fetch_and_inc_8bit();
    return to_le_16_bit(b1, b2);
}

u8 CPU::read(u16 address)
{
    return emulator().mmu().read(address);
}

void CPU::write(u16 address, u8 data)
{
    return emulator().mmu().write(address, data);
}

u8 CPU::in(u16 address)
{
    dbg() << "CPU::in(" << to_hex(address) << ")";
    return 0;
}

void CPU::out(u16 address, u8 value)
{
    dbg() << "CPU::out(" << to_hex(address) << ", " << to_hex(value) << ")";
}

void CPU::push(u8* reg_one, u8* reg_two)
{
    dec_sp();
    write(get_sp(), *reg_one);
    dec_sp();
    write(get_sp(), *reg_two);
}

void CPU::push(u16 addr)
{
    u8 byte_one = addr;
    u8 byte_two = addr >> 8;

    dec_sp();
    write(get_sp(), byte_two);
    dec_sp();
    write(get_sp(), byte_one);
}

void CPU::pop(u8* reg_one, u8* reg_two)
{
    // NOTE: Registers are reversed due to little endian storage.
    // We want the API to remain easy to read. I.E. POP_DE should be called like pop(&d, &e);
    *reg_two = read(get_sp());
    inc_sp();
    *reg_one = read(get_sp());
    inc_sp();
}

void CPU::pop_return()
{
    u8 b1 = read(get_sp());
    inc_sp();
    u8 b2 = read(get_sp());
    inc_sp();
    m_registers.program_counter = to_le_16_bit(b1, b2);
}

//
// Bit Twiddling
//

void CPU::shift_left(u8* reg_ptr)
{
    set_half_carry_flag(false);
    set_subtract_flag(false);
    set_carry_flag(will_carry_from_left_shift(*reg_ptr));
    *reg_ptr = (*reg_ptr << 1);
    set_zero_flag(*reg_ptr == 0);
}

void CPU::xor_reg(u8* reg_ptr)
{
    *reg_ptr ^= m_registers.a;
    set_zero_flag(*reg_ptr == 0);
}

void CPU::and_with_a(u8 value)
{
    // FIXME: Implement half carry
    //    set_half_carry_flag(will_half_carry(*reg_ptr, result));
    m_registers.a &= value;
    set_zero_flag(m_registers.a == 0);
    set_carry_flag(false);
    set_subtract_flag(false);
}

void CPU::swap_reg(u8* reg_ptr)
{
    *reg_ptr = swap_nibbles(*reg_ptr);
    set_subtract_flag(false);
    set_half_carry_flag(false);
    set_carry_flag(false);
    set_zero_flag(*reg_ptr == 0);
}

void CPU::or_with_a(u8 value)
{
    m_registers.a |= value;
    set_subtract_flag(false);
    set_half_carry_flag(false);
    set_carry_flag(false);
    set_zero_flag(m_registers.a == 0);
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

bool CPU::get_zero_flag()
{
    return m_registers.f & FLAG_ZERO;
}

// TODO: Abstract the address checking into a method which tells me two things:
// Hide all of this logic inside the MMU class which will simplify the CPU

// 1. which memory bank to access memory from
// 2. a normalized address into that bank
//
// -> Address { MemBank, NormalizedAddr }
// -> MemBank { WRAM, VRAM, etc. }

const LogStream& operator<<(const LogStream& stream, const CPUTestState& test_state)
{
    stream << to_trace_line(test_state);
    return stream;
}
const LogStream& operator<<(const LogStream& stream, CPU& cpu)
{
    stream << to_trace_line(cpu.test_state());
    return stream;
}

//
// Testing Utilities
//

String to_trace_line(const CPUTestState& test_state)
{
    local_persist u16 buf_size = 512;
    char* buffer = (char*)calloc(buf_size, sizeof(char));
    snprintf(
        buffer,
        buf_size,
        "A: %03u [0x%02x] F: %03u [0x%02x]  "
        "B: %03u [0x%02x] C: %03u [0x%02x]  "
        "D: %03u [0x%02x] E: %03u [0x%02x]  "
        "H: %03u [0x%02x] L: %03u [0x%02x] ---"
        " WRAM: 0x%016" PRIx64 "| VRAM: 0x%016" PRIx64 " |",
        test_state.registers.a,
        test_state.registers.a,
        test_state.registers.f,
        test_state.registers.f,
        test_state.registers.b,
        test_state.registers.b,
        test_state.registers.c,
        test_state.registers.c,
        test_state.registers.d,
        test_state.registers.d,
        test_state.registers.e,
        test_state.registers.e,
        test_state.registers.h,
        test_state.registers.h,
        test_state.registers.l,
        test_state.registers.l,
        test_state.wram_checksum,
        test_state.vram_checksum);
    auto str = String(buffer);
    free(buffer);
    return str;
}

String to_snapshot(const CPUTestState& state)
{
    local_persist u16 buf_size = 512;
    char* buffer = (char*)calloc(buf_size, sizeof(char));
    snprintf(
        buffer,
        buf_size,
        "Registers:\n"
        "–---------\n\n"
        "a: %03u [$%02x]   f: %03u [$%02x]\n"
        "b: %03u [$%02x]   c: %03u [$%02x]\n"
        "d: %03u [$%02x]   e: %03u [$%02x]\n"
        "h: %03u [$%02x]   l: %03u [$%02x]\n"
        "s: %03u [$%02x]   p: %03u [$%02x]\n\n"
        "Checksums:\n"
        "–---------\n\n"
        "WRAM Checksum: %" PRIu64 "\n"
        "VRAM Checksum: %" PRIu64 "\n",
        state.registers.a,
        state.registers.a,
        state.registers.f,
        state.registers.f,
        state.registers.b,
        state.registers.b,
        state.registers.c,
        state.registers.c,
        state.registers.d,
        state.registers.d,
        state.registers.e,
        state.registers.e,
        state.registers.h,
        state.registers.h,
        state.registers.l,
        state.registers.l,
        state.registers.stack_ptr,
        state.registers.stack_ptr,
        state.registers.program_counter,
        state.registers.program_counter,
        state.wram_checksum,
        state.vram_checksum);

    auto str = String(buffer);
    free(buffer);
    return str;
}

CPUTestState CPU::test_state()
{
    CPUTestState result;
    result.registers = m_registers;
    result.wram_checksum = checksum((const unsigned char*)emulator().mmu().wram(), WRAM_SIZE);
    result.vram_checksum = checksum((const unsigned char*)emulator().mmu().vram(), VRAM_SIZE);
    return result;
}
