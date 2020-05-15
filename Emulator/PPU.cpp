//
// Created by Spencer Dixon on 5/9/20.
//

#include "PPU.h"

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
    size_t smiley_start = 0x9010 - 0x8000;

    u8 smiley[16];

    for (size_t i = 0; i < 16; ++i) {
        smiley[i] = m_vram[smiley_start + i];
    }

    // Rendering a tile:
    // * 16 bytes: 8 pixels x 8 pixels x 2 bits per pixel
    // * It’s stored in lines, so the first byte is the first bit of the first line;
    // the second byte is the second bit of the first line; the third byte is the first bit of the second line, etc.

    u8* row = (u8*)m_buffer->memory;
    u8* color_row = smiley;
    u8 bytes_per_pixel = 2;

    for (int y = 0; y < 8; ++y) {
        u32* pixel = (u32*)row;

        for (int x = 0; x < 8; ++x) {
            u8 first_byte = color_row[0];
            u8 second_byte = color_row[1];

            // 00 01 10 = white AND 11 = black
            bool is_black = (first_byte & (1 << x)) && (second_byte & (1 << x));

            *pixel++ = is_black ? Colors::BLACK_ARGB : Colors::WHITE_ARGB;
        }

        color_row += bytes_per_pixel;
        row += m_buffer->pitch;
    }
}
