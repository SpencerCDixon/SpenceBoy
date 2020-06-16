//
// Created by Spencer Dixon on 5/9/20.
//

#include "PPU.h"
#include "Emulator.h"

#include <SD/Assertions.h>
#include <SD/Bytes.h>

// Useful Notes:
// * A vertical refresh happens every 70224 clocks (140448 in GBC double speed mode): 59,7275 Hz
// * A scanline normally takes 456 clocks (912 clocks in double speed mode) to complete
// * Vertical Blank interrupt is triggered when the LCD controller enters the VBL screen mode (mode 1, LY=144).
//   This happens once per frame, so this interrupt is triggered 59.7 times per second.

constexpr size_t TILE_WIDTH = 32;
constexpr size_t TILE_HEIGHT = 32;
constexpr size_t TOTAL_BG_TILES = TILE_HEIGHT * TILE_WIDTH;

//constexpr size_t TILESET_WIDTH = 32;
//constexpr size_t TILESET_HEIGHT = 24;
//constexpr size_t TOTAL_TILESET_TILES = TILESET_HEIGHT * TILESET_WIDTH;

constexpr u16 R_LCDC = 0xff40;
constexpr u16 R_SCY = 0xff42;
constexpr u16 R_SCX = 0xff43;

Tile8x8::Tile8x8()
{
    memset(m_pixels, 0, sizeof(m_pixels));
}

void Tile8x8::populate_from_palette(const u8* buffer, const u32* palette)
{
    for (size_t row = 0; row < 8; row++) {
        // 0 1 | 2 3 | 4 5 | 6 7
        size_t lower_byte_idx = row * 2;
        size_t higher_byte_idx = lower_byte_idx + 1;

        u8 lower_byte = buffer[lower_byte_idx];
        u8 higher_byte = buffer[higher_byte_idx];

        for (size_t col = 0; col < 8; ++col) {
            u8 hi_bit = (higher_byte >> (7 - col)) & 1;
            u8 low_bit = (lower_byte >> (7 - col)) & 1;
            // 00 01 10 11
            u8 idx_into_palette = (hi_bit << 1) | low_bit;
            set_pixel(col, row, palette[idx_into_palette]);
        }
    }
}

void PPU::clear(const Color& color)
{
    u32 argb_color = color.to_argb();
    u8* row = (u8*)m_bitmap.data();
    for (int y = 0; y < m_bitmap.height(); ++y) {
        u32* pixel = (u32*)row;

        for (int x = 0; x < m_bitmap.width(); ++x)
            *pixel++ = argb_color;

        row += m_bitmap.pitch();
    }

    row = (u8*)m_tileset_bitmap.data();
    for (int y = 0; y < m_tileset_bitmap.height(); ++y) {
        u32* pixel = (u32*)row;

        for (int x = 0; x < m_tileset_bitmap.width(); ++x)
            *pixel++ = argb_color;

        row += m_tileset_bitmap.pitch();
    }
}

// TODO:
// * layered rendering (background, sprites, window)
// * draw_scanline() (helps with V/HBlank)

void PPU::fill_square(size_t x, size_t y, const Tile8x8& tile, Bitmap& bitmap)
{
    ASSERT(x < 32);
    ASSERT(y < 32);

    auto offset = bitmap.pitch() * (y * 8); // y-offset
    offset += sizeof(u32) * 8 * x;          // x-offset

    u8* start = (u8*)bitmap.data();
    start += offset;

    for (size_t y = 0; y < 8; y++) {
        u32* pixel = (u32*)start;
        for (size_t x = 0; x < 8; x++) {
            *pixel++ = tile.pixel(x, y);
        }

        start += bitmap.pitch();
    }
}

void PPU::render()
{
    if (!lcd_display_enabled()) {
        dbg() << "LCD/PPU are disabled, returning early";
        return;
    }

    size_t tile_start = 0;
    Tile8x8 tiles[TOTAL_BG_TILES];
    for (size_t i = 0; i < TOTAL_BG_TILES; ++i) {
        size_t idx = tile_start + (i * 16);
        auto* pointer = &emulator().mmu().vram()[idx];
        tiles[i].populate_from_palette(pointer, &m_palette[0]);
    }

    // A special chunk of memory starting at 0x9800 or 0x9c00 is used to map which tile index
    // should be used to render. Each byte represents the index into the tile map.
    size_t map_start = bg_tilemap_display_select() - 0x8000;
    for (size_t i = 0; i < TOTAL_BG_TILES; ++i) {
        size_t tile_idx = emulator().mmu().vram()[map_start + i];
        //        dbg() << "tile_idx: " << (u16)tile_idx;
        size_t x = i % 32;
        size_t y = i / 32;
        fill_square(x, y, tiles[tile_idx], m_bitmap);
    }

    Tile8x8 tileset[384];
    for (size_t i = 0; i < 384; ++i) {
        size_t idx = i * 16; // 16 bytes (8 x 8 x 2 bpp)
        auto* pointer = &emulator().mmu().vram()[idx];
        tileset[i].populate_from_palette(pointer, &m_palette[0]);
    }
    for (size_t i = 0; i < 384; ++i) {
        size_t x = i % 32;
        size_t y = i / 32;
        fill_square(x, y, tileset[i], m_tileset_bitmap);
    }

    m_tilemap.set_data(m_bitmap);
    m_tileset.set_data(m_tileset_bitmap);
}

u8 PPU::in(u16 address)
{
    dbg() << "PPU::in() " << to_hex(address);

    if (address == R_SCX) {
        dbg() << "  handled!";
        return m_bg_scroll_x;
    }

    if (address == R_SCY) {
        dbg() << "  handled!";
        return m_bg_scroll_y;
    }

    if (address == R_LCDC)
        return m_lcd_control;

    // TODO: LCD Stat
    if (address == 0xff44) {
        return 145;
    }

    return 0;
}

void PPU::out(u16 address, u8 value)
{
    // TODO:
    //    PPU::out() 0xff47 - BGP - BG Palette Data (R/W) - Non CGB Mode Only
    //    PPU::out() 0xff40 - LCDC Status Interrupt
    //    PPU::out(0xff42, 0x0)
    //    PPU::out(0xff43, 0x0)
    //    PPU::out(0xff26, 0x0)
    dbg() << "PPU::out(" << to_hex(address) << ", " << to_hex(value) << ")";

    switch (address) {
    case R_SCX:
        dbg() << "  scx written";
        m_bg_scroll_x = value;
        break;
    case R_SCY:
        dbg() << "  scy written";
        m_bg_scroll_y = value;
        break;
    case R_LCDC:
        dbg() << "  lcd control written";
        m_lcd_control = value;
        break;
    default:
        break;
    }
}

const LogStream& operator<<(const LogStream& stream, const Tile8x8& tile)
{
    for (size_t row = 0; row < 8; ++row) {
        for (size_t col = 0; col < 8; ++col) {
            u32 pixel = tile.pixel(col, row);
            if (pixel == Color::BLACK_ARGB) {
                stream.write(" B ", 3);
            } else {
                stream.write(" W ", 3);
            }
        }
        stream.write("\n", 1);
    }
    return stream;
}