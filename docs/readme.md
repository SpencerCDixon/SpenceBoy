# GameBoy Emulator

This project is a GameBoy Color emulator. The primary objective is for
educational purposes. No external dependencies are allowed other than `SDL2` and
the `C` standard library.

# Roadmap

## General 

- [ ] Set up a separate library to store standard lib functionality
- [ ] Set up a basic CMake test 

## Standard Lib

- [ ] String
- [ ] Unique Pointer
- [ ] Shared Pointer
- [ ] Vector
- [ ] Hash Map
- [ ] File I/O
- [ ] Basic Logging

## Emulator

- [ ] Get a GameBoy "development environment" (meaning a GameBoy assembler and disassembler)
- [ ] Write a test program in GameBoy assembly that loops 3 times and then halts execution
- [ ] Build a simple CPU instruction interpreter that can read instructions from a stream of bytes and call an appropriate handler for each instruction (no need to implement all instructions yet!)
- [ ] Get your program from (1) to run in the interpreter from (2), printing out the state of the CPU at every step of execution
- [ ] Add RAM (really just a byte buffer) to your CPU and implement instructions for moving memory in/out of RAM
- [ ] Figure out how the GameBoy ROM file format works, and load a ROM into your emulator's memory at startup
- [ ] Build a simple PPU renderer that looks at all the PPU state in the emulator and renders the "screen" into an SDL window

