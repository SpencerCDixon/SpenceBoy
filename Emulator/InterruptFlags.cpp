#include "InterruptFlags.h"

u16 jump_vector_for(InterruptFlags flag)
{
    switch (flag) {
    case InterruptFlags::None:
        return 0;
    case InterruptFlags::VBlank:
        return 0x40;
    case InterruptFlags::LCDStat:
        return 0x48;
    case InterruptFlags::Timer:
        return 0x50;
    case InterruptFlags::Serial:
        return 0x58;
    case InterruptFlags::Joypad:
        return 0x60;
    }
}

bool is_on(InterruptFlags value, InterruptFlags to_test)
{
    return (int)(value & to_test) > 0;
}

InterruptFlags unset(InterruptFlags value, InterruptFlags to_turn_off)
{
    auto num = static_cast<u8>(value);
    num &= ~(static_cast<u8>(to_turn_off));
    return static_cast<InterruptFlags>(num);
}
