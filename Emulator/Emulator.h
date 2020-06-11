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

class Emulator {
public:
    Emulator(RuntimeSettings settings)
        : m_settings(settings)
        , m_mmu({})
        , m_joypad({})
        , m_cpu(*this, settings.verbose_logging)
        , m_ppu(*this)
    {
        SDLRenderer::the().init(settings);

        auto path = m_settings.assets_dir + "/SpenceBoy.png";
        m_gb_background = Texture::from_image(path);

        m_gb_frame = { 20, 20, m_gb_background.width(), m_gb_background.height() };


        // TODO: 2 step init process
        // mmu->init_io_devices
        ppu().init_textures();
    }

    void load_rom(const char* path);
    void run();

    MMU& mmu() { return m_mmu; }
    Joypad& joypad() { return m_joypad; }
    PPU& ppu() { return m_ppu; }
    CPU& cpu() { return m_cpu; }
    Renderer& renderer() { return SDLRenderer::the(); }
    String& assets_dir() { return m_settings.assets_dir; }

private:
    RuntimeSettings m_settings;
    MMU m_mmu;
    Joypad m_joypad;
    CPU m_cpu;
    PPU m_ppu;

    // Maybe this belongs elsewhere. But where?
    Texture m_gb_background;
    Rect m_gb_frame;
};
