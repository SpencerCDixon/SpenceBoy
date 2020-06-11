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

Tile8x8::Tile8x8()
{
    memset(m_pixels, 0, sizeof(m_pixels));
}

void Tile8x8::populate_from_palette(const u8* buffer)
{
    local_persist u8 black_mask = 0x3;

    // Andreas: Maybe there is a better way to do this? I need to iterate in pairs of two bytes
    for (size_t row = 0; row < 8; row++) {
        size_t idx_one = row * 2;
        size_t idx_two = idx_one + 1;

        u16 row_bytes = buffer[idx_one];
        row_bytes = (row_bytes << 8) | buffer[idx_two];

        for (size_t col = 0; col < 8; ++col) {
            // FIXME: Pick from different colors.
            // 2 bits for pixel. 11 = black anything else = white (for now)
            bool is_black = (row_bytes >> col * 2) & black_mask;
            set_pixel(col, row, is_black ? Color::BLACK_ARGB : Color::TAN_ARGB);
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
}

// TODO:
// * palette matching
// * layered rendering (background, sprites, window)
// * remove hard coded smiley tile and make it general purpose
// * draw_scanline() (helps with V/HBlank)

void PPU::render()
{
    // TODO: Tile start can change based on a bit flag. Will need to adjust accordingly.
    size_t tile_start = 0;
    Tile8x8 tiles[TOTAL_BG_TILES];
    for (size_t i = 0; i < TOTAL_BG_TILES; ++i) {
        size_t idx = tile_start + (i * 16);
        auto* pointer = &emulator().mmu().vram()[idx];
        tiles[i].populate_from_palette(pointer);
    }

    // A special chunk of memory starting at 0x9800 is used to map which tile index
    // should be used to render. Each byte represents the index into the tile map.
    size_t map_start = 0x9800 - 0x8000; // 0x8000 will be dynamic based on bit flag

    for (size_t i = 0; i < TOTAL_BG_TILES; ++i) {
        size_t tile_idx = emulator().mmu().vram()[map_start + i];
        size_t x = i % 32;
        size_t y = i / 32;
        fill_square(x, y, tiles[tile_idx]);
    }
}

void PPU::fill_square(size_t x, size_t y, const Tile8x8& tile)
{
    ASSERT(x < 32);
    ASSERT(y < 32);

    // Y-Offset
    auto offset = m_bitmap.pitch() * (y * 8);
    // X-Offset
    offset += sizeof(u32) * 8 * x;

    u8* start = (u8*)m_bitmap.data();
    start += offset;

    for (size_t y = 0; y < 8; y++) {
        u32* pixel = (u32*)start;
        for (size_t x = 0; x < 8; x++) {
            *pixel++ = tile.pixel(x, y);
        }

        start += m_bitmap.pitch();
    }
}

u8 PPU::in(u16 address)
{
    dbg() << "PPU::in() " << to_hex(address);
    return 0;
}

void PPU::out(u16 address, [[maybe_unused]] u8 value)
{
    // TODO:
//    PPU::out() 0xff47 - BGP - BG Palette Data (R/W) - Non CGB Mode Only
//    PPU::out() 0xff40 - LCDC Status Interrupt

    dbg() << "Unhandled PPU::out() " << to_hex(address);
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