//
// Created by spence on 8/9/20.
//

#pragma once

#include "SD/Types.h"

enum class InterruptFlags : u8 {
    None = 0,
    VBlank = 1 << 0,
    LCDStat = 1 << 1,
    Timer = 1 << 2,
    Serial = 1 << 3,
    Joypad = 1 << 4,
};

u16 jump_vector_for(InterruptFlags);
bool is_on(InterruptFlags value, InterruptFlags to_test);
InterruptFlags unset(InterruptFlags value, InterruptFlags to_turn_off);

constexpr enum InterruptFlags operator|(const enum InterruptFlags selfValue, const enum InterruptFlags inValue)
{
    return (enum InterruptFlags)(u8(selfValue) | u8(inValue));
}
constexpr enum InterruptFlags operator&(const enum InterruptFlags selfValue, const enum InterruptFlags inValue)
{
    return (enum InterruptFlags)(u8(selfValue) & u8(inValue));
}