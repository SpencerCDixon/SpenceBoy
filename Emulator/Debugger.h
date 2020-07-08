//
// Created by Spencer Dixon on 6/27/20.
//

#pragma once

#include <SD/String.h>

class Emulator;

//enum class DebuggerCommand {
//    Step,
//    Continue,
//    SetBreakpoint,
//    DeleteBreakpoint
//};

class Debugger {
public:
    explicit Debugger(Emulator& emulator);

    void enter();
    void repl(bool should_continue = true);

private:
    Emulator& emulator() { return m_emulator; }
    String prompt_for_input();
    bool handle_command(String command);

private:
    Emulator& m_emulator;
};
