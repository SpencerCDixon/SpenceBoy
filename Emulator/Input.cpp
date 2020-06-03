//
// Created by Spencer Dixon on 6/1/20.
//

#include "Input.h"


void Input::set_key_state(const Key& key, bool is_down)
{
    m_keys[static_cast<int>(key)] = is_down;
}

const LogStream& operator<<(const LogStream& stream, const Input& input)
{
    stream << "Input: Right=" << input.is_right_down() << " Left=" << input.is_left_down();
    return stream;
}
