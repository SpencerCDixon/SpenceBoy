//
// Created by Spencer Dixon on 5/9/20.
//

#pragma once

#include <SD/Color.h>
#include <SD/LogStream.h>
#include <SD/Types.h>

#include "IODevice.h"
#include "Buffer.h"

class Emulator;

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
    PPU(Emulator& emulator, OffscreenFrameBuffer* buffer)
        : m_emulator(emulator)
        , m_buffer(buffer)
    {
    }

    // IODevice
    u8 in(u16 address) override;
    void out(u16 address, u8 value) override;

    void clear(Color color);
    void render();
    void fill_square(size_t x, size_t y, const Tile8x8& tile);
private:
    Emulator& emulator() { return m_emulator; }

private:
    Emulator& m_emulator;
    // m_vram
    OffscreenFrameBuffer* m_buffer;
};

const LogStream& operator<<(const LogStream&, const Tile8x8&);