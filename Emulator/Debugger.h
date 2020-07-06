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
    void exit();
    void repl();

private:
    String prompt_for_input();
    bool handle_command(String command);

private:
    Emulator& m_emulator;
};
