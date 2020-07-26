//
// Created by Spencer Dixon on 5/6/20.
//

#pragma once

#include "IODevice.h"
#include "Joypad.h"
#include "MMU.h"
#include "OpCode.h"

#include <SD/Bytes.h>
#include <SD/LogStream.h>
#include <SD/Option.h>
#include <SD/Types.h>
#include <SD/Vector.h>

#include <stdlib.h>

class Emulator;
class Debugger;

static constexpr u8 RESET_INTERRUPT = 0x0;
static constexpr u8 VBLANK_INTERRUPT = 0x1;

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

struct CPUTestState {
    Registers registers;
    u64 wram_checksum;
    u64 vram_checksum;
};

String to_trace_line(const CPUTestState&);
String to_step_line(const CPUTestState&);
String to_snapshot(const CPUTestState&);
String to_human_readable_flags(u8 flag);

class CPU final : public IODevice {
public:
    explicit CPU(Emulator& emulator);
    ~CPU();

    void main_loop();
    void main_test_loop();
    OpCode execute_one_instruction();

    Emulator& emulator() { return m_emulator; }
    //    bool interrupts_enabled() { return m_interrupts_enabled; }
    bool in_boot_rom() { return m_in_boot_rom; }
    void set_interrupt_flag(u8);

    //
    // IODevice
    //
    u8 in(u16 address) override;
    void out(u16 address, u8 value) override;

    //
    // Testing/Debug Utilities
    //
    CPUTestState test_state();
    void attach_debugger(Debugger* debugger) { m_debugger = debugger; }
    void detach_debugger() { m_debugger = nullptr; }
    void add_breakpoint(u16 breakpoint)
    {
        m_breakpoints.append(breakpoint);
    }

private:
    void handle_prefix_op_code(const PrefixOpCode& op_code);

    // Memory Access
    u8 read(u16 address);
    void write(u16 address, u8 data);
    void push(u8* reg_one, u8* reg_two);
    void push(u16 addr);
    void pop(u8* reg_one, u8* reg_two);
    void pop_return();

    // Arithmetic
    void cp_a(u8* value_ptr);

    // Bit Twiddling
    void shift_left(u8* reg_ptr);
    void shift_right_into_carry(u8* reg_ptr);
    void xor_value_with_a(u8 value);
    void and_with_a(u8 value);
    void or_with_a(u8 value);
    void swap_reg(u8* reg_ptr);
    void rotate_left(u8* reg_ptr);
    void reset_bit(u8 bit_to_reset, u8* value_ptr);
    void check_bit(u8 bit_to_check, u8* value_ptr);

    // Flag Setting
    void set_zero_flag(bool should_set);
    void set_carry_flag(bool should_set);
    void set_half_carry_flag(bool should_set);
    void set_subtract_flag(bool should_set);
    bool get_carry_flag();
    bool get_half_carry_flag();
    bool get_subtract_flag();
    bool get_zero_flag();

    // Register related utilities
    // TODO(scd): consider maybe moving into Register class?
    u16 get_de() { return to_le_16_bit(m_registers.e, m_registers.d); }
    u16 get_hl() { return to_le_16_bit(m_registers.l, m_registers.h); }
    u16 get_bc() { return to_le_16_bit(m_registers.c, m_registers.b); }
    u16 get_sp() { return m_registers.stack_ptr; }
    void set_de(u16 value)
    {
        m_registers.e = value;
        m_registers.d = (value >> 8);
    }
    void set_hl(u16 value)
    {
        m_registers.l = value;
        m_registers.h = (value >> 8);
    }
    void set_bc(u16 value)
    {
        m_registers.c = value;
        m_registers.b = (value >> 8);
    }
    void inc_hl()
    {
        u16 hl = get_hl();
        set_hl(++hl);
    }
    void dec_hl()
    {
        u16 hl = get_hl();
        set_hl(--hl);
    }
    void inc_de()
    {
        u16 de = get_de();
        set_de(++de);
    }
    void dec_de()
    {
        u16 de = get_de();
        set_de(--de);
    }
    void inc_bc()
    {
        u16 bc = get_bc();
        set_bc(++bc);
    }
    void dec_bc()
    {
        u16 bc = get_bc();
        set_bc(--bc);
    }
    void inc_sp() { m_registers.stack_ptr++; }
    void dec_sp() { m_registers.stack_ptr--; }
    void inc_reg(u8* reg_ptr)
    {
        set_subtract_flag(false);
        set_half_carry_flag(will_half_carry(*reg_ptr, 1));
        *reg_ptr += 1;
        set_zero_flag(*reg_ptr == 0);
    }
    void dec_reg(u8* reg_ptr)
    {
        set_subtract_flag(true);
        set_half_carry_flag(will_half_carry(*reg_ptr, 1));
        *reg_ptr -= 1;
        set_zero_flag(*reg_ptr == 0);
    }
    void add_a(u8 value)
    {
        set_subtract_flag(false);
        set_half_carry_flag(will_half_carry(m_registers.a, value));
        set_carry_flag(will_carry(m_registers.a, value));
        m_registers.a += value;
        set_zero_flag(m_registers.a == 0);
    }

    //
    // Fetch
    //
    u8 fetch_and_inc_u8();
    s8 fetch_and_inc_s8();
    u16 fetch_and_inc_u16();

    //
    // Debug
    //
    bool in_breakpoint();
    bool should_skip_boot_rom();

private:
    Emulator& m_emulator;
    Registers m_registers;

    u64 m_cycles_executed { 0 };
    bool m_halted { false };
    bool m_interrupts_enabled { false };
    bool m_in_boot_rom { true };
    u8 m_interrupt_flag { 0 };

    // Debugging
    Debugger* m_debugger { nullptr };
    Vector<u16> m_breakpoints {};
};

const LogStream& operator<<(const LogStream&, const CPUTestState&);
const LogStream& operator<<(const LogStream&, CPU&);
