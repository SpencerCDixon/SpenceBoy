//
// Created by Spencer Dixon on 6/6/20.
//

#include "Emulator/Joypad.h"
#include <SD/Assertions.h>
#include <SD/Test.h>
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
    Joypad joypad;

    // DPad
    joypad.out(0, TURN_DPAD_MODE_ON);
    joypad.set_key_state(Key::Down, true);
    joypad.set_key_state(Key::Right, true);
    TEST_ASSERT(is_key_down(&joypad, KEY_DOWN_MASK), "key down");
    TEST_ASSERT(is_key_down(&joypad, KEY_RIGHT_MASK), "key right");
    TEST_ASSERT(!is_key_down(&joypad, KEY_UP_MASK), "key up");
    TEST_ASSERT(!is_key_down(&joypad, KEY_LEFT_MASK), "key left");
    dbg() << "Joypad state: " << joypad;

    // Buttons
    joypad.out(0, TURN_BUTTON_MODE_ON);
    joypad.set_key_state(Key::A, true);
    joypad.set_key_state(Key::Start, true);
    TEST_ASSERT(is_key_down(&joypad, KEY_A_MASK), "key a");
    TEST_ASSERT(is_key_down(&joypad, KEY_START_MASK), "key start");
    TEST_ASSERT(!is_key_down(&joypad, KEY_B_MASK), "key b");
    TEST_ASSERT(!is_key_down(&joypad, KEY_SELECT_MASK), "key select");
    dbg() << "Joypad state: " << joypad;
}
