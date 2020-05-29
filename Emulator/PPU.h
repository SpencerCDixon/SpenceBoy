//
// Created by Spencer Dixon on 5/9/20.
//

#pragma once

#include <SD/Color.h>
#include <SD/LogStream.h>
#include <SD/Types.h>

#include "Buffer.h"

class Tile8x8 {
public:
    Tile8x8();
    Tile8x8(const u8* buffer)
    {
        populate_from_palette(buffer);
    }

    inline u32 pixel(size_t x, size_t y) const
    {
        size_t idx = y * 8 + x;
        ASSERT(idx >= 0 && idx <= 63);
        return m_pixels[idx];
    }

    void set_pixel(size_t x, size_t y, u32 color)
    {
        size_t idx = y * 8 + x;
        ASSERT(idx >= 0 && idx <= 63);
        m_pixels[idx] = color;
    }

    void populate_from_palette(const u8* buffer);

private:
    u32 m_pixels[64];
};

class PPU {
public:
    PPU(const u8* vram, OffscreenFrameBuffer* buffer)
        : m_vram(vram)
        , m_buffer(buffer)
    {

        dbg() << "using vram address of: " << &m_vram;
        dbg() << "using buffer address of: " << &m_buffer;
    }

    void clear(Color color);
    void render();
    void fill_square(size_t x, size_t y, const Tile8x8& tile);

private:
    const u8* m_vram;
    OffscreenFrameBuffer* m_buffer;
};

const LogStream& operator<<(const LogStream&, const Tile8x8&);