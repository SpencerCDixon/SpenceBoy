//
// Created by Spencer Dixon on 6/9/20.
//

#pragma once

#include "Emulator/InternalSDL.h"

struct Rect {
    int x;
    int y;
    int width;
    int height;

    // ACall: Is there a better strategy for these types of conversions? In Rust, there is an Into<T> pattern I like
    inline SDL_Rect to_sdl() const
    {
        return SDL_Rect { this->x, this->y, this->width, this->height };
    }
};

struct Point {
    int x;
    int y;
};

struct Size {
    int width;
    int height;
};