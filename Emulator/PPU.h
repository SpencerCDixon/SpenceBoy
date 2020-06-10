//
// Created by Spencer Dixon on 5/9/20.
//

#pragma once

#include <SD/Color.h>
#include <SD/LogStream.h>
#include <SD/Types.h>

#include "Buffer.h"
#include "IODevice.h"

class Emulator;

// FIXME: This is the size that gets displayed. Before getting game rendering working I want to
// get tile map data showing properly (which uses 32x32 (256x256))
//constexpr u16 WIN_HEIGHT = 144;
//constexpr u16 WIN_WIDTH = 160;
constexpr u16 GB_WIN_HEIGHT = 256;
constexpr u16 GB_WIN_WIDTH = 256;

constexpr u16 BITS_PER_PIXEL = sizeof(u32);

class Tile8x8 {
public:
    Tile8x8();
    Tile8x8(const u8* buffer)
    {
        memset(m_pixels, 0, sizeof(m_pixels));
        populate_from_palette(buffer);
    }

    inline u32 pixel(size_t x, size_t y) const
    {
        size_t idx = y * 8 + x;
        //        ASSERT(idx >= 0 && idx <= 63);
        return m_pixels[idx];
    }

    void set_pixel(size_t x, size_t y, u32 color)
    {
        size_t idx = y * 8 + x;
        //        ASSERT(idx >= 0 && idx <= 63);
        m_pixels[idx] = color;
    }

    void populate_from_palette(const u8* buffer);

private:
    u32 m_pixels[64];
};

// VRAM is never going to move, cache that pointer in the PPU.
class PPU final : public IODevice {
public:
    PPU(Emulator& emulator)
        : m_emulator(emulator)
        , m_bitmap({ (void*)calloc(GB_WIN_WIDTH * GB_WIN_HEIGHT, BITS_PER_PIXEL),
              GB_WIN_HEIGHT,
              GB_WIN_WIDTH,
              GB_WIN_WIDTH * BITS_PER_PIXEL,
              BITS_PER_PIXEL })
    {
    }

    // IODevice
    u8 in(u16 address) override;
    void out(u16 address, u8 value) override;

    OffscreenFrameBuffer& bitmap() { return m_bitmap; }
    void clear(Color color);
    void render();
    void fill_square(size_t x, size_t y, const Tile8x8& tile);

private:
    Emulator& emulator() { return m_emulator; }

private:
    Emulator& m_emulator;
    OffscreenFrameBuffer m_bitmap;
};

const LogStream& operator<<(const LogStream&, const Tile8x8&);