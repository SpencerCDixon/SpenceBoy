//
// Created by Spencer Dixon on 6/1/20.
//

#pragma once

#include <SD/Types.h>
#include <SD/LogStream.h>

enum class Key: u8 {
    Right,
    Left,
    Up,
    Down,
    A,
    B,
    Start,
    Select,
    _Count,
};

// clang-format off
constexpr u8 KEY_RIGHT_MASK  = (1 << 0);
constexpr u8 KEY_LEFT_MASK   = (1 << 1);
constexpr u8 KEY_UP_MASK     = (1 << 2);
constexpr u8 KEY_DOWN_MASK   = (1 << 3);
constexpr u8 KEY_A_MASK      = (1 << 4);
constexpr u8 KEY_B_MASK      = (1 << 5);
constexpr u8 KEY_SELECT_MASK = (1 << 6);
constexpr u8 KEY_START_MASK  = (1 << 7);
// clang-format on

class Input {
public:
    void set_key_state(const Key& key, bool is_down);

    bool is_up_down() const { return m_keys[static_cast<int>(Key::Up)]; }
    bool is_down_down() const { return m_keys[static_cast<int>(Key::Down)]; }
    bool is_right_down() const { return m_keys[static_cast<int>(Key::Right)]; }
    bool is_left_down() const { return m_keys[static_cast<int>(Key::Left)]; }
    bool is_a_down() const { return m_keys[static_cast<int>(Key::A)]; }
    bool is_b_down() const { return m_keys[static_cast<int>(Key::B)]; }
    bool is_start_down() const { return m_keys[static_cast<int>(Key::Start)]; }
    bool is_select_down() const { return m_keys[static_cast<int>(Key::Select)]; }

    u8 to_bit_mask() const
    {
        u8 result = 0;

        if (is_right_down())
            result |= KEY_RIGHT_MASK;

        if (is_left_down())
            result |= KEY_LEFT_MASK;

        if (is_up_down())
            result |= KEY_UP_MASK;

        if (is_down_down())
            result |= KEY_DOWN_MASK;

        if (is_a_down())
            result |= KEY_A_MASK;

        if (is_b_down())
            result |= KEY_B_MASK;

        if (is_select_down())
            result |= KEY_SELECT_MASK;

        if (is_start_down())
            result |= KEY_START_MASK;

        return result;
    }

private:
    bool m_keys[static_cast<int>(Key::_Count)];
};

class InputDebugWindow {
public:
    InputDebugWindow();

private:

    SDL_Renderer* m_renderer { nullptr };
};

const LogStream& operator<<(const LogStream& stream, const Input& input);
