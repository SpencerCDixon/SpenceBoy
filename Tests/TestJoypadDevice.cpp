//
// Created by Spencer Dixon on 6/6/20.
//

#include "Emulator/Joypad.h"
#include <SD/Assertions.h>
#include <SD/LogStream.h>

constexpr u8 TURN_BUTTON_MODE_ON = 0x10;
constexpr u8 TURN_DPAD_MODE_ON = 0x10;

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    dbg() << "Running Joypad Tests..";

    Joypad joypad;

    // Buttons
    joypad.set_key_state(Key::Down, true);
    joypad.set_key_state(Key::Right, true);
    ASSERT(joypad.is_down_down());
    ASSERT(joypad.is_right_down());

    joypad.write(0, TURN_BUTTON_MODE_ON);

    ASSERT(!joypad.is_a_down());
}