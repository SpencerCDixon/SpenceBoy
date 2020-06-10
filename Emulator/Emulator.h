//
// Created by Spencer Dixon on 5/10/20.
//

#pragma once

#include "CPU.h"
#include "Emulator/GUI/Bitmap.h"
#include "Emulator/GUI/SDLRenderer.h"
#include "InternalSDL.h"
#include "Joypad.h"
#include "MMU.h"
#include "PPU.h"

class Emulator {
public:
    Emulator(bool verbose_logging = false)
        : m_mmu({})
        , m_joypad({})
        , m_cpu(*this, verbose_logging)
        , m_ppu(*this)
    {
       SDLRenderer::the().init();
        // TODO: 2 step init process
        // mmu->init_io_devices
    }

    ~Emulator()
    {
    }

    void load_rom(const char* path);
    void run();

    MMU& mmu() { return m_mmu; }
    Joypad& joypad() { return m_joypad; }
    PPU& ppu() { return m_ppu; }
    CPU& cpu() { return m_cpu; }
    Renderer& renderer() { return SDLRenderer::the(); }

private:
    MMU m_mmu;
    Joypad m_joypad;
    CPU m_cpu;
    PPU m_ppu;
};
