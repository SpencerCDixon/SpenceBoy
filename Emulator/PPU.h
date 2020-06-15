//
// Created by Spencer Dixon on 5/9/20.
//

#pragma once

#include <SD/Color.h>
#include <SD/LogStream.h>
#include <SD/Types.h>
#include <SD/Utility.h>

#include "Emulator/GUI/Bitmap.h"
#include "Emulator/GUI/Texture.h"
#include "IODevice.h"

class Emulator;

// FIXME: This is the size that gets displayed. Before getting game rendering working I want to
// get tile map data showing properly (which uses 32x32 (256x256))
//constexpr u16 WIN_HEIGHT = 144;
//constexpr u16 WIN_WIDTH = 160;
constexpr u16 GB_WIN_HEIGHT = 256;
constexpr u16 GB_WIN_WIDTH = 256;
constexpr u16 TILESET_WIN_HEIGHT = 192;
constexpr u16 TILESET_WIN_WIDTH = 256;

class Tile8x8 {
public:
    Tile8x8();

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

    void populate_from_palette(const u8* buffer, const u32* palette);
private:
    u32 m_pixels[64];
};

// TODO: VRAM is never going to move, cache that pointer in the PPU.
class PPU final : public IODevice {
public:
    PPU(Emulator& emulator)
        : m_emulator(emulator)
        , m_bitmap({ GB_WIN_WIDTH, GB_WIN_HEIGHT }, GB_WIN_WIDTH * BITS_PER_PIXEL)
        , m_tileset_bitmap({ TILESET_WIN_WIDTH, TILESET_WIN_HEIGHT }, TILESET_WIN_WIDTH * BITS_PER_PIXEL)
    {
    }

    void init_textures()
    {
        m_tilemap = Texture::from_size({ GB_WIN_WIDTH, GB_WIN_HEIGHT }, TextureUsage::Streaming);
        m_tileset = Texture::from_size({ TILESET_WIN_WIDTH, TILESET_WIN_HEIGHT }, TextureUsage::Streaming);
    }

    // IODevice
    u8 in(u16 address) override;
    void out(u16 address, u8 value) override;

    Texture& tilemap() { return m_tilemap; }
    Texture& tileset() { return m_tileset; }

    void clear(const Color& color);
    void render();
    void fill_square(size_t x, size_t y, const Tile8x8& tile, Bitmap& bitmap);

private:
    Emulator& emulator() { return m_emulator; }

private:
    Emulator& m_emulator;
    Bitmap m_bitmap;
    Bitmap m_tileset_bitmap;

    Texture m_tilemap;
    Texture m_tileset;

    u32 m_palette[4] = {
        Color::TAN_ARGB,
        Color::LIGHT_GRAY_ARGB,
        Color::DARK_GRAY_ARGB,
        Color::BLACK_ARGB
    };

    u8 m_bg_scroll_y { 0 };
    u8 m_bg_scroll_x { 0 };
};

const LogStream& operator<<(const LogStream&, const Tile8x8&);