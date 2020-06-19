//
// Created by Spencer Dixon on 6/7/20.
//

#include "MMU.h"
#include "Emulator.h"
#include "MemoryMap.h"

#include <SD/Assertions.h>
#include <SD/LogStream.h>

#define MMU_DEBUG 0

void MMU::init_devices()
{
    // FIXME: I probably want some sort of address_to_index which translates the
    // I/O u16 address into an index into our array of devices and such.
    constexpr u16 ppu_start = 0xff40 - IO_START;
    constexpr u16 ppu_end = 0xff4b - IO_START;
    //    constexpr u16 sound_start = 0xff10 - IO_START;
    //    constexpr u16 sound_end = 0xff3f - IO_START;

    constexpr u16 interrupt_flag = 0xff0f - IO_START;
    constexpr u16 sound_start = 0xff10 - IO_START;
    constexpr u16 sound_end = 0xff3f - IO_START;

    for (size_t i = 0; i < IO_SIZE; ++i) {
        if (i == 0) {
            m_io_devices[i] = &emulator().joypad();
        } else if (i >= ppu_start && i < ppu_end) {
            m_io_devices[i] = &emulator().ppu();
        } else if (i >= sound_start && i < sound_end) {
            m_io_devices[i] = &emulator().sound();
        } else if (i == interrupt_flag) {
            m_io_devices[i] = &emulator().cpu();
        } else {
            m_io_devices[i] = &DummyIODevice::the();
        }
    }
}

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
    } else if (address >= IO_START && address <= IO_END) {
        u16 idx = address - IO_START;
        ASSERT(idx >= 0 && idx < IO_SIZE);
        return m_io_devices[idx]->in(address);
    } else {
        dbg() << "bad read address: " << to_hex(address);
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
    } else if (address >= IO_START && address <= IO_END) {
        u16 idx = address - IO_START;
        ASSERT(idx >= 0 && idx < IO_SIZE);
        return m_io_devices[idx]->out(address, data);
    } else if (address >= ERAM_START && address <= ERAM_END) {
        dbg() << "echo ram write at: " << address << " treating as no-op";
        return;
    } else if (address == IE_LOCATION) {
        emulator().cpu().out(address, data);
    } else {
        dbg() << "bad write address: " << address;
    }

    // If we've reached here it means we're trying to write to memory
    // that is not set up yet. Fail hard and implement that memory!
    //
    // When working on new ROM's uncomment out the assert so I can diagnose.
//    ASSERT_NOT_REACHED();
}
