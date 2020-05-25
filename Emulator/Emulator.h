//
// Created by Spencer Dixon on 5/10/20.
//

#pragma once

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#include <SDL.h>
#pragma clang diagnostic pop
#else
#include <SDL.h>
#endif

#include "Buffer.h"
#include "CPU.h"
#include "PPU.h"

// FIXME: This is the size that gets displayed. Before getting game rendering working I want to
// get tile map data showing properly (which uses 32x32 (256x256))
//constexpr u16 WIN_HEIGHT = 144;
//constexpr u16 WIN_WIDTH = 160;

constexpr u16 WIN_HEIGHT = 256;
constexpr u16 WIN_WIDTH = 256;

constexpr u16 BITS_PER_PIXEL = sizeof(u32);

class Emulator {
public:
    Emulator()
        : m_cpu(CPU(true))
        , m_frame_buffer({ (void*)calloc(WIN_WIDTH * WIN_HEIGHT, BITS_PER_PIXEL),
              WIN_HEIGHT,
              WIN_WIDTH,
              WIN_WIDTH * BITS_PER_PIXEL,
              BITS_PER_PIXEL })
        , m_ppu(m_cpu.v_ram(), &m_frame_buffer)
    {
    }

    // Andreas: Is this a good way to be doing memory management? It sort of seems like at this point in the program cleaning up
    // memory is kind of redundant. A leak is probably OK since when the process dies it will just be cleaned up
    ~Emulator()
    {
        if (m_frame_buffer.memory)
            free(m_frame_buffer.memory);
    }

    void init();
    void load_rom(const char* path);
    void run();

private:
    void swap();

private:
    CPU m_cpu;
    OffscreenFrameBuffer m_frame_buffer;
    PPU m_ppu;

    SDL_Window* m_window { nullptr };
    SDL_Renderer* m_renderer { nullptr };
    SDL_Texture* m_gb_screen { nullptr };
};
