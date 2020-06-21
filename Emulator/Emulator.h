//
// Created by Spencer Dixon on 5/10/20.
//

#pragma once

#include <SD/Utility.h>

#include "CPU.h"
#include "Emulator/GUI/Bitmap.h"
#include "Emulator/GUI/SDLRenderer.h"
#include "Emulator/GUI/Texture.h"
#include "InternalSDL.h"
#include "Joypad.h"
#include "MMU.h"
#include "PPU.h"
#include "RuntimeSettings.h"
#include "SoundCard.h"

class Emulator {
public:
    Emulator(RuntimeSettings settings)
        : m_settings(settings)
        , m_mmu(*this)
        , m_joypad({})
        , m_cpu(*this, settings.verbose_logging)
        , m_ppu(*this)
        , m_sound_card({})
    {
        SDLRenderer::the().init();

        auto path = m_settings.assets_dir + "/SpenceBoy.png";
        m_gb_background = Texture::from_image(path);

        mmu().init_devices();
        ppu().init_textures();
    }

    void load_rom(const char* path);
    void run();

    MMU& mmu() { return m_mmu; }
    Joypad& joypad() { return m_joypad; }
    PPU& ppu() { return m_ppu; }
    CPU& cpu() { return m_cpu; }
    SoundCard& sound() { return m_sound_card; }
    Renderer& renderer() { return SDLRenderer::the(); }
    String& assets_dir() { return m_settings.assets_dir; }

private:
    RuntimeSettings m_settings;
    MMU m_mmu;
    Joypad m_joypad;
    CPU m_cpu;
    PPU m_ppu;
    SoundCard m_sound_card;

    // Maybe this belongs elsewhere. But where?
    Texture m_gb_background;
};
