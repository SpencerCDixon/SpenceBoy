//
// Created by Spencer Dixon on 5/9/20.
//

#include "PPU.h"
#include "Emulator.h"

#include <SD/Assertions.h>
#include <SD/Bytes.h>

// Useful Notes:
// * A vertical refresh happens every 70224 clocks (140448 in GBC double speed mode): 59,7275 Hz
// * A scanline normally takes 456 clocks (912 clocks in double speed mode) to complete
// * Vertical Blank interrupt is triggered when the LCD controller enters the VBL screen mode (mode 1, LY=144).
//   This happens once per frame, so this interrupt is triggered 59.7 times per second.

constexpr size_t TILE_WIDTH = 32;
constexpr size_t TILE_HEIGHT = 32;
constexpr size_t TOTAL_BG_TILES = TILE_WIDTH * TILE_HEIGHT;

// Three blocks of 128 each
constexpr size_t TOTAL_TILESET_TILES = 384;

// PPU-related Registers
constexpr static u16 R_LCDC = 0xff40;
constexpr static u16 R_SCY = 0xff42;
constexpr static u16 R_SCX = 0xff43;
constexpr static u16 R_BGP = 0xff47;

#define DEBUG_PPU 0

Tile8x8::Tile8x8()
{
    memset(m_pixels, 0, sizeof(m_pixels));
}

void Tile8x8::populate_from_palette(const u8* buffer, const u32* palette)
{
    for (size_t row = 0; row < 8; row++) {
        // 0 1 | 2 3 | 4 5 | 6 7
        size_t lower_byte_idx = row * 2;
        size_t higher_byte_idx = lower_byte_idx + 1;

        u8 lower_byte = buffer[lower_byte_idx];
        u8 higher_byte = buffer[higher_byte_idx];

        for (size_t col = 0; col < 8; ++col) {
            u8 hi_bit = (higher_byte >> (7 - col)) & 1;
            u8 low_bit = (lower_byte >> (7 - col)) & 1;
            // 00 01 10 11
            u8 idx_into_palette = (hi_bit << 1) | low_bit;
            set_pixel(col, row, palette[idx_into_palette]);
        }
    }
}

PPU::PPU(Emulator& emulator)
    : m_emulator(emulator)
    , m_vram(emulator.mmu().vram())
    , m_bitmap({ GB_WIN_WIDTH, GB_WIN_HEIGHT }, GB_WIN_WIDTH * BITS_PER_PIXEL)
    , m_tileset_bitmap({ TILESET_WIN_WIDTH, TILESET_WIN_HEIGHT }, TILESET_WIN_WIDTH * BITS_PER_PIXEL)
    , m_lcd_bitmap({ LCD_WIDTH, LCD_HEIGHT }, LCD_WIDTH * BITS_PER_PIXEL)
{
}

bool PPU::lcd_display_enabled()
{
    return emulator().cpu().in_boot_rom() || m_lcd_control & 0x80;
}

void PPU::clear_debug_textures()
{
    local_persist Color gb_clear { 125, 130, 255, 255 };
    u32 argb_color = gb_clear.to_argb();

    m_bitmap.set_all_pixels_to(argb_color);
    m_tileset_bitmap.set_all_pixels_to(argb_color);
}

// TODO:
// * layered rendering (background, sprites, window)

void PPU::fill_square(size_t x, size_t y, const Tile8x8& tile, Bitmap& bitmap)
{
    ASSERT(x < 32);
    ASSERT(y < 32);

    for (size_t yy = 0; yy < 8; yy++) {
        for (size_t xx = 0; xx < 8; xx++) {
            bitmap.set_pixel_to(xx + (x * 8), yy + (y * 8), tile.pixel(xx, yy));
        }
    }
}

// void PPU::fill_line(size_t, const Tile8x8&, Bitmap&)
// {
// }

// NOTE: There is some weird logic here to handle cycle overflows because the CPU is driving the PPU forward
// but the cycles executed might not always line up perfectly with the number of cycles for switching PPU modes.
// We need to carry those overflowed cycles into the next mode. Eventually, we'll want to emulate the pixel FIFO
// which will help in determining the amount of cycles we need to wait between Mode 3 (drawing) and Mode 0 (HBlank).
void PPU::update_by(u8 cycles)
{
    // TODO: switch_mode() -> set any interrupts
    m_cycles_until_mode_transition -= (s32)cycles;
    m_scanline_cycle_count += cycles;

    switch (mode()) {
    case PPUMode::AccessingOAM: {
        if (m_cycles_until_mode_transition <= 0) {
            // FIXME: Really we should be using the pixel FIFO to determine this 43 cycle count. Depending on sprites being rendered
            // the drawing mode (AccessVRAM) will  change.
            m_cycles_until_mode_transition = 43 + m_cycles_until_mode_transition;
            set_mode(PPUMode::AccessingVRAM);
        }
        break;
    }
    case PPUMode::AccessingVRAM: {
        if (m_cycles_until_mode_transition <= 0) {
            draw_scanline();
            m_current_scanline++;
            m_cycles_until_mode_transition = 114 - m_scanline_cycle_count;
            set_mode(PPUMode::HorizontalBlanking);
        }
        break;
    }
    case PPUMode::VerticalBlanking: {
        if (m_cycles_until_mode_transition <= 0 && m_current_scanline < 153) {
            draw_scanline();
            m_current_scanline++;
            m_cycles_until_mode_transition = 114 + m_cycles_until_mode_transition;
            m_scanline_cycle_count = 0;
        } else if (m_current_scanline == 153) {
            // Reset, frame is done being rendered!
            draw_scanline();
            m_current_scanline = { 0 };
            m_scanline_cycle_count = { 0 };
            m_cycles_until_mode_transition = { 20 };
            set_mode(PPUMode::AccessingOAM);
        }
        break;
    }
    case PPUMode::HorizontalBlanking: {
        if (m_cycles_until_mode_transition <= 0) {
            // Enter VBlank for 10 lines
            if (m_current_scanline == 143) {
                m_cycles_until_mode_transition = 114 + m_cycles_until_mode_transition;
                m_scanline_cycle_count = 0;
                set_mode(PPUMode::VerticalBlanking);
            } else {
                m_scanline_cycle_count = { 0 };
                m_cycles_until_mode_transition = 20 + m_cycles_until_mode_transition;
                set_mode(PPUMode::AccessingOAM);
            }
        }
        break;
    }
    }
}

void PPU::draw_scanline()
{
#if DEBUG_PPU
    dbg() << "draw_scanline() current line: " << m_current_scanline << " cycles to transition: " << m_cycles_until_mode_transition;
#endif

    // FIXME: Take scx and scy into consideration. Regressing that behavior for now

    // * Create tiles
    // * Get tiles that are located on the current scanline
    // * Fill pixels for the line of each tile

    // FIXME: This is heavy handed, I can just fetch the tiles I need instead of
    // all of them each scanline
    // Tile8x8 tileset[TOTAL_TILESET_TILES];
    // for (size_t i = 0; i < TOTAL_TILESET_TILES; ++i) {
    // size_t idx = i * 16; // 16 bytes (8 x 8 x 2 bpp)
    // auto* pointer = &vram()[idx];
    // tileset[i].populate_from_palette(pointer, &m_palette[0]);
    // }

    // size_t map_start = bg_tilemap_display_select() - 0x8000;
    // for (size_t i = 0; i < 5; ++i) {
    // }

    // Render example square for testing
    auto color = Color { 128, 128, 128, 255 }.to_argb();
    auto y = 1;
    auto x = 2;

    for (size_t yy = 0; yy < 8; yy++) {
        for (size_t xx = 0; xx < 8; xx++) {
            m_lcd_bitmap.set_pixel_to(xx + (x * 8), yy + (y * 8), color);
        }
    }

    m_lcd_display.set_data(m_lcd_bitmap);
}

void PPU::set_mode(const PPUMode& mode)
{
#if DEBUG_PPU
    dbg() << "setting mode: " << mode;
#endif
    m_mode = mode;
}

void PPU::render()
{
    if (!lcd_display_enabled()) {
        dbg() << "LCD/PPU are disabled, returning early";
        return;
    }

    Tile8x8 tileset[TOTAL_TILESET_TILES];
    for (size_t i = 0; i < TOTAL_TILESET_TILES; ++i) {
        size_t idx = i * 16; // 16 bytes (8 x 8 x 2 bpp)
        auto* pointer = &vram()[idx];
        tileset[i].populate_from_palette(pointer, &m_palette[0]);
    }

    // A special chunk of memory starting at 0x9800 or 0x9c00 is used to map which tile index
    // should be used to render. Each byte represents the index into the tile map.
    size_t map_start = bg_tilemap_display_select() - 0x8000;
    for (size_t i = 0; i < TOTAL_BG_TILES; ++i) {
        size_t tile_idx = vram()[map_start + i];
        size_t x = i % 32;
        size_t y = i / 32;
        fill_square(x, y, tileset[index_into_tileset(tile_idx)], m_bitmap);
    }

    for (size_t i = 0; i < TOTAL_TILESET_TILES; ++i) {
        size_t x = i % 32;
        size_t y = i / 32;
        fill_square(x, y, tileset[i], m_tileset_bitmap);
    }

    m_tilemap.set_data(m_bitmap);
    m_tileset.set_data(m_tileset_bitmap);
}

void PPU::render_debug_textures()
{
    Tile8x8 tileset[TOTAL_TILESET_TILES];
    for (size_t i = 0; i < TOTAL_TILESET_TILES; ++i) {
        size_t idx = i * 16; // 16 bytes (8 x 8 x 2 bpp)
        auto* pointer = &vram()[idx];
        tileset[i].populate_from_palette(pointer, &m_palette[0]);
    }

    // A special chunk of memory starting at 0x9800 or 0x9c00 is used to map which tile index
    // should be used to render. Each byte represents the index into the tile map.
    size_t map_start = bg_tilemap_display_select() - 0x8000;
    for (size_t i = 0; i < TOTAL_BG_TILES; ++i) {
        size_t tile_idx = vram()[map_start + i];
        size_t x = i % 32;
        size_t y = i / 32;
        fill_square(x, y, tileset[index_into_tileset(tile_idx)], m_bitmap);
    }

    for (size_t i = 0; i < TOTAL_TILESET_TILES; ++i) {
        size_t x = i % 32;
        size_t y = i / 32;
        fill_square(x, y, tileset[i], m_tileset_bitmap);
    }

    m_tilemap.set_data(m_bitmap);
    m_tileset.set_data(m_tileset_bitmap);
}

size_t PPU::index_into_tileset(size_t original_index)
{
    // * Block 0 is $8000-87FF
    // * Block 1 is $8800-8FFF
    // * Block 2 is $9000-97FF
    // When in 8000 mode:
    //   0-127 are in block 0
    //   128-255 are in block 1
    // When in 8800 mode:
    //   0-127 from block 2
    //   128-255 from block 1
    if (bg_window_tile_data_select() == 0x8000) {
        return original_index;
    } else if (bg_window_tile_data_select() == 0x8800) {
        if (original_index < 128) {
            // 0-127 is in Block 2. Index 0 is actually index 256
            return original_index + 256;
        } else {
            return original_index;
        }
    } else {
        ASSERT_NOT_REACHED();
    }
}

u8 PPU::in(u16 address)
{
    dbg() << "PPU::in() " << to_hex(address);

    if (address == R_SCX) {
        dbg() << "  handled!";
        return m_bg_scroll_x;
    }

    if (address == R_SCY) {
        dbg() << "  handled!";
        return m_bg_scroll_y;
    }

    if (address == R_LCDC)
        return m_lcd_control;

    // TODO: LCD Stat
    //    if (address == 0xff44) {
    //        // Tetris expects to receive 148 on the LCD stat otherwise it gets in an infinite loop
    //        return 148;
    //    }

    return 0;
}

void PPU::out(u16 address, u8 value)
{
    // TODO:
    //    PPU::out() 0xff40 - LCDC Status Interrupt
    //    PPU::out(0xff42, 0x0)
    //    PPU::out(0xff43, 0x0)
    //    PPU::out(0xff26, 0x0)
    dbg() << "PPU::out(" << to_hex(address) << ", " << to_hex(value) << ")";

    switch (address) {
    case R_SCX:
        dbg() << "  scx written";
        m_bg_scroll_x = value;
        break;
    case R_SCY:
        dbg() << "  scy written";
        m_bg_scroll_y = value;
        break;
    case R_LCDC:
        dbg() << "  lcd control written";
        m_lcd_control = value;
        break;
    case R_BGP:
        dbg() << "  background palette would be set";
        break;
    default:
        break;
    }
}

const LogStream& operator<<(const LogStream& stream, const Tile8x8& tile)
{
    for (size_t row = 0; row < 8; ++row) {
        for (size_t col = 0; col < 8; ++col) {
            u32 pixel = tile.pixel(col, row);
            if (pixel == Color::BLACK_ARGB) {
                stream.write(" B ", 3);
            } else {
                stream.write(" W ", 3);
            }
        }
        stream.write("\n", 1);
    }
    return stream;
}

const LogStream& operator<<(const LogStream& stream, const PPUMode& mode)
{
    switch (mode) {
    case PPUMode::AccessingOAM:
        stream << YELLOW "AccessingOAM" RESET;
        break;
    case PPUMode::AccessingVRAM:
        stream << GREEN "AccessingVRAM" RESET;
        break;
    case PPUMode::VerticalBlanking:
        stream << BLUE "VerticalBlanking" RESET;
        break;
    case PPUMode::HorizontalBlanking:
        stream << RED "HorizontalBlanking" RESET;
        break;
    }
    return stream;
}
