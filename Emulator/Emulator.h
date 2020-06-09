//
// Created by Spencer Dixon on 5/10/20.
//

#pragma once

#include "Buffer.h"
#include "CPU.h"
#include "InternalSDL.h"
#include "Joypad.h"
#include "MMU.h"
#include "PPU.h"

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
        , m_mmu({})
        , m_joypad({})
        , m_cpu(*this, verbose_logging)
        , m_ppu( *this, &m_frame_buffer)

    {
        // TODO: 2 step init process
        // mmu->init_io_devices
    }

    ~Emulator()
    {
        if (m_frame_buffer.memory)
            free(m_frame_buffer.memory);
    }

    void init();
    void load_rom(const char* path);
    void run();

    MMU& mmu() { return m_mmu; }
    Joypad& joypad() { return m_joypad; }
    PPU& ppu() { return m_ppu; }
    CPU& cpu() { return m_cpu; }

private:
    void swap();

private:
    OffscreenFrameBuffer m_frame_buffer;
    MMU m_mmu;
    Joypad m_joypad;
    CPU m_cpu;
    PPU m_ppu;

    SDL_Window* m_window { nullptr };
    SDL_Renderer* m_renderer { nullptr };
    SDL_Texture* m_gb_screen { nullptr };
};
