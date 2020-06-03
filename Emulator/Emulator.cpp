//
// Created by Spencer Dixon on 5/10/20.
//

#include "Emulator.h"
#include "Input.h"
#include <SD/Assertions.h>
#include <SD/Types.h>
#include <unistd.h>

// FIXME: Look into correct clock speed info. For now, going to hard code 4 Megahertz
//constexpr u64 CYCLES_PER_SECOND = 4000000;
constexpr u64 CYCLES_PER_SECOND = 50000; // DEBUG purposes

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
}

// TODO(scd): Decide if I want to be able to load a new rom once the emulator is running. For now,
// lets keep it simple and force a load before run().
void Emulator::load_rom(const char* path)
{
    m_cpu.load_rom(path);
}

void Emulator::run()
{
    SDL_Event e;
    bool quit = false;
    bool halted = false;
    Input input;

    u64 cycle_count = 0;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
                bool is_down = e.type == SDL_KEYDOWN;
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_a:
                    input.set_key_state(Key::Left, is_down);
                    break;
                case SDLK_d:
                    input.set_key_state(Key::Right, is_down);
                    break;
                case SDLK_s:
                    input.set_key_state(Key::Down, is_down);
                    break;
                case SDLK_w:
                    input.set_key_state(Key::Up, is_down);
                    break;
                case SDLK_q:
                    input.set_key_state(Key::A, is_down);
                    break;
                case SDLK_e:
                    input.set_key_state(Key::B, is_down);
                    break;
                }
            }
        }

        if (!halted) {
            for (;;) {
                auto result = m_cpu.step();
                if (result.should_halt) {
                    halted = true;
                    break;
                } else {
                    cycle_count += result.cycles;
                    if (cycle_count >= CYCLES_PER_SECOND) {
                        cycle_count = 0;
                        break;
                    }
                }
            }
        }

        dbg() << input;

        m_ppu.clear({ 255, 255, 255, 255 });
        m_ppu.render();
        swap();

        SDL_RenderClear(m_renderer);
        SDL_RenderCopy(m_renderer, m_gb_screen, NULL, NULL);
        SDL_RenderPresent(m_renderer);
    }
}

void Emulator::swap()
{
    SDL_UpdateTexture(m_gb_screen, NULL, m_frame_buffer.memory, m_frame_buffer.pitch);
}
