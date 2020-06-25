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
        254, 255, 33, 0, 128, 34, 203, 108, 40, 251, 62, 128, 224, 38, 224, 17, 62, 243, 224,
        224, 37, 62, 119, 224, 36, 62, 84, 224, 71, 17, 4, 1, 33, 16, 128, 26, 71, 205,
        0, 205, 162, 0, 19, 123, 238, 52, 32, 242, 17, 209, 0, 14, 8, 26, 19, 34, 35,
        32, 249, 62, 25, 234, 16, 153, 33, 47, 153, 14, 12, 61, 40, 8, 50, 13, 32, 249,
        15, 24, 245, 62, 30, 224, 66, 62, 145, 224, 64, 22, 137, 14, 15, 205, 183, 0, 122,
        47, 203, 47, 224, 66, 122, 129, 87, 121, 254, 8, 32, 4, 62, 168, 224, 71, 13, 32,
        62, 252, 224, 71, 62, 131, 205, 202, 0, 6, 5, 205, 195, 0, 62, 193, 205, 202, 0,
        60, 205, 195, 0, 33, 176, 1, 229, 241, 33, 77, 1, 1, 19, 0, 17, 216, 0, 195,
        0, 62, 4, 14, 0, 203, 32, 245, 203, 17, 241, 203, 17, 61, 32, 245, 121, 34, 35,
        35, 201, 229, 33, 15, 255, 203, 134, 203, 70, 40, 252, 225, 201, 205, 183, 0, 5, 32,
        201, 224, 19, 62, 135, 224, 20, 201, 60, 66, 185, 165, 185, 165, 66, 60, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 224, 80
    };

    IODevice* m_io_devices[IO_SIZE];
};
