//
// Created by Spencer Dixon on 5/10/20.
//

#pragma once

#ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#    include <SDL.h>
#    pragma clang diagnostic pop
#else
#    include <SDL.h>
#endif

#include "Buffer.h"
#include "CPU.h"
#include "Joypad.h"
#include "PPU.h"
#include "MMU.h"

// FIXME: This is the size that gets displayed. Before getting game rendering working I want to
// get tile map data showing properly (which uses 32x32 (256x256))
//constexpr u16 WIN_HEIGHT = 144;
//constexpr u16 WIN_WIDTH = 160;

constexpr u16 GB_WIN_HEIGHT = 256;
constexpr u16 GB_WIN_WIDTH = 256;

constexpr u16 BITS_PER_PIXEL = sizeof(u32);

class Emulator {
public:
    Emulator(bool verbose_logging = false)
        : m_frame_buffer({ (void*)calloc(GB_WIN_WIDTH * GB_WIN_HEIGHT, BITS_PER_PIXEL),
            GB_WIN_HEIGHT,
            GB_WIN_WIDTH,
            GB_WIN_WIDTH * BITS_PER_PIXEL,
            BITS_PER_PIXEL })
    {

        m_mmu = new MMU;
        m_joypad = new Joypad;
        m_ppu = new PPU(*this, &m_frame_buffer);
        m_cpu = new CPU(*this, verbose_logging);
    }

    ~Emulator()
    {
        if (m_frame_buffer.memory)
            free(m_frame_buffer.memory);

        delete m_mmu;
        delete m_cpu;
        delete m_ppu;
        delete m_joypad;
    }

    void init();
    void load_rom(const char* path);
    void run();

    MMU& mmu() { return *m_mmu; }
    Joypad& joypad() { return *m_joypad; }
    PPU& ppu() { return *m_ppu; }
    CPU& cpu() { return *m_cpu; }

private:
    void swap();

private:
    OffscreenFrameBuffer m_frame_buffer;
    MMU* m_mmu { nullptr };
    CPU* m_cpu { nullptr };
    PPU* m_ppu { nullptr };
    Joypad* m_joypad { nullptr };

    SDL_Window* m_window { nullptr };
    SDL_Renderer* m_renderer { nullptr };
    SDL_Texture* m_gb_screen { nullptr };
};
