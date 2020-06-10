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

// TODO(scd): Decide if I want to be able to load a new rom once the emulator is running. For now,
// lets keep it simple and force a load before run().
void Emulator::load_rom(const char* path)
{
    mmu().load_rom(path);
}

void Emulator::run()
{
    SDL_Event e;
    bool quit = false;
    bool halted = false;
    bool show_input_debug = false;

//    InputDebugWindow input_debug(m_renderer);
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
        local_persist Color bg_clear { 255, 255, 255, 255};
        local_persist Color gb_clear { 125, 130, 255, 255};
        renderer().clear(bg_clear);

        ppu().clear(gb_clear);
        ppu().render();

        renderer().draw_bitmap(ppu().bitmap(), {324, 20, 256, 256});
        renderer().draw_hardware();

        // Render Debug:
        if (show_input_debug) {
//            input_debug.render(&m_joypad);
        }

        renderer().present();
        // TODO: Timing to determine how much I should sleep to hit 60 FPS.
    }
}

