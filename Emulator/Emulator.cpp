//
// Created by Spencer Dixon on 5/10/20.
//

#include <SD/Timer.h>
#include <SD/Types.h>

#include "Emulator.h"
#include "Joypad.h"

void Emulator::load_rom(const char* path)
{
    mmu().load_rom(path);
}

void Emulator::run()
{
    if (m_settings.in_test_mode)
        cpu().main_test_loop();

    SDL_Event e;
    bool quit = false;
    bool show_input_debug = true;
    bool should_enter_debugger = settings().in_debug_mode;
    InputDebugWindow input_debug(*this);

    while (!quit) {
        auto frame_timer = Timer("frame");

        //
        // Input
        //
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
                    joypad().set_key_state(Key::B, is_down);
                    break;
                case SDLK_e:
                    joypad().set_key_state(Key::A, is_down);
                    break;
                case SDLK_SPACE:
                    joypad().set_key_state(Key::Start, is_down);
                    break;
                case SDLK_r:
                    joypad().set_key_state(Key::Select, is_down);
                    break;
                default:
                    break;
                }
            }

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_k && SDL_GetModState() & KMOD_CTRL)
                show_input_debug = !show_input_debug;

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
                should_enter_debugger = !should_enter_debugger;
        }

        if (should_enter_debugger) {
            debugger().enter();
            cpu().attach_debugger(&debugger());
            should_enter_debugger = false;
        }

        //
        // Execute
        //
        cpu().main_loop();

        //
        // Render
        //
        local_persist Color bg_clear { 255, 255, 255, 255 };

        // TODO: These really need to be moved into the PPU for rendering one line at a time
        local_persist u8 WINDOW_WIDTH = 160;
        local_persist u8 WINDOW_HEIGHT = 144;

        renderer().clear(bg_clear);

        ppu().clear_debug_textures();
        ppu().render_debug_textures();

        renderer().draw_texture(m_gb_background, Point { 20, 20 });
        renderer().draw_texture(ppu().tilemap(), Point { 324, 20 });
        renderer().draw_texture(ppu().tileset(), Point { 324, 286 });
        renderer().draw_texture(ppu().lcd_display(), Point { 85, 67 });

        //
        // Render Debug
        //
        if (show_input_debug) {
            input_debug.render();
        }

        // BG Tilemap Debug Wireframe:
        auto window_frame_color = Color { 0, 0, 0, 255 };
        auto base_x = 324 + ppu().scx();
        auto base_y = 20 + ppu().scy();
        auto thickness = 1;
        renderer().draw_rect({ base_x, base_y, WINDOW_WIDTH, thickness }, window_frame_color);
        renderer().draw_rect({ base_x, base_y + WINDOW_HEIGHT, WINDOW_WIDTH, thickness }, window_frame_color);
        renderer().draw_rect({ base_x, base_y, thickness, WINDOW_HEIGHT }, window_frame_color);
        renderer().draw_rect({ base_x + WINDOW_WIDTH, base_y, thickness, WINDOW_HEIGHT }, window_frame_color);

        renderer().present();
        frame_timer.wait_until_elapsed_ms(16.666);
    }
}

void Emulator::render_debug_frame()
{
    local_persist Color bg_clear { 255, 255, 255, 255 };

    // TODO: These really need to be moved into the PPU for rendering one line at a time
    local_persist u8 WINDOW_WIDTH = 160;
    local_persist u8 WINDOW_HEIGHT = 144;

    renderer().clear(bg_clear);

    ppu().clear_debug_textures();
    ppu().render_debug_textures();

    renderer().draw_texture(m_gb_background, Point { 20, 20 });
    renderer().draw_texture(ppu().tilemap(), Point { 324, 20 });
    renderer().draw_texture(ppu().tileset(), Point { 324, 286 });
    renderer().draw_texture(ppu().lcd_display(), Point { 85, 67 });

    // BG Tilemap Debug Wireframe:
    auto window_frame_color = Color { 0, 0, 0, 255 };
    auto base_x = 324 + ppu().scx();
    auto base_y = 20 + ppu().scy();
    auto thickness = 1;
    renderer().draw_rect({ base_x, base_y, WINDOW_WIDTH, thickness }, window_frame_color);
    renderer().draw_rect({ base_x, base_y + WINDOW_HEIGHT, WINDOW_WIDTH, thickness }, window_frame_color);
    renderer().draw_rect({ base_x, base_y, thickness, WINDOW_HEIGHT }, window_frame_color);
    renderer().draw_rect({ base_x + WINDOW_WIDTH, base_y, thickness, WINDOW_HEIGHT }, window_frame_color);

    renderer().present();
}
