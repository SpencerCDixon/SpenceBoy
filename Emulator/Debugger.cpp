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
    dbg() << "\tw | wram     - dump contents of WRAM\n\n";

    dbg() << m_emulator.cpu().test_state();

    repl();
}

void Debugger::exit()
{
    // TODO:
}

void Debugger::repl()
{
    auto input = prompt_for_input();
    while (handle_command(input))
        repl();
}

String Debugger::prompt_for_input()
{
    printf(" >> ");

    char* line = (char*)malloc(1024 * sizeof(char));
    fgets(line, 1024, stdin);
    String result = line;
    free(line);

    return result;
}

bool Debugger::handle_command(String command)
{
    auto trimmed = command.trim_whitespace_right();

    if (trimmed == "q" || trimmed == "quit")
        ::exit(0);

    if (trimmed == "s" || trimmed == "step") {
        dbg() << "  stepping the CPU one cycle";
        m_emulator.cpu().step();
        dbg() << m_emulator.cpu().test_state();
    }

    if (trimmed == "c" || trimmed == "continue") {
        dbg() << "exiting debugger...";
        return false;
    }

    if (trimmed == "v" || trimmed == "vram") {
        hex_dump("VRAM", m_emulator.mmu().vram(), VRAM_SIZE, VRAM_START);
    }

    if (trimmed == "w" || trimmed == "wram") {
        hex_dump("WRAM", m_emulator.mmu().wram(), WRAM_SIZE, WRAM_START);
    }

    return true;
}
