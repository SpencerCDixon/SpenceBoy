# Daily Development Log

A place to store my daily progress in building a GameBoy Color Emulator.
Template for daily journal:

**Date: X/X/XXXX**
### What did I learn today?

### What did I get done today?

### Any questions for Andreas?

---

**Date: 6/4/2020**

### What did I get done today?

* Perf testing to find the cause of a slow ASM test
* Added a Timer() class for being able to get a quick sense of how long certain ops are taking

---

**Date: 6/3/2020**

### What did I get done today?

* Fixed SDL compiling on my macOS machine after breaking it yesterday lol
* Op code: JP_Z_a16
* Read in arrow key inputs and update IO RAM every frame
* ASM program that when pressing Right - Left - Up - Down will show stripes on the screen

---

**Date: 6/2/2020**

### What did I get done today?

* Fixed SDL compiling on my Linux machine so I can work here more.

---

**Date: 6/1/2020**

### What did I get done today?

* Started using cycle count of op codes inside emulator loop in order to get input handling working
* Optimize some byte manipulations

---

**Date: 5/31/2020**

### What did I learn today?

* Often GB devs will read from the joypad register a few times and only take the last reads values to ensure
any I/O issues are resolved

### What did I get done today?

* Fix broken test 'complex_subroutine' which broke when fixing the `ld bc, d16` op code. 
* Added an ASM test for reading joypad values
* Fix bug with reading IO RAM
* Implement op codes: LDH_a8_ADDR_A, LDH_A_a8_ADDR, AND_d8, LD_B_A, SWAP_A, OR_B, and LD_A_B

---

**Date: 5/30/2020**

### What did I learn today?

* Sometimes it can appear that my emulator is in an infinite loop when really it's just taking a super long time
to complete the program. In the future, be patient and keep an eye on decrementing counters when running in verbose
mode.

### What did I get done today?

* Add move constructor to custom String class
* Fix a super big bug in ld bc, d16 causing all sorts of issues. Byte order was flipped accidently
* Got stripes rendering successfully in SameBoy emulator with the help of some Gingerbread macros
* Got stripes rendering successfully in SpenceBoy PPU!
* Add support for opcode: LD_A_HL_ADDR_INC

---

**Date: 5/29/2020**

### What did I get done today?

* Stripe pattern ASM being rendered to screen
* Adds more load CPU instructions to aid in writing more robust assembly test
    programs

---

**Date: 5/28/2020**

### What did I get done today?

* Use the new Tile8x8 to render pixels into the frame buffer. Successfully generalized tile background rendering!
* Fix bug with how black/white pixel calculations were being done
* Zero out pixel data in Tiles to ensure no incorrect colors are being rendered

---

**Date: 5/27/2020**

### What did I get done today?

* Adds a Tile8x8 class with useful introspection to start getting the background tilemap
rendering properly

---

**Date: 5/26/2020**

### What did I get done today?

* Added CPU step traces to snapshot tests to get a better idea if I break things
* Refactored CPUSnapshot to use more String utilities
* Added a custom File class to make building test functionality easier to do

---

**Date: 5/25/2020**

### What did I get done today?

* String refactor to be less complicated and easier to reason about.
* Use new String in various call sites that were previously returning `const char *`s
* Refactor log stream format types to be cross platform so I can do more work on my Linux machine
* Add equality operators for String

---

**Date: 5/24/2020**

### What did I get done today?

* Basic String class that is probably leaking memory hardcore

---

**Date: 5/23/2020**

### What did I get done today?

* Start work on displaying all the tile map vram on screen to aid in creating a program that with real background

---

**Date: 5/22/2020**

### What did I get done today?

* to_string for op codes to improve verbose logging
* adds a stripes test cast to start rendering more than a single sprite

---
 
**Date: 5/21/2020**
### What did I learn today?

* `constexpr` expressions can be used at runtime and compile time
* Prefer `enum class` to raw `enum`s as it prevents possible scoping bugs. Also, the class variant can't be
coerced into an int automatically thus saving your skin from potential arguments being passed incorrectly.

### What did I get done today?

* Fix bug with how I was incrementing PC when doing call/returns
* Clean up Color class to be more idiomatic C++
* Research the String class in Serenity a bit to prepare how I would make a String class
* Added `to_string` utilities to CPU/CPUTestState which will be used to improve testing situation soon
 
---
 
**Date: 5/20/2020**

### What did I get done today?

* Flag getter
* Pushing and popping test case

---

**Date: 5/19/2020**

### What did I get done today?

* Enable/disable interrupt instruction
* LD_BC_d16 instruction
* Push/Pop allowing for first stack interaction!
* Adds HRAM access
* Call A16
* Relative jumps

---

**Date: 5/18/2020**
### What did I learn today?

* Pandocs has the boot up sequence for the GBC well documented which I'll need to implement exactly. It involves locations
in memory for specific instructions and a checksum that needs to match for the ROM to run.

### What did I get done today?

* Updated JS script for generating all op codes with their cycle count 
* Implemented xor for common registers

---

**Date: 5/17/2020**
### What did I learn today?

* There are a whole set of op codes that change based on a prefix of `cb`

### What did I get done today?

* 16 bit incrementing op codes
* op codes for complementing A reg bits
* ~20 new op codes for loading values into regs
* Create new asm test for loading calls
* Added support for prefixed op codes

### Any questions for Andreas?

* General commit strategy? He seems to have things namespaced on subsystem but maybe that's just because he has
such a large project

---

**Date: 5/16/2020**
### What did I learn today?

* C/C++ do not define evaluation order of arguments. This can lead to undefined behavior and extremely
hard to track down bugs.
* Learned about the shift and rotate op codes. Shift does 0 padding and rotate wraps the bits 

### What did I get done today?

* Fix some scary potential UB
* Wrote script for generating op codes that need to get implemented 

---

**Date: 5/15/2020**
### What did I learn today?

* The registers are supposed to have default values based on what kind of game boy is running which is why
my emulator had different register state at the end of execution as some that I tested

### What did I get done today?

* Jump A16 op code
* Default register state

---

**Date: 5/14/2020**
### What did I learn today?

* You can explore memory in LLDB with: `x &variable_address` and it will print out a 32 bytes of memory around that
address to view.
* A swizzle is how you determine the first LSB that is set. MSVC has a compiler intrinsic for this and most likely clang
does too. This is useful when working with masks.

### What did I get done today?

* Improve test case macros
* Got first sprite (smiley) rendering to the screen!
* Refactored op code handlers to use new register utilities

### Any questions for Andreas?

* Should I use friend class and protected state for my test harness state in the CPU

---

**Date: 5/13/2020**
### What did I learn today?

* How to use macros inside of macros for building my new test harness

### What did I get done today?

* Added snapshot testing infrastructure to assert programs run correctly on the CPU
* Added verbose logging level to CPU so I can adjust how noisy execution is
* Update test scripts to work with the new testing infrastructure

### Any questions for Andreas?

* I had trouble getting `memcmp` working properly in my test diffs. Instead, I compared raw bytes
myself. Is that bad?

---

**Date: 5/12/2020**
### What did I learn today?

* I learned about the LCDC bit flags and how they affect the PPU and which layer to render (i.e you can just render background layer).
* The built in Calculator.app has a programmer mode which is SUPER useful for hex stuff.

### What did I get done today?

* Add additional data to op code macros
* Improve hex dump utility
* Validated my smiley ASM is putting data in correct locations
* Came up with a game plan for getting first pixels rendered by the PPU
* Added more compiler flags to increase code quality 

---

**Date: 5/11/2020**
### What did I learn today?

* I learned that "placement new" exists which allows you to 'new' memory into a specific location
* I learned how to create an Option class using templates
* I learned how `&=` combined with `~` can be used to unset a specific bit

### What did I get done today?

* Added I/O Register memory
* Call with Andreas, created an Option class
* Moved OpCodes into a separate header with a useful macro for adding op code related functionality
* Added utilities for setting flags
* Fix bugs with flag setting and Load_Inc_HL_Addr_A/Inc_DE preventing me from writing to VRAM

---

**Date: 5/10/2020**
### What did I get done today?

* Fix flags being set in a few op code handlers
* A number of op codes which will be required to render pixels on the screen
* Refactor of main.c into an Emulator class which owns the window object
* Added an 'OffscreenFrameBuffer' which the PPU can use to render into
* Remove compilation errors detected in SDL2

### Any questions for Andreas?

* How to properly set half carry and carry flag. Confused how to best detect
    that a bit will change given some operation.

---

**Date: 5/9/2020**
### What did I learn today?

* How to use `calloc` to init zeroed out memory
* `constexpr` is guaranteed to have a value at compile time where `static const` could
have a value at runtime or compile time.

### What did I get done today?

* Added a 32K bank of RAM memory for the CPU to use
* `read`/`write` methods for accessing the RAM
* New OpCodes for loading and NoOp
* Ability to write to RAM from assembly

### Any questions for Andreas?

* When to use constexpr vs. DEFINE. Seems constexpr is the right move for compile time
constants if using C++11> but want to confirm.

---

**Date: 5/8/2020**
### What did I learn today?

* How to use stat to check the size of a file

### What did I get done today?

* Refactored CPU abstractions a bit to properly store ROM data and registers
* Can step through the execution of my basic loop ASM!

---

**Date: 5/7/2020**
### What did I learn today?

* Some weird difference in C++ between referencing a hex number as 0xC2 and '\xc2'.

### What did I get done today?

* Added the last of the opcodes I needed to parse for my first sample ASM program
* Added a struct for tracking register values
* Started thinking about the abstraction I'll use for 'Instructions' which are a combo of op codes and operands

### Any questions for Andreas?

* Why does he create a namespace for AK and then immediately use 'using AK:XXX'. For example:

```c++
namespace AK {

template<typename T, size_t Capacity>
class CircularQueue {
public:
    CircularQueue() {}
    ~CircularQueue() {
        clear();
    }
}
using AK::CircularQueue; // Why??
```

* I noticed you use m_*** for member names. What other conventions do you use? Do you use the m_*** name for structs as well?

* What is up with these special attributes:

```c++
#    define CONSUMABLE(initial_state) __attribute__((consumable(initial_state)))
#    define CALLABLE_WHEN(...) __attribute__((callable_when(__VA_ARGS__)))
```

---

**Date: 5/6/2020**
### What did I learn today?

* Opening files in C and iterating over byte stream 
* Only certain registers can be combined in the GBz80 to be turned into a 16 bit address
* I can use `hexdump -C my-rom.gb` to see the hex that the assembler generates

### What did I get done today?

* Created the CPU abstraction to start parsing Op Codes
* Set up a new test with a debug ROM to start parsing
* Installed new plugin to CLion to make it easier to view Hex/binary dumps
* Set up CLion to use ClangFormat properly

---

**Date: 5/5/2020**
### What did I learn today?

* Ubuntu/Linux are hard to set up and not bork :-(

### What did I get done today?

* Created a 'loop' in assembly that then halts

### Any questions for Andreas?

* How do I best verify my loop works properly? I'd like to step through my
    assembly one clock cycle at a time

---

**Date: 5/4/2020**
### What did I learn today?

* You can use `fwrite` to write 'n' number of objects of size T to a file descriptor.
* HBlank is the time between writing rows of pixels
* VBlank is the time between finishing writing a framebuffer and starting back at the top left
* A lot of assembly programming will set flags which can then be checked against in jumps
* A ton of assembly commands for the GBz80! (See research.md)

### What did I get done today?

* Fixed logging utility based on Andreas feedback

---

**Date: 5/3/2020**
### What did I learn today?

I learned that hex is so useful because it is 4x as compact as using bits. I've
always seen hex represented as 0xfff...etc. but apparently it is also common to
prefix a $ for hex. Therefore:

Hex -- Hex --- B10 -- Binary
$ff == 0xff == 255 == 11111111

I learned a new way to calculate modulo (%). You can subtract B from A until A
is smaller than B and you're left with the remainder. For example:

7 % 3 = 1

7 - 3 = 4, 4 - 3 = 1

### What did I get done today?

* Added a new `Types.h` to fix things I don't like about C/C++
* Added a a texture we can render into and filled it with white pixels
* Added a basic `LogStream` so I can start getting debug info printed to console
* Started researching GBC Assembly programming

### Any questions for Andreas?

* Best way to create a good dbg() logging utility? Mine is heavily inspired from SerenityOS
* Things to worry about security wise when using `printf` for my log stream
* Do I have to write my own assembler or can I use open source tools like [this
    one](https://github.com/rednex/rgbds)

---

**Date: 5/2/2020**
### What did I learn today?

I learned how to get SDL2 linking with CMake. I ended up doing it in a
somewhat hacky way which hard coded some paths. When I get my new Linux
machine I will look into using a `FindSDL2` type abstraction to maintain
the cross-platform capabilities of CMake.

### What did I get done today?

* Basic project structure
* SDL2 linking and window rendering
* Set up `clang-format` to match SerenityOS 
* Added a convenient `run.sh` script for command line iteration
* Set up basic test to validate CTest as a valid suite runner

### Any questions for Andreas?

* Am I allowed to look at his std lib abstractions?
* Why are a lot of his std-lib in `.h` and not `.cpp` files?
