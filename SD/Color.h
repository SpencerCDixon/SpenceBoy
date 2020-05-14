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

    u32 to_argb() {
        return this->a << 24 | this->r << 16 | this->g << 8 | this->b;
    }
};