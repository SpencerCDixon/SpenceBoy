//
// Created by Spencer Dixon on 6/7/20.
//

#pragma once

#include "IODevice.h"

#include <SD/String.h>
#include <SD/Types.h>

#include <stdlib.h>

// clang-format off
constexpr u16 MAX_ROM_SIZE = KB * 32;
constexpr size_t WRAM_SIZE = KB * 8;
constexpr size_t VRAM_SIZE = KB * 16;
constexpr size_t HRAM_SIZE = 126;
constexpr size_t IO_SIZE   = 112;
// clang-format on


// 0xff00 - input
// 0xff10 - 0xff1e - all sound card related
// 0xff47

class Emulator;

class MMU {
public:
    MMU(Emulator& emulator)
        : m_emulator(emulator)
    {
        m_wram = (u8*)calloc(WRAM_SIZE, sizeof(u8));
        m_vram = (u8*)calloc(VRAM_SIZE, sizeof(u8));
        m_hram = (u8*)calloc(HRAM_SIZE, sizeof(u8));
    }

    ~MMU()
    {
        free(m_wram);
        free(m_vram);
        free(m_hram);
    }

    void init_devices();
    void load_rom(const char* rom_path);

    u8* vram() { return m_vram; }
    u8* wram() { return m_wram; }
    u8* rom()
    {
        ASSERT(m_rom);
        return m_rom;
    }

    u8 read(u16 address);
    void write(u16 address, u8 data);

private:
    Emulator& emulator() { return m_emulator; }

private:
    Emulator& m_emulator;

    u8* m_wram { nullptr };
    u8* m_vram { nullptr };
    u8* m_hram { nullptr };
    u8* m_rom { nullptr };

    IODevice* m_io_devices[IO_SIZE];
};
