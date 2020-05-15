//
// Created by Spencer Dixon on 5/6/20.
//

#pragma once

#include "OpCode.h"
#include <SD/Bytes.h>
#include <SD/LogStream.h>
#include <SD/Types.h>

#include <stdlib.h>

constexpr size_t WRAM_SIZE = KB * 32;
constexpr size_t VRAM_SIZE = KB * 16;
constexpr size_t IO_SIZE = 112;

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
void print_registers(const Registers& reg);

struct CPUTestState {
    Registers registers;
    u64 wram_checksum;
    u64 vram_checksum;
    u64 io_checksum;
};

class CPU {

public:
    CPU(bool verbose_logging = false)
        : m_verbose_logging(verbose_logging)
        , m_registers({ 0 })
    {
        m_wram = (u8*)calloc(WRAM_SIZE, sizeof(u8));
        m_vram = (u8*)calloc(VRAM_SIZE, sizeof(u8));
        m_io_registers = (u8*)calloc(IO_SIZE, sizeof(u8));

        // Initial Register Values:
        // AF 1180
        // BC 0000
        // de ff56
        // hl 000d
        // sp fffe
        // pc 0100
        m_registers.stack_ptr = 0xfffe;
        m_registers.program_counter = 0x100;
    }

    ~CPU()
    {
        if (m_rom)
            free(m_rom);

        free(m_wram);
        free(m_vram);
    }

    void load_rom(const char* rom_path);
    bool step();


    // Memory accessors
    u8* v_ram() { return m_vram; }

    // Andreas: Should this be protected somehow and use a Friend class or something?
    CPUTestState test_state()
    {
        CPUTestState result;
        result.registers = m_registers;
        result.wram_checksum = checksum((const unsigned char*)m_wram, WRAM_SIZE);
        result.vram_checksum = checksum((const unsigned char*)m_vram, VRAM_SIZE);
        result.io_checksum = checksum((const unsigned char*)m_io_registers, IO_SIZE);
        return result;
    }

private:
    u8 read(u16 address);
    void write(u16 address, u8 data);
    u8 fetch_and_inc();

    // TODO(scd): change m_register.f based on should_set
    void set_zero_flag(bool should_set);
    void set_carry_flag(bool should_set);
    void set_half_carry_flag(bool should_set);
    void set_subtract_flag(bool should_set);

    // Register related utilities
    // TODO(scd): consider maybe moving into Register class
    u16 get_de() { return to_le_16_bit(m_registers.e, m_registers.d); }
    u16 get_hl() { return to_le_16_bit(m_registers.l, m_registers.h); }
    u16 get_bc() { return to_le_16_bit(m_registers.c, m_registers.b); }
    void set_de(u16 value) {
        m_registers.e = value;
        m_registers.d = (value >> 8);
    }
    void set_hl(u16 value) {
        m_registers.l = value;
        m_registers.h = (value >> 8);
    }
    void set_bc(u16 value) {
        m_registers.c = value;
        m_registers.b = (value >> 8);
    }
    void inc_hl() {
        u16 hl = get_hl();
        set_hl(++hl);
    }
    void inc_de() {
        u16 de = get_de();
        set_de(++de);
    }

private:
    bool m_verbose_logging;
    Registers m_registers;

    // TODO(scd): MMU / Bus which resolves pointers to the correct address bank.
    // Memory is very different from CPU and should belong in different places.
    u8* m_rom { nullptr };
    u8* m_wram { nullptr };
    u8* m_vram { nullptr };
    u8* m_io_registers { nullptr };
};