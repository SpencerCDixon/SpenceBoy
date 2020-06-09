//
// Created by Spencer Dixon on 5/10/20.
//

#include "Emulator.h"
#include "Joypad.h"
#include <SD/Assertions.h>
#include <SD/Timer.h>
#include <SD/Types.h>

// FIXME: Look into correct clock speed info. For now, going to hard code 4 Megahertz
// FIXME: Of course I need to device by the frame rate to calculate how many cycles to execute before
// rendering. 4,000,000 / 60 = 66k cycles worth of work a frame.
constexpr u64 CYCLES_PER_SECOND = 4000000;

void Emulator::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        error("unable to initialize SDL video subsystem");
    }

    if (SDL_CreateWindowAndRenderer(GB_WIN_HEIGHT, GB_WIN_WIDTH, 0, &m_window, &m_renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        error("creating window and renderer");
    }

    SDL_SetWindowTitle(m_window, "SpenceBoy");
    SDL_SetWindowResizable(m_window, SDL_bool::SDL_TRUE);
    SDL_RenderSetLogicalSize(m_renderer, GB_WIN_HEIGHT, GB_WIN_WIDTH);

    m_gb_screen = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        GB_WIN_WIDTH,
        GB_WIN_HEIGHT);
}

// TODO(scd): Decide if I want to be able to load a new rom once the emulator is running. For now,
// lets keep it simple and force a load before run().
void Emulator::load_rom(const char* path)
{
    cpu().load_rom(path);
}

void Emulator::run()
{
    SDL_Event e;
    bool quit = false;
    bool halted = false;
    bool show_input_debug = false;
    InputDebugWindow input_debug(m_renderer);
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
                    joypad().set_key_state(Key::Left, is_down);
                    break;
                case SDLK_d:
                    joypad().set_key_state(Key::Right, is_down);
                    break;
                case SDLK_s:
                    joypad().set_key_state(Key::Down, is_down);
                    break;
                case SDLK_w:
                    joypad().set_key_state(Key::Up, is_down);
                    break;
                case SDLK_q:
                    joypad().set_key_state(Key::A, is_down);
                    break;
                case SDLK_e:
                    joypad().set_key_state(Key::B, is_down);
                    break;
                default:
                    break;
                }
            }

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_k && SDL_GetModState() & KMOD_CTRL)
                show_input_debug = !show_input_debug;
        }

        if (!halted) {
            auto t = Timer("4 megahertz()");

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

        //        if (halted)
        //            dbg() << "halted!";

        ppu().clear({ 255, 255, 255, 255 });
        ppu().render();
        swap();

        SDL_RenderClear(m_renderer);
        SDL_RenderCopy(m_renderer, m_gb_screen, NULL, NULL);

        // Render Debug:
        if (show_input_debug) {
            input_debug.render(&m_joypad);
        }

        SDL_RenderPresent(m_renderer);

        // TODO: Timing to determine how much I should sleep to hit 60 FPS.
    }
}

void Emulator::swap()
{
    SDL_UpdateTexture(m_gb_screen, NULL, m_frame_buffer.memory, m_frame_buffer.pitch);
}
