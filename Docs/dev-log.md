# Daily Development Log

A place to store my daily progress in building a GameBoy Color Emulator.
Template for daily journal:

**Date: X/X/XXXX**
### What did I learn today?

### What did I get done today?

### Any questions for Andreas?

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

### Any questions for Andreas?

* None today! Time to write more GBC assembly :)

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