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

    inline SDL_Rect to_sdl() const
    {
        return SDL_Rect { this->x, this->y, this->width, this->height };
    }
};

struct Size {
    int width;
    int height;
};