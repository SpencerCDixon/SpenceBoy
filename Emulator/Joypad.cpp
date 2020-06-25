//
// Created by Spencer Dixon on 6/1/20.
//

#include "Joypad.h"
#include "Emulator.h"
#include <SD/Assertions.h>
#include <SD/Bytes.h>

Joypad::Joypad()
    : m_mode(JoypadReadMode::None)
{
    for (int i = 0; i < static_cast<int>(Key::_Count); ++i)
        m_keys[i] = false;
}

u8 Joypad::in(u16)
{
    // Note: We only have 1 address (0xff00) being used for Joypad
    // related logic so we don't need to use the address
    // and it can be safely ignored.

    // Remember this weirdness:
    // 0 -> pressed   1 -> not pressed
    u8 result = 0x0f; // Assume none pressed

    if (m_mode == JoypadReadMode::DPad) {
        result |= 0x20;

        if (is_down_down())
            result ^= KEY_DOWN_MASK;
        if (is_up_down())
            result ^= KEY_UP_MASK;
        if (is_right_down())
            result ^= KEY_RIGHT_MASK;
        if (is_left_down())
            result ^= KEY_LEFT_MASK;
    } else if (m_mode == JoypadReadMode::Button) {
        result |= 0x10;

        if (is_a_down())
            result ^= KEY_A_MASK;
        if (is_b_down())
            result ^= KEY_B_MASK;
        if (is_start_down())
            result ^= KEY_START_MASK;
        if (is_select_down())
            result ^= KEY_SELECT_MASK;
    } else {
        dbg() << "Program was trying to read Joypad data but didn't have the IODevice "
              << "in a proper state to be able to get useful data back (0x10 or 0x20)";
//        ASSERT_NOT_REACHED();
    }

    return result;
}

void Joypad::out(u16, u8 value)
{
    // 0010 0000 - Read DPad
    // 0001 0000 - Read Buttons
    // 0011 0000 - None
    // 0000 1010 - what happens here? TODO: Write test program which determines how other emulators deal with this write (both 00 or 11)
    if ((value & 0x20) && !(value & 0x10)) {
        set_mode(JoypadReadMode::DPad);
    } else if ((value & 0x10) && !(value & 0x20)) {
        set_mode(JoypadReadMode::Button);
    } else {
        set_mode(JoypadReadMode::None);
    }
}

void Joypad::set_key_state(const Key& key, bool is_down)
{
    m_keys[static_cast<int>(key)] = is_down;
}

const LogStream& operator<<(const LogStream& stream, const Joypad& input)
{
    stream << "Joypad Bitmask: " << to_bits(const_cast<Joypad&>(input).in(0));
    return stream;
}

//
// InputDebugWindow
//

InputDebugWindow::InputDebugWindow(Emulator& emulator)
    : m_emulator(emulator)
{
    auto a_path = emulator.assets_dir() + "/A.png";
    m_a_tex = Texture::from_image(a_path);
    auto b_path = emulator.assets_dir() + "/B.png";
    m_b_tex = Texture::from_image(b_path);
    auto start_path = emulator.assets_dir() + "/Start.png";
    m_start_tex = Texture::from_image(start_path);
    auto select_path = emulator.assets_dir() + "/Select.png";
    m_select_tex = Texture::from_image(select_path);
    auto arrow_path = emulator.assets_dir() + "/Arrow.png";
    m_arrow_tex = Texture::from_image(arrow_path);
}

InputDebugWindow::~InputDebugWindow()
{
}

void InputDebugWindow::render()
{
    constexpr Color DOWN_COLOR = { 220, 10, 10, 255 };
    constexpr Color UP_COLOR = { 255, 255, 255, 255 };

    m_a_tex.color(emulator().joypad().is_a_down() ? DOWN_COLOR : UP_COLOR);
    m_emulator.renderer().draw_texture(m_a_tex, Point { 254, 315 });

    m_b_tex.color(emulator().joypad().is_b_down() ? DOWN_COLOR : UP_COLOR);
    m_emulator.renderer().draw_texture(m_b_tex, Point { 205, 330 });

    m_arrow_tex.color(emulator().joypad().is_up_down() ? DOWN_COLOR : UP_COLOR);
    m_emulator.renderer().draw_texture_rotated(m_arrow_tex, Point { 76, 303 }, 0);   // up

    m_arrow_tex.color(emulator().joypad().is_right_down() ? DOWN_COLOR : UP_COLOR);
    m_emulator.renderer().draw_texture_rotated(m_arrow_tex, Point { 97, 324 }, 90);  // right

    m_arrow_tex.color(emulator().joypad().is_down_down() ? DOWN_COLOR : UP_COLOR);
    m_emulator.renderer().draw_texture_rotated(m_arrow_tex, Point { 76, 345 }, 180); // down

    m_arrow_tex.color(emulator().joypad().is_left_down() ? DOWN_COLOR : UP_COLOR);
    m_emulator.renderer().draw_texture_rotated(m_arrow_tex, Point { 55, 324 }, 270); // left

    m_start_tex.color(emulator().joypad().is_start_down() ? DOWN_COLOR : UP_COLOR);
    m_emulator.renderer().draw_texture(m_start_tex, Point { 169, 421 });

    m_select_tex.color(emulator().joypad().is_select_down() ? DOWN_COLOR : UP_COLOR);
    m_emulator.renderer().draw_texture(m_select_tex, Point { 112, 421 });
}
