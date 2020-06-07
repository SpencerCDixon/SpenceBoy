//
// Created by Spencer Dixon on 6/7/20.
//

#pragma once

#include <SD/Types.h>

#include <stdlib.h>

constexpr size_t WRAM_SIZE = KB * 8;
constexpr size_t VRAM_SIZE = KB * 16;
constexpr size_t HRAM_SIZE = 126;

class MMU {
public:
    MMU() {
        m_wram = (u8*)calloc(WRAM_SIZE, sizeof(u8));
        m_vram = (u8*)calloc(VRAM_SIZE, sizeof(u8));
        m_hram = (u8*)calloc(HRAM_SIZE, sizeof(u8));
    }

    ~MMU() {
        free(m_wram);
        free(m_vram);
        free(m_hram);
    }

    u8* vram() { return m_vram; }
    u8* wram() { return m_wram; }

    u8 read(u16 address);
    void write(u16 address, u8 data);

private:
    u8* m_wram { nullptr };
    u8* m_vram { nullptr };
    u8* m_hram { nullptr };
};
