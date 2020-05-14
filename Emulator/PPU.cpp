//
// Created by Spencer Dixon on 5/9/20.
//

#include "PPU.h"

// TODO:

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

void PPU::render()
{
    size_t smiley_start = 0x9010 - 0x8000;

    u8 smiley[16];

    for (size_t i = 0; i < 16; ++i) {
        smiley[i] = m_vram[smiley_start + i];
    }

    dbg() << "render() " << smiley;

    // row * width + column
    // y * 8 + x

    // Rendering a tile:
    // * 16 bytes: 8 pixels x 8 pixels x 2 bits per pixel
    // * It’s stored in lines, so the first byte is the first bit of the first line; the second byte is the second bit of the first line; the third byte is the first bit of the second line,

    auto black = Color{0, 0, 0, 255};
    auto white = Color{255, 255, 255, 255};


    u8* row = (u8*)m_buffer->memory;
    u8* color_row = smiley;


    for (int y = 0; y < 8; ++y) {
        u32* pixel = (u32*)row;

        for (int x = 0; x < 8; ++x) {
            u8 first_byte = color_row[0];
            u8 second_byte = color_row[1];

            bool is_black = (first_byte & (1 << x)) && (second_byte & (1 << x));

            *pixel++ = is_black ? black.to_argb() : white.to_argb();
        }

        color_row += 2;
        row += m_buffer->pitch;
    }


}
