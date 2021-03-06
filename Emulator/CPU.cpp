//
// Created by Spencer Dixon on 5/6/20.
//

#include "CPU.h"
#include "Emulator.h"
#include "IODevice.h"
#include "MemoryMap.h"

#include <SD/Assertions.h>
#include <SD/File.h>

#include <cinttypes>
#include <cstdio>

// 4.194304 MHz -> 4,194,304 / 60 = 69905
//static constexpr u32 CYCLES_PER_SECOND = 69905;
// 70224 clocks = 17556 cycles = 59.7 fps
static constexpr u32 CYCLES_PER_FRAME = 70224;

// Flags
// clang-format off
constexpr u8 FLAG_ZERO       = 0b10000000;
constexpr u8 FLAG_SUBTRACT   = 0b01000000;
constexpr u8 FLAG_HALF_CARRY = 0b00100000;
constexpr u8 FLAG_CARRY      = 0b00010000;
// clang-format on

CPU::CPU(Emulator& emulator)
    : m_emulator(emulator)
    , m_registers({ 0 })
{
}

CPU::~CPU()
{
}

void CPU::boot()
{
    if (should_skip_boot_rom()) {
        exit_boot_rom();
    } else {
        m_registers.program_counter = 0x00;
    }
}

void CPU::main_loop()
{
    for (;;) {
        if (m_halted)
            break;

        if (in_breakpoint() && !m_debugger)
            attach_debugger(&emulator().debugger());

        if (m_debugger)
            while (m_debugger->loop() == DebuggerResult::Continue) { }

        int prev_cycle_count = m_cycles_executed;
        execute_one_instruction();

        int cycles = m_cycles_executed - prev_cycle_count;

        emulator().ppu().update_by(cycles);
        emulator().timer().update_by(cycles);
        // TODO: Do Interrupts
        // if (has_interrupt_request)
        // handle_interrupt

        if (m_cycles_executed >= CYCLES_PER_FRAME) {
            m_cycles_executed = 0;
            break;
        }
    }
}

// TODO: This separation between test and main loop has lead to some issues.
// This really needs to be unified and rethought.
void CPU::main_test_loop()
{
    // Test cases should all be able to execute in less than 20 seconds. If we've been executing
    // for more than 20 seconds there is probably a bug in our CPU/Test ASM that needs to be addressed.
    u64 max_test_cycles = CYCLES_PER_FRAME * 20;

    dbg() << "Trace:\n------\n";
    for (;;) {
        int prev_cycle_count = m_cycles_executed;
        execute_one_instruction();
        int cycles = m_cycles_executed - prev_cycle_count;
        emulator().ppu().update_by(cycles);
        emulator().timer().update_by(cycles);

        if (m_halted)
            ::exit(0);

        if (m_cycles_executed >= max_test_cycles) {
            dbg() << " [ " RED "ERROR: " RESET " ] test case reached max cycle count!";
            ::exit(1);
        }
    }
}

OpCode CPU::execute_one_instruction()
{
    handle_interrupts();

    OpCode op_code = static_cast<OpCode>(fetch_and_inc_u8());
    m_cycles_executed += cycles_for_opcode(op_code);

    // TODO: Potentially extract hl src/dst and don't go through this flow for those potentially.
    if ((u8)op_code >= 0x40 && (u8)op_code < 0x80 && op_code != OpCode::HALT) {
        handle_load_op_code(op_code);
        log_test_state(op_code);
        return op_code;
    }

    switch (op_code) {
    case OpCode::NOP:
        break;
    case OpCode::RLCA: {
        bool will_carry = m_registers.a & 0x80;
        set_subtract_flag(false);
        set_half_carry_flag(false);
        set_zero_flag(false);
        set_carry_flag(will_carry);
        m_registers.a <<= 1;
        if (will_carry)
            m_registers.a += 1;
        break;
    }
    case OpCode::RLA: {
        bool will_carry = m_registers.a & 0x80;
        m_registers.a <<= 1;
        if (m_registers.f & FLAG_CARRY)
            m_registers.a += 1;

        set_subtract_flag(false);
        set_half_carry_flag(false);
        set_zero_flag(false);
        set_carry_flag(will_carry);
        break;
    }
    case OpCode::CPL:
        m_registers.a = ~m_registers.a;
        break;
    case OpCode::LD_A_d8:
        m_registers.a = fetch_and_inc_u8();
        break;
    case OpCode::LD_B_d8:
        m_registers.b = fetch_and_inc_u8();
        break;
    case OpCode::LD_C_d8:
        m_registers.c = fetch_and_inc_u8();
        break;
    case OpCode::LD_D_d8:
        m_registers.d = fetch_and_inc_u8();
        break;
    case OpCode::LD_E_d8:
        m_registers.e = fetch_and_inc_u8();
        break;
    case OpCode::LD_H_d8:
        m_registers.h = fetch_and_inc_u8();
        break;
    case OpCode::LD_L_d8:
        m_registers.l = fetch_and_inc_u8();
        break;
    case OpCode::LD_A_C_ADDR: {
        u16 address = 0xff00 + m_registers.c;
        m_registers.a = read(address);
        break;
    }
    case OpCode::LD_C_ADDR_A: {
        u16 address = 0xff00 + m_registers.c;
        write(address, m_registers.a);
        break;
    }
    case OpCode::LD_A_a16_ADDR:
        m_registers.a = read(fetch_and_inc_u16());
        break;
    case OpCode::LD_A_HL_ADDR_INC:
        m_registers.a = read(get_hl());
        inc_hl();
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
    case OpCode::LD_a16_ADDR_A:
        write(fetch_and_inc_u16(), m_registers.a);
        break;
    case OpCode::LD_a16_ADDR_SP:
        write(fetch_and_inc_u16(), m_registers.stack_ptr);
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
        write(get_hl(), fetch_and_inc_u8());
        break;
    case OpCode::LDH_a8_ADDR_A: {
        u16 address = IO_START + fetch_and_inc_u8();
        write(address, m_registers.a);
        break;
    }
    case OpCode::LDH_A_a8_ADDR: {
        u16 address = IO_START + fetch_and_inc_u8();
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
    case OpCode::DEC_HL_ADDR:
        dec_addr(get_hl());
        break;
    case OpCode::ADD_A_d8:
        add_a(fetch_and_inc_u8());
        break;
    case OpCode::ADD_A_A:
        add_a(m_registers.a);
        break;
    case OpCode::ADD_A_B:
        add_a(m_registers.b);
        break;
    case OpCode::ADD_A_C:
        add_a(m_registers.c);
        break;
    case OpCode::ADD_A_D:
        add_a(m_registers.d);
        break;
    case OpCode::ADD_A_E:
        add_a(m_registers.e);
        break;
    case OpCode::ADD_A_H:
        add_a(m_registers.h);
        break;
    case OpCode::ADD_A_L:
        add_a(m_registers.l);
        break;
    case OpCode::ADD_A_HL_ADDR:
        add_a(read(get_hl()));
        break;
    case OpCode::ADD_SP_r8: {
        u8 value = fetch_and_inc_s8();
        set_zero_flag(false);
        set_subtract_flag(false);
        set_carry_flag(will_carry(get_sp(), value));
        m_registers.stack_ptr += value;
        // TODO: half-carry
        break;
    }
    case OpCode::ADD_HL_DE:
        add_hl(get_de());
        break;
    case OpCode::ADD_HL_BC:
        add_hl(get_bc());
        break;
    case OpCode::ADD_HL_HL:
        add_hl(get_hl());
        break;
    case OpCode::ADD_HL_SP:
        add_hl(m_registers.stack_ptr);
        break;
    case OpCode::JP_NZ_a16: {
        u16 new_address = fetch_and_inc_u16();
        if (!get_zero_flag())
            m_registers.program_counter = new_address;
        break;
    }
    case OpCode::JP_Z_a16: {
        u16 new_address = fetch_and_inc_u16();
        if (get_zero_flag())
            m_registers.program_counter = new_address;
        break;
    }
    case OpCode::JP_a16:
        m_registers.program_counter = fetch_and_inc_u16();
        break;
    case OpCode::JR_r8:
        m_registers.program_counter += fetch_and_inc_s8();
        break;
    case OpCode::JR_NZ_r8: {
        s8 offset = fetch_and_inc_s8();
        if (!get_zero_flag())
            m_registers.program_counter += offset;
        break;
    }
    case OpCode::JR_Z_r8: {
        s8 offset = fetch_and_inc_s8();
        if (get_zero_flag())
            m_registers.program_counter += offset;
        break;
    }
    case OpCode::JR_NC_r8: {
        s8 offset = fetch_and_inc_s8();
        if (!get_carry_flag())
            m_registers.program_counter += offset;
        break;
    }
    case OpCode::JR_C_r8: {
        s8 offset = fetch_and_inc_s8();
        if (get_carry_flag())
            m_registers.program_counter += offset;
        break;
    }
    case OpCode::JP_HL:
        m_registers.program_counter = get_hl();
        break;
    case OpCode::CP_A:
        cp_a(&m_registers.a);
        break;
    case OpCode::CP_B:
        cp_a(&m_registers.b);
        break;
    case OpCode::CP_C:
        cp_a(&m_registers.c);
        break;
    case OpCode::CP_D:
        cp_a(&m_registers.d);
        break;
    case OpCode::CP_E:
        cp_a(&m_registers.e);
        break;
    case OpCode::CP_H:
        cp_a(&m_registers.h);
        break;
    case OpCode::CP_L:
        cp_a(&m_registers.l);
        break;
    case OpCode::CP_HL_ADDR: {
        u8 value = read(get_hl());
        cp_a(&value);
        break;
    }
    case OpCode::CP_d8: {
        u8 value = fetch_and_inc_u8();
        cp_a(&value);
        break;
    }
    // FIXME: We need to be setting flags properly on subtracts!
    case OpCode::SUB_d8:
        set_subtract_flag(true);
        m_registers.a -= fetch_and_inc_u8();
        set_zero_flag(m_registers.a == 0);
        break;
    case OpCode::SUB_B:
        set_subtract_flag(true);
        m_registers.a -= m_registers.b;
        set_zero_flag(m_registers.a == 0);
        break;
    case OpCode::SUB_HL_ADDR:
        set_subtract_flag(true);
        m_registers.a -= read(get_hl());
        set_zero_flag(m_registers.a == 0);
        break;
    case OpCode::LD_HL_d16:
        m_registers.l = fetch_and_inc_u8();
        m_registers.h = fetch_and_inc_u8();
        break;
    case OpCode::LD_DE_d16:
        m_registers.e = fetch_and_inc_u8();
        m_registers.d = fetch_and_inc_u8();
        break;
    case OpCode::LD_BC_d16:
        m_registers.c = fetch_and_inc_u8();
        m_registers.b = fetch_and_inc_u8();
        break;
    case OpCode::LD_SP_d16:
        m_registers.stack_ptr = fetch_and_inc_u16();
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
    case OpCode::AND_A:
        and_with_a(m_registers.a);
        break;
    case OpCode::AND_B:
        and_with_a(m_registers.b);
        break;
    case OpCode::AND_C:
        and_with_a(m_registers.c);
        break;
    case OpCode::AND_D:
        and_with_a(m_registers.d);
        break;
    case OpCode::AND_E:
        and_with_a(m_registers.e);
        break;
    case OpCode::AND_H:
        and_with_a(m_registers.h);
        break;
    case OpCode::AND_L:
        and_with_a(m_registers.l);
        break;
    case OpCode::AND_d8:
        and_with_a(fetch_and_inc_u8());
        break;
    case OpCode::OR_A:
        or_with_a(m_registers.a);
        break;
    case OpCode::OR_B:
        or_with_a(m_registers.b);
        break;
    case OpCode::OR_C:
        or_with_a(m_registers.c);
        break;
    case OpCode::OR_D:
        or_with_a(m_registers.d);
        break;
    case OpCode::OR_E:
        or_with_a(m_registers.e);
        break;
    case OpCode::OR_H:
        or_with_a(m_registers.h);
        break;
    case OpCode::OR_L:
        or_with_a(m_registers.l);
        break;
    case OpCode::OR_d8:
        or_with_a(fetch_and_inc_u8());
        break;
    case OpCode::XOR_A:
        xor_value_with_a(m_registers.a);
        break;
    case OpCode::XOR_B:
        xor_value_with_a(m_registers.b);
        break;
    case OpCode::XOR_C:
        xor_value_with_a(m_registers.c);
        break;
    case OpCode::XOR_D:
        xor_value_with_a(m_registers.d);
        break;
    case OpCode::XOR_E:
        xor_value_with_a(m_registers.e);
        break;
    case OpCode::XOR_H:
        xor_value_with_a(m_registers.h);
        break;
    case OpCode::XOR_L:
        xor_value_with_a(m_registers.l);
        break;
    case OpCode::XOR_d8:
        xor_value_with_a(fetch_and_inc_u8());
        break;
    case OpCode::DI:
        m_interrupt_master_enable_flag = false;
        break;
    case OpCode::EI:
        m_interrupt_master_enable_flag = true;
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
        m_registers.program_counter = fetch_and_inc_u16();
        break;
    case OpCode::RST_00H:
        rst(0);
        break;
    case OpCode::RST_08H:
        rst(0x8);
        break;
    case OpCode::RST_10H:
        rst(0x1);
        break;
    case OpCode::RST_18H:
        rst(0x18);
        break;
    case OpCode::RST_20H:
        rst(0x20);
        break;
    case OpCode::RST_28H:
        rst(0x28);
        break;
    case OpCode::RST_30H:
        rst(0x30);
        break;
    case OpCode::RST_38H:
        rst(0x38);
        break;
    case OpCode::RET_C:
        if (get_carry_flag())
            pop_return();
        break;
    case OpCode::RET_NC:
        if (!get_carry_flag())
            pop_return();
        break;
    case OpCode::RET_NZ:
        if (!get_zero_flag())
            pop_return();
        break;
    case OpCode::RET_Z:
        if (get_zero_flag())
            pop_return();
        break;
    case OpCode::RET:
        pop_return();
        break;
    case OpCode::RETI:
        m_interrupt_master_enable_flag = true;
        pop_return();
        break;
    case OpCode::TEST_COMPLETE:
    case OpCode::HALT:
        // hex_dump("WRAM", emulator().mmu().wram(), 100, WRAM_START);
        // hex_dump("VRAM", emulator().mmu().vram(), VRAM_SIZE, VRAM_START);
        m_halted = true;
        break;
    case OpCode::PREFIX: {
        PrefixOpCode prefix_op_code = static_cast<PrefixOpCode>(fetch_and_inc_u8());
        handle_prefix_op_code(prefix_op_code);
        m_cycles_executed += cycles_for_prefix_opcode(prefix_op_code);
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

    log_test_state(op_code);

    return op_code;
}

void CPU::handle_load_op_code(const OpCode& op_code)
{
    u8 code = static_cast<u8>(op_code);
    ASSERT(code >= 0x40 && code < 0x80);

    // LD_dest_src
    u8 src = code & 0x7;         // 00000SSS
    u8 dest = (code >> 3) & 0x7; // 00DDD000

    // FIXME: Could be created once instead of every handle_op call
    u8* load_table[8] = {
        &m_registers.b,
        &m_registers.c,
        &m_registers.d,
        &m_registers.e,
        &m_registers.h,
        &m_registers.l,
        nullptr, // address of HL, need to manually read/write
        &m_registers.a,
    };

    // Andreas: is there a better way to handle this when some values need a
    // read/write indirection step?
    if (src == 6 && dest == 6) {
        write(get_hl(), read(get_hl()));
    } else if (src == 6) {
        *load_table[dest] = read(get_hl());
    } else if (dest == 6) {
        write(get_hl(), *load_table[src]);
    } else {
        *load_table[dest] = *load_table[src];
    }
}

void CPU::handle_prefix_op_code(const PrefixOpCode& op_code)
{
    switch (op_code) {
    case PrefixOpCode::RL_A:
        rotate_left_through_carry(&m_registers.a);
        break;
    case PrefixOpCode::RL_B:
        rotate_left_through_carry(&m_registers.b);
        break;
    case PrefixOpCode::RL_C:
        rotate_left_through_carry(&m_registers.c);
        break;
    case PrefixOpCode::RL_D:
        rotate_left_through_carry(&m_registers.d);
        break;
    case PrefixOpCode::RL_E:
        rotate_left_through_carry(&m_registers.e);
        break;
    case PrefixOpCode::RL_H:
        rotate_left_through_carry(&m_registers.h);
        break;
    case PrefixOpCode::RL_L:
        rotate_left_through_carry(&m_registers.l);
        break;
    case PrefixOpCode::SRL_L: { // TODO: Generalize for other prefixed op codes
        u8 carry = m_registers.l & 0x01;
        u8 new_value = m_registers.l >> 1;
        set_zero_flag(new_value == 0);
        set_subtract_flag(false);
        set_half_carry_flag(false);
        set_carry_flag(carry != 0);
        m_registers.l = new_value;
        break;
    }
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
    case PrefixOpCode::SRA_A:
        shift_right_into_carry(&m_registers.a);
        break;
    case PrefixOpCode::SRA_B:
        shift_right_into_carry(&m_registers.b);
        break;
    case PrefixOpCode::SWAP_A:
        swap_reg(&m_registers.a);
        break;
    case PrefixOpCode::BIT_7_H:
        check_bit(7, &m_registers.h);
        break;
    case PrefixOpCode::BIT_5_H:
        check_bit(5, &m_registers.h);
        break;
    case PrefixOpCode::BIT_0_L:
        check_bit(0, &m_registers.l);
        break;;
    case PrefixOpCode::BIT_0_HL_ADDR: {
        u8 value = read(get_hl());
        check_bit(0, &value);
        //        dbg() << "zero flag: " << get_zero_flag();
        write(get_hl(), value);
        break;
    }
    case PrefixOpCode::RES_0_HL_ADDR: {
        u8 value = read(get_hl());
        reset_bit(0, &value);
        write(get_hl(), value);
        break;
    }
    case PrefixOpCode::RES_0_A:
        reset_bit(0, &m_registers.a);
        break;
    case PrefixOpCode::RES_0_B:
        reset_bit(0, &m_registers.b);
        break;
    case PrefixOpCode::RES_0_C:
        reset_bit(0, &m_registers.c);
        break;
    case PrefixOpCode::RES_0_D:
        reset_bit(0, &m_registers.d);
        break;
    default:
        if (is_prefix_opcode(op_code)) {
            printf("[ " RED "FATAL" RESET " ] "
                   "Missing implementation for the following prefix op code: ");
            print_prefix_opcode(op_code);
        } else {
            printf("missing prefix op code: %x\n", (u8)op_code);
        }
        ASSERT_NOT_REACHED();
    }

    if (emulator().settings().verbose_logging || emulator().settings().in_test_mode) {
        dbg() << to_string(op_code) << "   " << *this;
    }
}

void CPU::handle_interrupts()
{
    if (!m_interrupt_master_enable_flag)
        return;

    // Bit 0: VBlank
    // Bit 1: LCD Stat
    // Bit 2: Timer
    // Bit 3: Serial
    // Bit 4: Joypad
    for (int i = 0; i < 5; i++) {
        auto flag_to_test = static_cast<InterruptFlags>(1 << i);

        if (is_on(m_interrupt_enable, flag_to_test) && is_on(m_interrupt_flag, flag_to_test)) {
            m_interrupt_master_enable_flag = false;
            m_interrupt_flag = unset(m_interrupt_flag, flag_to_test);
            push(m_registers.program_counter);
            m_registers.program_counter = jump_vector_for(flag_to_test);
            break;
        }
    }
}

void CPU::exit_boot_rom()
{
    m_in_boot_rom = false;
    m_registers.program_counter = 0x100;
    m_registers.stack_ptr = 0xfffe;
    m_registers.a = 0x01;
    m_registers.f = 0xb0;
    m_registers.b = 0x00;
    m_registers.c = 0x13;
    m_registers.d = 0x00;
    m_registers.e = 0xd8;
    m_registers.h = 0x01;
    m_registers.l = 0x4d;

    if (!emulator().settings().in_test_mode) {
        write(0xff05, 0x00); // TIMA
        write(0xff06, 0x00); // TMA
        write(0xff07, 0x00); // TAC
        write(0xff10, 0x80); // NR10
        write(0xff11, 0xbf); // NR11
        write(0xff12, 0xf3); // NR12
        write(0xff14, 0xbf); // NR14
        write(0xff16, 0x3f); // NR21
        write(0xff17, 0x00); // NR22
        write(0xff19, 0xbf); // NR24
        write(0xff1a, 0x7f); // NR30
        write(0xff1b, 0xff); // NR31
        write(0xff1c, 0x9f); // NR32
        write(0xff1e, 0xbf); // NR33
        write(0xff20, 0xff); // NR41
        write(0xff21, 0x00); // NR42
        write(0xff22, 0x00); // NR43
        write(0xff23, 0xbf); // NR44
        write(0xff24, 0x77); // NR50
        write(0xff25, 0xf3); // NR51
        write(0xff26, 0xf1); // NR52
        write(0xff40, 0x91); // LCDC
        write(0xff42, 0x00); // SCY
        write(0xff43, 0x00); // SCX
        write(0xff45, 0x00); // LYC
        write(0xff47, 0xfc); // BGP
        write(0xff48, 0xff); // OBP0
        write(0xff49, 0xff); // OBP1
        write(0xff4a, 0x00); // WY
        write(0xff4b, 0x00); // WX
        write(0xffff, 0x00); // IE
    }
}

//
// Memory Access
//
s8 CPU::fetch_and_inc_s8()
{
    s8 next;
    if (m_in_boot_rom && !emulator().settings().in_test_mode)
        next = emulator().mmu().bios()[m_registers.program_counter];
    else
        next = emulator().mmu().rom()[m_registers.program_counter];

    m_registers.program_counter++;
    return next;
}

u8 CPU::fetch_and_inc_u8()
{
    u8 next;
    if (m_in_boot_rom && !emulator().settings().in_test_mode)
        next = emulator().mmu().bios()[m_registers.program_counter];
    else
        next = emulator().mmu().rom()[m_registers.program_counter];

    m_registers.program_counter++;
    return next;
}

u16 CPU::fetch_and_inc_u16()
{
    u8 b1 = fetch_and_inc_u8();
    u8 b2 = fetch_and_inc_u8();
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
    //    dbg() << "CPU::in(" << to_hex(address) << ") PC: " << m_registers.program_counter;

    if (address == IF_LOCATION) {
        // READ the interrupt flag
        return static_cast<u8>(m_interrupt_flag);
    }

    return 0;
}

void CPU::out(u16 address, u8 value)
{
    if (emulator().settings().verbose_logging)
        dbg() << "CPU::out(" << to_hex(address) << ", " << to_hex(value) << ")";

    // End of boot rom sequence
    if (address == 0xff50 && value == 1) {
        return exit_boot_rom();
    }

    // Write to the interrupt flag signaling that we'd like an interrupt to occur
    if (address == IF_LOCATION) {
        m_interrupt_flag = static_cast<InterruptFlags>(value);
    }

    // FIXME: Look into this to confirm this can actually be written from code.
    if (address == IE_LOCATION) {
        m_interrupt_enable = static_cast<InterruptFlags>(value);
    }
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

void CPU::rst(u16 jump_to_address)
{
    push(m_registers.program_counter);
    m_registers.program_counter = jump_to_address;
}

//
// Arithmetic
//
void CPU::cp_a(u8* value_ptr)
{
    // TODO: half carry
    set_subtract_flag(true);
    set_zero_flag(m_registers.a == *value_ptr);
    set_carry_flag(m_registers.a < *value_ptr);
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

void CPU::shift_right_into_carry(u8* reg_ptr)
{
    u8 lsb = *reg_ptr & 0x1;
    set_half_carry_flag(false);
    set_subtract_flag(false);
    set_carry_flag((bool)lsb);
    *reg_ptr >>= 1;
    *reg_ptr ^= (-lsb ^ *reg_ptr) & (1 << 7);
}

void CPU::xor_value_with_a(u8 value)
{
    u8 result = m_registers.a ^ value;
    m_registers.a = result;
    set_zero_flag(result == 0);
    set_half_carry_flag(false);
    set_subtract_flag(false);
    set_carry_flag(false);
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

void CPU::check_bit(u8 bit_to_check, u8* reg_ptr)
{
    set_zero_flag(((1 << bit_to_check) & *reg_ptr) == 0);
    set_subtract_flag(false);
    set_half_carry_flag(true);
}

void CPU::rotate_left_through_carry(u8* reg_ptr)
{
    bool will_carry = (*reg_ptr & 0x80) == 0x80;
    *reg_ptr <<= 1;

    if (m_registers.f & FLAG_CARRY)
        *reg_ptr += 1;

    set_zero_flag(*reg_ptr == 0);
    set_carry_flag(will_carry);
    set_subtract_flag(false);
    set_half_carry_flag(false);
}

void CPU::reset_bit(u8 bit_to_reset, u8* value_ptr)
{
    *value_ptr &= ~(1 << bit_to_reset);
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

bool CPU::get_carry_flag()
{
    return m_registers.f & FLAG_CARRY;
}
bool CPU::get_half_carry_flag()
{
    return m_registers.f & FLAG_HALF_CARRY;
}
bool CPU::get_subtract_flag()
{
    return m_registers.f & FLAG_SUBTRACT;
}
bool CPU::get_zero_flag()
{
    return m_registers.f & FLAG_ZERO;
}

void CPU::set_interrupt_flag(InterruptFlags flag)
{
    m_interrupt_flag = flag;
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
String to_human_readable_flags(u8 flags)
{
    String result;
    result += (flags & FLAG_ZERO) ? "Z" : "-";
    result += (flags & FLAG_SUBTRACT) ? "N" : "-";
    result += (flags & FLAG_HALF_CARRY) ? "H" : "-";
    result += (flags & FLAG_CARRY) ? "C" : "-";
    return result;
}

String to_trace_line(const CPUTestState& test_state)
{
    local_persist u16 buf_size = 512;
    char* buffer = (char*)calloc(buf_size, sizeof(char));
    snprintf(
        buffer,
        buf_size,
        "PC [0x%04x] A: %03u [0x%02x] F: %03u [0x%02x] %s "
        "B: %03u [0x%02x] C: %03u [0x%02x] "
        "D: %03u [0x%02x] E: %03u [0x%02x] "
        "H: %03u [0x%02x] L: %03u [0x%02x] "
        " WRAM: 0x%08" PRIx64 " | VRAM: 0x%08" PRIx64 " |",
        test_state.registers.program_counter,
        test_state.registers.a,
        test_state.registers.a,
        test_state.registers.f,
        test_state.registers.f,
        to_human_readable_flags(test_state.registers.f).characters(),
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

String to_step_line(const CPUTestState& state)
{
    local_persist u16 buf_size = 512;
    char* buffer = (char*)calloc(buf_size, sizeof(char));
    snprintf(
        buffer,
        buf_size,
        "Flags: %s\n"
        "a: %03u [$%02x]       f: %03u [$%02x]\n"
        "b: %03u [$%02x]       c: %03u [$%02x]\n"
        "d: %03u [$%02x]       e: %03u [$%02x]\n"
        "h: %03u [$%02x]       l: %03u [$%02x]\n"
        "s: %03u [$%02x]   p: %03u [$%02x]\n",
        to_human_readable_flags(state.registers.f).characters(),
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
        state.registers.program_counter);

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

bool CPU::in_breakpoint()
{
    return emulator().settings().in_debug_mode && !m_breakpoints.is_empty() && m_breakpoints.contains_slow(m_registers.program_counter);
}

bool CPU::should_skip_boot_rom()
{
    return emulator().settings().in_test_mode;
}

void CPU::log_test_state(const OpCode& op_code)
{
    if (emulator().settings().verbose_logging || emulator().settings().in_test_mode) {
        dbg() << to_string(op_code) << "   " << *this;
        if (m_halted)
            dbg() << to_snapshot(test_state());
    }
}
