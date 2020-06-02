//
// Created by Spencer Dixon on 6/1/20.
//

#pragma once

#include <SD/Types.h>

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

private:
    // Andreas: best way to get Key size at compile time?
    bool m_keys[8];
};
