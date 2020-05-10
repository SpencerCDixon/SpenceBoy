//
// Created by Spencer Dixon on 5/10/20.
//

#include "Emulator.h"
#include <SD/Assertions.h>
#include <SD/Color.h>

void Emulator::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        error("unable to initialize SDL video subsystem");
    }

    if (SDL_CreateWindowAndRenderer(WIN_HEIGHT, WIN_WIDTH, 0, &m_window, &m_renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        error("creating window and renderer");
    }

    SDL_SetWindowTitle(m_window, "SpenceBoy");
    SDL_RenderSetLogicalSize(m_renderer, WIN_HEIGHT, WIN_WIDTH);

    m_gb_screen = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIN_WIDTH,
        WIN_HEIGHT);

    u32 color = SD::Color { 100, 100, 100, 255 }.to_argb();
    for (int i = 0; i < WIN_HEIGHT * WIN_WIDTH; i++) {
        m_frame_buffer[i] = color;
    }
    SDL_UpdateTexture(m_gb_screen, NULL, m_frame_buffer, WIN_WIDTH * sizeof(u32));
}

// TODO(scd): Decide if I want to be able to load a new rom once the emulator is running. For now,
// lets keep it simple and force a load before run().
void Emulator::load_rom(const char* path)
{
    // TODO(scd): Error checking if unable to load ROM?
    m_cpu.load_rom(path);
}

void Emulator::run()
{
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN) {
                quit = true;
            }
        }

        SDL_RenderClear(m_renderer);
        SDL_RenderCopy(m_renderer, m_gb_screen, NULL, NULL);
        SDL_RenderPresent(m_renderer);

        // Update CPU ticks until ready for frame render
        // Give PPU frame buffer and VRAM for updates
        // Do the 'swap' and render buffer
        // Sleep for appropriate amount of time
    }
}
