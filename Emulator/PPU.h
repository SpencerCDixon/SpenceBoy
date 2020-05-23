//
// Created by Spencer Dixon on 5/9/20.
//

#pragma once

#include <SD/LogStream.h>
#include <SD/Types.h>
#include <SD/Color.h>

#include "Buffer.h"

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
    void render_background_tiles();
    void fill_square(size_t x, size_t y, u32 color);

private:
    void render_sprite(u8* src_dest, const u8* sprite);

private:
    const u8* m_vram;
    OffscreenFrameBuffer* m_buffer;
};
