//
// Created by Spencer Dixon on 5/10/20.
//

#pragma once

#include <SD/Utility.h>

#include "CPU.h"
#include "Debugger.h"
#include "Emulator/GUI/Bitmap.h"
#include "Emulator/GUI/SDLRenderer.h"
#include "Emulator/GUI/Texture.h"
#include "InternalSDL.h"
#include "Joypad.h"
#include "MMU.h"
#include "PPU.h"
#include "RuntimeSettings.h"
#include "SoundCard.h"
#include "Timer.h"

class Emulator {
public:
    Emulator(RuntimeSettings settings)
        : m_settings(settings)
        , m_mmu(*this)
        , m_joypad(*this)
        , m_cpu(*this)
        , m_ppu(*this)
        , m_sound_card({})
        , m_timer(*this)
        , m_debugger(*this)
    {
        // NOTE: order matters here. Devices need to be initialized before CPU can boot.
        mmu().init_devices();
        cpu().boot();

        if (!settings.in_test_mode) {
            auto font_path = m_settings.assets_dir + "/early-gameboy.ttf";
            SDLRenderer::the().init(font_path);
            auto bg_path = m_settings.assets_dir + "/SpenceBoy.png";
            m_gb_background = Texture::from_image(bg_path);
            ppu().init_textures();
        }
    }

    void load_rom(const char* path);
    void run();

    MMU& mmu() { return m_mmu; }
    Joypad& joypad() { return m_joypad; }
    PPU& ppu() { return m_ppu; }
    CPU& cpu() { return m_cpu; }
    Timer& timer() { return m_timer; }
    SoundCard& sound() { return m_sound_card; }
    Renderer& renderer() { return SDLRenderer::the(); }
    String& assets_dir() { return m_settings.assets_dir; }
    RuntimeSettings& settings() { return m_settings; }
    Debugger& debugger() { return m_debugger; }

    void render_debug_frame();

private:
    RuntimeSettings m_settings;
    MMU m_mmu;
    Joypad m_joypad;
    CPU m_cpu;
    PPU m_ppu;
    SoundCard m_sound_card;
    Timer m_timer;
    Debugger m_debugger;

    // Maybe this belongs elsewhere. But where?
    Texture m_gb_background;
};
