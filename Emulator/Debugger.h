//
// Created by Spencer Dixon on 6/27/20.
//

#pragma once

#include <SD/String.h>

class Emulator;

enum class DebuggerResult {
    Exit,
    Continue,
};

class Debugger {
public:
    explicit Debugger(Emulator& emulator);

    void enter();
    DebuggerResult loop();

private:
    Emulator& emulator() { return m_emulator; }
    String prompt_for_input();
    DebuggerResult handle_command(String command);

private:
    Emulator& m_emulator;
};
