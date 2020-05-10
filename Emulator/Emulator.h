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

class Emulator {
public:
    Emulator()
        : m_cpu(CPU())
        , m_ppu(PPU())
    {
        m_frame_buffer = (u32*)calloc(WIN_WIDTH * WIN_HEIGHT, sizeof(u32));
    }

    ~Emulator() {
        if (m_frame_buffer)
            free(m_frame_buffer);
    }

    void init();
    void load_rom(const char* path);
    void run();

private:
    CPU m_cpu;
    PPU m_ppu;

    SDL_Window* m_window { nullptr };
    SDL_Renderer* m_renderer { nullptr };
    SDL_Texture* m_gb_screen { nullptr };
    u32* m_frame_buffer { nullptr };
};
