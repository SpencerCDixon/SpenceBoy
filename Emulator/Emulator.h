//
// Created by Spencer Dixon on 5/10/20.
//

#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#include <SDL.h>
#pragma clang diagnostic pop

#include "CPU.h"
#include "PPU.h"

constexpr u16 WIN_HEIGHT = 144;
constexpr u16 WIN_WIDTH = 160;
constexpr u16 BITS_PER_PIXEL = sizeof(u32);

struct OffscreenFrameBuffer {
    void* memory;
    int height;
    int width;
    int pitch;
};

class Emulator {
public:
    Emulator()
        : m_cpu(CPU())
        , m_ppu(PPU())
    {
        m_frame_buffer = OffscreenFrameBuffer { 0 };

        m_frame_buffer.memory = (void*)calloc(WIN_WIDTH * WIN_HEIGHT, BITS_PER_PIXEL);
        m_frame_buffer.height = WIN_HEIGHT;
        m_frame_buffer.width = WIN_WIDTH;
        m_frame_buffer.pitch = WIN_WIDTH * BITS_PER_PIXEL;
    }

    // Andreas: Is this a good way to be doing memory management? It sort of seems like at this point in the program cleaning up
    // memory is kind of redundant. A leak is probably OK since when the process dies it will just be cleaned up
    ~Emulator() {
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
    PPU m_ppu;

    SDL_Window* m_window { nullptr };
    SDL_Renderer* m_renderer { nullptr };
    SDL_Texture* m_gb_screen { nullptr };
    OffscreenFrameBuffer m_frame_buffer;
};
