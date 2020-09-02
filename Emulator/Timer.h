//
// Created by Spencer Dixon on 6/13/20.
//

#pragma once

#include "IODevice.h"
#include <SD/Types.h>

// Timer: http://imrannazar.com/GameBoy-Emulation-in-JavaScript:-Timers
//
// ff04 - divider - count up at 16384HZ
// ff05 - counter - trigger INT 0x50 when going 255->0
// ff06 - modulo - when counter resets from overflow 0, it gets set to this
// ffo7 - control
//   0-1 - speed
//   00 - 4096HZ
//   01 - 262144Hz
//   10 - 65536Hz
//   11 - 16384Hz
//
//   2 - 1 to run, 0 to turn it off
class Emulator;

class Timer final : public IODevice {
public:
    Timer(Emulator& emulator);

    // IODevice
    u8 in(u16 address) override;
    void out(u16 address, u8 value) override;

public:
    void update_by(u8 cycles);

private:
    Emulator& emulator() { return m_emulator; }
    void update_divider_reg(u8 cycles);
    void set_clock_freq();
    u8 get_clock_freq();
    bool is_clock_enabled();

private:
    Emulator& m_emulator;
    // Registers
    u8 m_div { 0 };  // constant timer counter
    u8 m_tima { 0 }; // programmable timer counter
    u8 m_tma { 0 };  // timer modulo (what tima get set to when overflow)
    u8 m_tac { 0 };  // timer controller

    s32 m_timer_counter { 1024 }; // Defaults to 4096
    s32 m_divider_counter { 0 };  // Defaults to 0, counts up at freq of 16382
};
