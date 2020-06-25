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
constexpr size_t VRAM_SIZE = KB * 8; // TODO: CGB is 16k but regular GB is 8k
constexpr size_t ERAM_SIZE = KB * 8;
constexpr size_t HRAM_SIZE = 126;
constexpr size_t IO_SIZE   = 128;
// clang-format on

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

    u8* bios() { return m_bios; }
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
    u8 m_bios[256] {
        254, 255, 62, 48, 224, 0, 175, 33, 255, 159, 50, 203, 124, 32, 251, 33, 38, 255, 14,
        62, 128, 50, 226, 12, 62, 243, 226, 50, 62, 119, 119, 62, 252, 224, 71, 33, 95, 192,
        8, 175, 50, 13, 32, 252, 17, 79, 1, 62, 251, 14, 6, 245, 6, 0, 26, 27, 50,
        71, 13, 32, 248, 50, 241, 50, 14, 14, 214, 2, 254, 239, 32, 234, 17, 4, 1, 33,
        128, 26, 205, 211, 0, 205, 212, 0, 19, 123, 254, 52, 32, 243, 17, 230, 0, 6, 8,
        19, 34, 35, 5, 32, 249, 62, 25, 234, 16, 153, 33, 47, 153, 14, 12, 61, 40, 8,
        13, 32, 249, 46, 15, 24, 243, 62, 145, 224, 64, 33, 0, 192, 14, 0, 62, 0, 226,
        48, 226, 6, 16, 30, 8, 42, 87, 203, 66, 62, 16, 32, 2, 62, 32, 226, 62, 48,
        203, 26, 29, 32, 239, 5, 32, 232, 62, 32, 226, 62, 48, 226, 205, 194, 0, 125, 254,
        32, 210, 14, 19, 62, 193, 226, 12, 62, 7, 226, 24, 58, 22, 4, 240, 68, 254, 144,
        250, 30, 0, 29, 32, 253, 21, 32, 242, 201, 79, 6, 4, 197, 203, 17, 23, 193, 203,
        23, 5, 32, 245, 34, 35, 34, 35, 201, 60, 66, 185, 165, 185, 165, 66, 60, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 1, 224, 80
    };

    IODevice* m_io_devices[IO_SIZE];
};
