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

void PPU::clear(Color color)
{
    u8* row = (u8*)m_buffer->memory;
    for (int y = 0; y < m_buffer->height; ++y) {
        u32* pixel = (u32*)row;

        for (int x = 0; x < m_buffer->width; ++x) {
            *pixel++ = color.to_argb();
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
    size_t tile1 = 0x9010 - 0x8000;
    u8 sprite[16];

    for (size_t i = 0; i < 16; ++i) {
        sprite[i] = m_vram[tile1 + i];
    }

    // Rendering a tile:
    // * 16 bytes: 8 pixels x 8 pixels x 2 bits per pixel
    // * It’s stored in lines, so the first byte is the first bit of the first line;
    // the second byte is the second bit of the first line; the third byte is the first bit of the second line, etc.

    // $ff $ff
    // 1111 1111 1111 1111
    // 11 11 11 11  11 11 11 11
    // b  b  b  b   b  b  b  b

    u8* row = (u8*)m_buffer->memory;
    u8* color_row = sprite;
    u8 bits_per_pixel = 2;

    for (int y = 0; y < 8; ++y) {
        u32* pixel = (u32*)row;

        for (int x = 0; x < 8; ++x) {
            u8 first_byte = color_row[0];
            u8 second_byte = color_row[1];

            // FIXME: I don't think this works as I exepct it is :/
            // 00 01 10 = white AND 11 = black
            bool is_black = (first_byte & (1 << x)) && (second_byte & (1 << x));

            *pixel++ = is_black ? Color::BLACK_ARGB : Color::WHITE_ARGB;
        }

        color_row += bits_per_pixel;
        row += m_buffer->pitch;
    }
}

void PPU::render_background_tiles()
{
    // FIXME: get proper start location of bg tile data based on the IO Flag that determines it.
    [[maybe_unused]] size_t start = 0x9000 - 0x8000;

    constexpr size_t MAX_TILES = 2;

    // Start with just first row for now...
    u8* sprite_src = (u8*)m_buffer->memory;
    for (size_t x = 0; x < MAX_TILES; ++x) {
        const u8* sprite = m_vram + start + (x * 16);
        render_sprite(sprite_src, sprite);
        sprite_src += 16;
    }
}

void PPU::render_sprite(u8* src_dest, const u8* sprite)
{
    // TODO: Use fill_square() as an example of how to approach this
}

void PPU::fill_square(size_t x, size_t y, u32 color) {
    ASSERT(x < 32);
    ASSERT(y < 32);

    // Y-Offset
    auto offset = m_buffer->pitch * (y * 8);
    // X-Offset
    offset += sizeof(u32) * 8 * x;

    u8* start = (u8*)m_buffer->memory;
    start += offset;

    for (int y = 0; y < 8; y++) {
        u32* pixel = (u32*)start;
        for (int x = 0; x < 8; x++) {
            *pixel++ = color;
        }

        start += m_buffer->pitch;
    }
}
