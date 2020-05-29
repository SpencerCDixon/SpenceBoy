//
// Created by Spencer Dixon on 5/9/20.
//

#include "PPU.h"
#include <SD/Assertions.h>

// Useful Notes:
// * A vertical refresh happens every 70224 clocks (140448 in GBC double speed mode): 59,7275 Hz
// * A scanline normally takes 456 clocks (912 clocks in double speed mode) to complete
// * Vertical Blank interrupt is triggered when the LCD controller enters the VBL screen mode (mode 1, LY=144).
//   This happens once per frame, so this interrupt is triggered 59.7 times per second.
Tile8x8::Tile8x8()
{
}

void Tile8x8::populate_from_palette(const u8* buffer)
{
    local_persist u8 black_mask = 0x2;

    for (size_t row = 0; row < 8; ++row) {
        u16 row_bytes = buffer[row];
        row_bytes = (row_bytes << 8) | buffer[row + 1];

        for (size_t col = 0; col < 8; ++col) {
            // 2 bits for pixel. 11 = black anything else = white (for now)
            bool is_black = (row_bytes >> col * 2) & black_mask;
            set_pixel(col, row, is_black ? Color::BLACK_ARGB : Color::WHITE_ARGB);
        }
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

void PPU::clear(Color color)
{
    u32 arg_color = color.to_argb();
    u8* row = (u8*)m_buffer->memory;
    for (int y = 0; y < m_buffer->height; ++y) {
        u32* pixel = (u32*)row;

        for (int x = 0; x < m_buffer->width; ++x) {
            *pixel++ = arg_color;
        }

        row += m_buffer->pitch;
    }
}

// TODO:
// * palette matching
// * layered rendering (background, sprites, window)
// * remove hard coded smiley tile and make it general purpose

void PPU::render()
{
    size_t tile_start = 0x9000 - 0x8000;
    Tile8x8 tiles[4];
    for (size_t i = 0; i < 4; ++i) {
        tiles[i].populate_from_palette(&m_vram[tile_start + (i * 16)]);
    }

    for (size_t i = 0; i < 4; ++i) {
        dbg() << tiles[i];
        fill_square(i, 0, tiles[i]);
    }
}

void PPU::fill_square(size_t x, size_t y, const Tile8x8& tile)
{
    ASSERT(x < 32);
    ASSERT(y < 32);

    // Y-Offset
    auto offset = m_buffer->pitch * (y * 8);
    // X-Offset
    offset += sizeof(u32) * 8 * x;

    u8* start = (u8*)m_buffer->memory;
    start += offset;

    for (size_t y = 0; y < 8; y++) {
        u32* pixel = (u32*)start;
        for (size_t x = 0; x < 8; x++) {
            *pixel++ = tile.pixel(x, y);
        }

        start += m_buffer->pitch;
    }
}
