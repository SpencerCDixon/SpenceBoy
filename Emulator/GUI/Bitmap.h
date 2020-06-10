//
// Created by Spencer Dixon on 5/14/20.
//

#pragma once

#include <SD/Assertions.h>

#include "Emulator/GUI/Rect.h"

// TODO: pixel formats?

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

private:
    int m_height;
    int m_width;
    int m_pitch;
    void* m_data { nullptr };
};