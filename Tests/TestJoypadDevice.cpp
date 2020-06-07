//
// Created by Spencer Dixon on 6/6/20.
//

#include "Emulator/Joypad.h"
#include <SD/Assertions.h>
#include <SD/LogStream.h>

constexpr u8 TURN_BUTTON_MODE_ON = 0x10;
constexpr u8 TURN_DPAD_MODE_ON = 0x20;

bool is_key_down(Joypad* joypad, u8 key_mask)
{
    // 0: pressed 1: not pressed
    return !(joypad->in(0) & key_mask);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    dbg() << "Running Joypad Tests..";

    Joypad joypad;

    // DPad
    joypad.out(0, TURN_DPAD_MODE_ON);
    joypad.set_key_state(Key::Down, true);
    joypad.set_key_state(Key::Right, true);
    ASSERT(is_key_down(&joypad, KEY_DOWN_MASK));
    ASSERT(is_key_down(&joypad, KEY_RIGHT_MASK));
    ASSERT(!is_key_down(&joypad, KEY_UP_MASK));
    ASSERT(!is_key_down(&joypad, KEY_LEFT_MASK));
    dbg() << "Joypad state: " << joypad;

    // Buttons
    joypad.out(0, TURN_BUTTON_MODE_ON);
    joypad.set_key_state(Key::A, true);
    joypad.set_key_state(Key::Start, true);
    ASSERT(is_key_down(&joypad, KEY_A_MASK));
    ASSERT(is_key_down(&joypad, KEY_START_MASK));
    ASSERT(!is_key_down(&joypad, KEY_B_MASK));
    ASSERT(!is_key_down(&joypad, KEY_SELECT_MASK));
    dbg() << "Joypad state: " << joypad;
}