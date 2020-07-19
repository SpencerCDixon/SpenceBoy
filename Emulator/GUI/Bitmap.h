//
// Created by Spencer Dixon on 5/14/20.
//

#pragma once

#include <SD/Assertions.h>
#include <SD/Types.h>

#include "Emulator/GUI/Rect.h"

// TODO: more robust pixel formats?
constexpr u16 BITS_PER_PIXEL = sizeof(u32);

class Bitmap {
public:
    Bitmap(const Size& size, int pitch)
        : m_height(size.height)
        , m_width(size.width)
        , m_pitch(pitch)
    {
        m_data = (void*)calloc(size.width * size.height, BITS_PER_PIXEL);
        ASSERT(m_data != nullptr);
    }

    ~Bitmap()
    {
        free(m_data);
    }

    void* data() { return m_data; }
    const void* data() const { return m_data; }
    int height() { return m_height; }
    int width() { return m_width; }
    int pitch() { return m_pitch; }
    int pitch() const { return m_pitch; }

    void set_pixel_to(size_t x, size_t y, u32 color)
    {
        ASSERT(x < (size_t)width());
        ASSERT(y < (size_t)height());
        auto offset = (BITS_PER_PIXEL * x) + (pitch() * y);
        u8* ptr = (u8*)m_data;
        ptr += offset;
        *((u32*)ptr) = color;
    }

    void set_all_pixels_to(u32 color)
    {
        u8* row = (u8*)data();
        for (int y = 0; y < height(); ++y) {
            u32* pixel = (u32*)row;

            for (int x = 0; x < width(); ++x)
                *pixel++ = color;

            row += pitch();
        }
    }

private:
    int m_height;
    int m_width;
    int m_pitch;
    void* m_data { nullptr };
};
