//
// Created by Spencer Dixon on 5/3/20.
//

#pragma once

#include <SD/Types.h>
#include <stdint.h>

struct Color {
    // clang-format off
    static constexpr u32 TAN_ARGB        = 0xffDCDFD1;
    static constexpr u32 BLACK_ARGB      = 0xff000000;
    static constexpr u32 DARK_GRAY_ARGB  = 0xff555555;
    static constexpr u32 LIGHT_GRAY_ARGB = 0xffAAAAAA;
    static constexpr u32 WHITE_ARGB      = 0xffffffff;
    static constexpr u32 RED_ARGB        = 0xffff0000;
    // clang-format on

    u8 r;
    u8 g;
    u8 b;
    u8 a;

    constexpr u32 to_argb() const
    {
        return this->a << 24 | this->r << 16 | this->g << 8 | this->b;
    }
};