//
// Created by Spencer Dixon on 6/27/20.
//

#include "Debugger.h"
#include "Emulator.h"
#include "MemoryMap.h"

#include <SD/LogStream.h>

Debugger::Debugger(Emulator& emulator)
    : m_emulator(emulator)
{
}

// TODO:
// * add a <num> after memory dump commands so I can just see a percentage of memory
void Debugger::enter()
{
    dbg() << "---------------------------";
    dbg() << "Entering SpenceBoy Debugger";
    dbg() << "---------------------------";
    dbg() << "\nCommands:\n";
    dbg() << "\tq | quit     - exit the emulator";
    dbg() << "\ts | step     - step through to next command";
    dbg() << "\tc | continue - continue execution without stepping";
    dbg() << "\tv | vram     - dump contents of VRAM";
    dbg() << "\tw | wram     - dump contents of WRAM";
    dbg() << "\tbp <num>     - add breakpoint at num\n\n";
}

DebuggerResult Debugger::loop()
{
    auto input = prompt_for_input();
    return handle_command(input);
}

String Debugger::prompt_for_input()
{
    printf(GREEN " >> " RESET);

    char* line = (char*)malloc(1024 * sizeof(char));
    fgets(line, 1024, stdin);
    String result = line;
    free(line);

    return result;
}

DebuggerResult Debugger::handle_command(String command)
{
    auto peeked_instruction = [&] {
        auto next = emulator().cpu().peek_next_instruction();
        if (next.is_none())
            return String("No More Instructions");

        return to_string(next.value()).trim_whitespace();
    };

    auto command_args = command.trim_whitespace().split(' ');
    auto primary = command_args[0];

    if (primary == "q" || primary == "quit" || primary == "exit")
        ::exit(0);

    if (primary == "s" || primary == "step") {
        auto op_code = emulator().cpu().execute_one_instruction();
        dbg() << "Executed OpCode: " << YELLOW << to_string(op_code).trim_whitespace() << RESET << "\n";
        dbg() << to_step_line(emulator().cpu().test_state());
    }

    if (primary == "p" || primary == "peek") {
        dbg() << "Next OpCode: " << YELLOW << peeked_instruction() << RESET;
    }

    if (primary == "c" || primary == "continue") {
        dbg() << "exiting debugger...";
        emulator().cpu().detach_debugger();
        return DebuggerResult::Exit;
    }

    if (primary == "v" || primary == "vram") {
        hex_dump("VRAM", emulator().mmu().vram(), VRAM_SIZE, VRAM_START);
    }

    if (primary == "w" || primary == "wram") {
        hex_dump("WRAM", emulator().mmu().wram(), WRAM_SIZE, WRAM_START);
    }

    if (primary == "bp") {
        dbg() << "Would set breakpiont at: " << command_args[1];
    }

    return DebuggerResult::Continue;
}
