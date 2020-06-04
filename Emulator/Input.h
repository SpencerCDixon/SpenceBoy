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
};

class Input {
public:
    void set_key_state(const Key& key, bool is_down);

    // TODO: Complete Start/Select
    bool is_up_down() const { return m_keys[static_cast<int>(Key::Up)]; }
    bool is_down_down() const { return m_keys[static_cast<int>(Key::Down)]; }
    bool is_right_down() const { return m_keys[static_cast<int>(Key::Right)]; }
    bool is_left_down() const { return m_keys[static_cast<int>(Key::Left)]; }
//    bool is_a_down() const { return m_keys[static_cast<int>(Key::A)]; }
//    bool is_b_down() const { return m_keys[static_cast<int>(Key::B)]; }

    u8 to_bit_mask() {
        u8 result = 0;

        if (is_right_down())
            result |= 0x01;

        if (is_left_down())
            result |= 0x02;

        if (is_up_down())
            result |= 0x04;

        if (is_down_down())
            result |= 0x08;

        // TODO: Complete masks for all controls
        return result;
    }

private:
    // Andreas: best way to get Key size at compile time?
    bool m_keys[8];
};

const LogStream& operator<<(const LogStream& stream, const Input& input);
