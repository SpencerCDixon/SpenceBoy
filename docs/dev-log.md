# Daily Development Log

A place to store my daily progress in building a GameBoy Color Emulator.
Template for daily journal:

**Date: X/X/XXXX**
### What did I learn today?

### What did I get done today?

### Any questions for Andreas?

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
