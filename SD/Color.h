//
// Created by Spencer Dixon on 5/3/20.
//

#pragma once

#include <SD/Types.h>
#include <stdint.h>

struct Color {
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    u32 to_argb()
    {
        return this->a << 24 | this->r << 16 | this->g << 8 | this->b;
    }

    constexpr u32 to_argb_compile()
    {
        return this->a << 24 | this->r << 16 | this->g << 8 | this->b;
    }
};

// Andreas: Is there a cleaner way of having compile time constants like this that are namespaced and such?
namespace Colors {

static constexpr u32 BLACK_ARGB = Color { 0, 0, 0, 255 }.to_argb_compile();
static constexpr u32 WHITE_ARGB = Color { 255, 255, 255, 255 }.to_argb_compile();
static constexpr u32 RED_ARGB = Color { 255, 0, 0, 255 }.to_argb_compile();

};
