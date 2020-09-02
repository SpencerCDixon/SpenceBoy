//
// Created by Spencer Dixon on 8/30/2020
//

#include <SD/Assertions.h>
#include <SD/LogStream.h>

#include "Timer.h"
#include "Emulator.h"
#include "InterruptFlags.h"

Timer::Timer(Emulator& emulator)
    : m_emulator(emulator)
{
}

u8 Timer::in(u16 address)
{
    switch (address) {
    case 0xFF04:
        return m_div;
    case 0xFF05:
        return m_tima;
    case 0xFF06:
        return m_tma;
    case 0xFF07:
        return m_tac;
    default:
        ASSERT_NOT_REACHED();
    }
}
void Timer::out(u16 address, u8 value)
{
    if (address == 0xFF07) {
        // trying to change the timer frequency and maybe need to update
        u8 old_freq = get_clock_freq();
        m_tac = value;
        u8 new_freq = get_clock_freq();

        if (old_freq != new_freq)
            set_clock_freq();
    } else if (address == 0xFF04) {
        // If the GB ever tries to write the divider register we reset it to 0
        m_div = 0;
    }
}

void Timer::update_by(u8 cycles)
{
    update_divider_reg(cycles);

    if (is_clock_enabled()) {
        m_timer_counter -= cycles;

        // Timer has been triggered
        if (m_timer_counter <= 0) {
            set_clock_freq();
            if (m_tima == 255) { // about to overflow
                m_tima = m_tma;  // reset to our timer modulo
                dbg() << "would be setting timer interrupt";
                emulator().cpu().set_interrupt_flag(InterruptFlags::Timer);
            } else {
                m_tima++;
            }
        }
    }
}

void Timer::update_divider_reg(u8 cycles)
{
    m_divider_counter += cycles;
    if (m_divider_counter >= 255) {
        m_divider_counter -= 255; // handle any overflow
        m_div++;
    }
}

bool Timer::has_interrupt()
{
    return m_has_pending_interrupt;
}

void Timer::reset_interrupt()
{
    m_has_pending_interrupt = false;
}

void Timer::set_clock_freq()
{
    u8 freq = get_clock_freq();
    switch (freq) {
    case 0: // frequency 4096
        m_timer_counter = 1024;
        break;
    case 1: // frequency 262,144
        m_timer_counter = 16;
        break;
    case 2: // frequency 65,536
        m_timer_counter = 64;
        break;
    case 3: // frequency 16,382
        m_timer_counter = 256;
        break;
    }
}

u8 Timer::get_clock_freq()
{
    return m_tac & 0x3;
}

bool Timer::is_clock_enabled()
{
    // enabled when bit 2 of the timer controller is turned on
    return m_tac & 0x2;
}
