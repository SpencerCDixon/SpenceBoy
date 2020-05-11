//
// Created by Spencer Dixon on 5/10/20.
//

#include "Emulator.h"
#include <SD/Assertions.h>
#include <SD/Color.h>
#include <unistd.h>

// background - always on bottom
//  open a window with all my tiles so I can see if they're correct
// sprites
// window - always on top

void render_gradient_test(OffscreenFrameBuffer* buffer, int blue_offset, int green_offset)
{
    u8* row = (u8*)buffer->memory;
    for (int y = 0; y < buffer->height; ++y) {
        u32* pixel = (u32*)row;

        for (int x = 0; x < buffer->width; ++x) {
            u8 blue = (x + blue_offset);
            u8 green = (y + green_offset);
            *pixel++ = ((green << 8) | blue);
        }

        row += buffer->pitch;
    }
}

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

    m_gb_screen = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIN_WIDTH,
        WIN_HEIGHT);

    render_gradient_test(&m_frame_buffer, 0, 0);
    swap();
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
    int green_offset = 0;
    int blue_offset = 0;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN) {
                quit = true;
            }
        }

        green_offset++;
        blue_offset++;
        render_gradient_test(&m_frame_buffer, green_offset, blue_offset);
        swap();

        SDL_RenderClear(m_renderer);
        SDL_RenderCopy(m_renderer, m_gb_screen, NULL, NULL);
        SDL_RenderPresent(m_renderer);
        usleep(5000);

        // Update CPU ticks until ready for frame render
        // Give PPU frame buffer and VRAM for updates
        // Do the 'swap' and render buffer
        // Sleep for appropriate amount of time
    }
}

void Emulator::swap()
{
    SDL_UpdateTexture(m_gb_screen, NULL, m_frame_buffer.memory, m_frame_buffer.pitch);
}
