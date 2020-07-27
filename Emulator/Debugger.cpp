//
// Created by Spencer Dixon on 6/27/20.
//

#include "Debugger.h"
#include "Emulator.h"
#include "MemoryMap.h"

#include <SD/LogStream.h>

// Improvements:
// **** What is the CPU cycle counter and what is the PPU dot counter (need them to be very visible)
// * conditional breakpoints
// * render the PPU after every step
// * 'watch' breakpoints when an address in memory changes
// * break whenever this register has this value
// * dump all IO registers and state of PPU
// * scx/scy sorts of values to understand state better

// Implement these when it feels like they would be useful to solve problem at hand.

Debugger::Debugger(Emulator& emulator)
    : m_emulator(emulator)
{
}

void Debugger::enter()
{
    dbg() << "---------------------------";
    dbg() << "Entering SpenceBoy Debugger";
    dbg() << "---------------------------";
    dbg() << "\n " WHITE "Commands:\n" RESET;
    dbg() << "  q  | quit     - exit the debugger and the emulator";
    dbg() << "  s  | step     - step through to the next command";
    dbg() << "  c  | continue - continue execution without stepping until a breakpoint";
    dbg() << "  v  | vram     - dump contents of VRAM";
    dbg() << "  w  | wram     - dump contents of WRAM";
    dbg() << "  bp | <num>    - add breakpoint at program counter num\n\n";
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
    emulator().render_debug_frame();
    auto command_args = command.trim_whitespace().split(' ');
    auto primary_command = command_args[0];

    if (primary_command == "q" || primary_command == "quit" || primary_command == "exit")
        ::exit(0);

    if (primary_command == "s" || primary_command == "step") {
        auto op_code = emulator().cpu().execute_one_instruction();
        dbg() << "Executed OpCode: " << YELLOW << to_string(op_code).trim_whitespace() << RESET << "\n";
        dbg() << to_step_line(emulator().cpu().test_state());
    }

    if (primary_command == "c" || primary_command == "continue") {
        dbg() << "exiting debugger...";
        emulator().cpu().detach_debugger();
        return DebuggerResult::Exit;
    }

    if (primary_command == "v" || primary_command == "vram") {
        hex_dump("VRAM", emulator().mmu().vram(), VRAM_SIZE, VRAM_START);
    }

    if (primary_command == "w" || primary_command == "wram") {
        hex_dump("WRAM", emulator().mmu().wram(), WRAM_SIZE, WRAM_START);
    }

    if (primary_command == "bp") {
        auto bp = command_args[1].to_int();
        if (bp.is_none()) {
            dbg() << RED "error: " RESET "incompatible breakpoint (require an int)";
            return DebuggerResult::Continue;
        }

        emulator().cpu().add_breakpoint(bp.value());

        dbg() << GREEN "added: " RESET "breakpoint at " << bp.value();
    }

    return DebuggerResult::Continue;
}
