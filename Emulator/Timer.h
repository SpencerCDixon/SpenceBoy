//
// Created by Spencer Dixon on 6/13/20.
//

#pragma once

#include "IODevice.h"

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

class Timer final : public IODevice {
public:
    u8 in(u16 address) override;
    void out(u16 address, u8 value) override;
};
