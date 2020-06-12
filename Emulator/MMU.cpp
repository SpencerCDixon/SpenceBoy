//
// Created by Spencer Dixon on 6/7/20.
//

#include "MMU.h"

#include <SD/Assertions.h>
#include <SD/LogStream.h>

#define MMU_DEBUG 0

constexpr u16 ROM_START = 0x0000;
constexpr u16 ROM_END = 0x3FFF;
constexpr u16 WRAM_START = 0xC000;
constexpr u16 WRAM_END = 0xDFFF;
constexpr u16 VRAM_START = 0x8000;
constexpr u16 VRAM_END = 0xA000; // Isn't GBC supposed to have 16k VRAM? Look into this...
constexpr u16 HRAM_START = 0xFF80;
constexpr u16 HRAM_END = 0xFFFE;
// Echo ram is apparently not used by anything. We'll see how that holds up...
constexpr u16 ERAM_START = 0xE000;
constexpr u16 ERAM_END = 0xFDFF;

void MMU::load_rom(const char* rom_path)
{
    auto file = File::open(rom_path);

#if MMU_DEBUG
    dbg() << "CPU::load_rom() loading rom into memory from path: " << rom_path;
    dbg() << "size of ROM file is: " << file.size_in_bytes();
    ASSERT(file.size_in_bytes() == MAX_ROM_SIZE);
#endif

    m_rom = (u8*)malloc((size_t)file.size_in_bytes());
    file.read_into((char*)m_rom);
}

u8 MMU::read(u16 address)
{
    if (address >= WRAM_START && address <= WRAM_END) {
        u16 idx = address - WRAM_START;
        ASSERT(idx >= 0);
        return m_wram[idx];
    } else if (address >= VRAM_START && address <= VRAM_END) {
        u16 idx = address - VRAM_START;
        ASSERT(idx >= 0);
        return m_vram[idx];
    } else if (address >= ROM_START && address <= ROM_END) {
        return m_rom[address];
    } else if (address >= HRAM_START && address <= HRAM_END) {
        u16 idx = address - HRAM_START;
        return m_hram[idx];
    } else {
        dbg() << "bad read address: " << address;
    }

    ASSERT_NOT_REACHED();
    return 0;
}

void MMU::write(u16 address, u8 data)
{
    if (address >= WRAM_START && address <= WRAM_END) {
        u16 idx = address - WRAM_START;
        ASSERT(idx >= 0);
        m_wram[idx] = data;
        return;
    } else if (address >= VRAM_START && address <= VRAM_END) {
        u16 idx = address - VRAM_START;
        ASSERT(idx >= 0);
        m_vram[idx] = data;
        return;
    } else if (address >= ROM_START && address <= ROM_END) {
        m_rom[address] = data;
        return;
    } else if (address >= HRAM_START && address <= HRAM_END) {
        u16 idx = address - HRAM_START;
        m_hram[idx] = data;
        return;
    } else if (address >= ERAM_START && address <= ERAM_END) {
        dbg() << "echo ram write at: " << address << " treating as no-op";
        return;
    } else {
        dbg() << "bad write address: " << address;
    }

    // If we've reached here it means we're trying to write to memory
    // that is not set up yet. Fail hard and implement that memory!
//    ASSERT_NOT_REACHED();
}
