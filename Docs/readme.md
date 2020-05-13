# SpenceBoy - A GameBoy Color Emulator

This project is a GameBoy Color emulator. The primary objective is for
educational purposes. No external dependencies are allowed other than `SDL2` and
the `C` standard library.

# Roadmap

## General 

- [x] Set up a separate library to store standard lib functionality
- [x] Set up a basic CMake test 
- [ ] Add ROM conformation tests to ensure I'm as compliant as possible
- [x] Remove CMake Tests for my own harness which does diffs on traces of different .gb files 

## Standard Lib

- [ ] String
- [ ] Unique Pointer
- [ ] Shared Pointer
- [ ] Vector
- [ ] Option
- [ ] Hash Map
- [ ] File I/O
- [x] Assertions
- [x] Basic Logging
- [x] Color
- [ ] Pair/Tuple (useful for converting u16 into two u8's etc.)

## Emulator

- [X] Get a GameBoy "development environment" (meaning a GameBoy assembler and disassembler)
- [X] Write a test program in GameBoy assembly that loops 3 times and then halts execution
- [x] Build a simple CPU instruction interpreter that can read instructions from a stream of bytes and call an appropriate handler for each instruction (no need to implement all instructions yet!)
- [x] Get your program from (1) to run in the interpreter from (2), printing out the state of the CPU at every step of execution
- [x] Add RAM (really just a byte buffer) to your CPU and implement instructions for moving memory in/out of RAM
- [x] Figure out how the GameBoy ROM file format works, and parse_rom a ROM into your emulator's memory at startup
- [ ] Build a simple PPU renderer that looks at all the PPU state in the emulator and renders the "screen" into an SDL window

- [ ] Stack push/pop
- [ ] Interrupts
- [ ] Finish rest of the op codes
- [ ] Call/return from ASM 'functions'

## Testing Infra

- [ ] Add flags at end of execution to snapshots

## Game Targets

– [ ] Get super mario land to render on screen
- [ ] Tetris 

## Chores

- [ ] Get CMake script for SDL working on Linux/Windows
- [ ] Get Clang Tidy set up 
