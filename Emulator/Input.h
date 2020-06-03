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

    bool is_right_down() const { return m_keys[static_cast<int>(Key::Right)]; }
    bool is_left_down() const { return m_keys[static_cast<int>(Key::Left)]; }

private:
    // Andreas: best way to get Key size at compile time?
    bool m_keys[8];
};

const LogStream& operator<<(const LogStream& stream, const Input& input);
