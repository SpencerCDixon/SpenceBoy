//
// Created by Spencer Dixon on 6/12/20.
//

#pragma once

#include <SD/Types.h>

// clang-format off

// TODO: ROM's can have different sizes. We need to properly account for this with Bank switching.
// 0x0000 - 0x3fff - BANK 0
// 0x4000 - 0x7fff - BANK 1
constexpr u16 ROM_START   = 0x0000;
constexpr u16 ROM_END     = 0x7FFF;

constexpr u16 WRAM_START  = 0xC000;
constexpr u16 WRAM_END    = 0xDFFF;
constexpr u16 VRAM_START  = 0x8000;
constexpr u16 VRAM_END    = 0xA000; // Isn't GBC supposed to have 16k VRAM? Look into this...
constexpr u16 HRAM_START  = 0xFF80;
constexpr u16 HRAM_END    = 0xFFFE;
constexpr u16 ERAM_START  = 0xE000; // Apparently not used for anything. We'll see!
constexpr u16 ERAM_END    = 0xFDFF;
constexpr u16 IO_START    = 0xFF00;
constexpr u16 IO_END      = 0xFF7F;
constexpr u16 IE_LOCATION = 0xFFFF;
// clang-format on