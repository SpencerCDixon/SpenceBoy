# Research

## Resources

* [Pandoc CPU specs](http://bgb.bircd.org/pandocs.htm)
* [Solid blog post on high level implementation](https://mattbruv.github.io/gameboy-crust/)
* [Awesome GB Dev](https://github.com/gbdev/awesome-gbdev)
* [Building a GB Game](https://teamlampoil.se/book/gbasmdev.pdf)
* [Acceptance tests](https://github.com/Gekkio/mooneye-gb)
* [Useful walk-through of looping in GB ASM](https://medium.com/@ulrikdamm/writing-your-first-gameboy-game-4ea62c76db29)
* [ROM tests](https://gbdev.gg8.se/files/roms/blargg-gb-tests/)
* [Ultimate GameBoy talk](https://www.youtube.com/watch?v=HyzD8pNlpwI)
* [Lots of ROMs](http://nesninja.com/public/GoodGBx%203.14/)

## GBC - Game Boy Color (GBz80)

* Game Boy Color software can ask the console to double its CPU speed.
* 144h x 160w screen
* up to 40 sprites
* 8x8 or 8x16 pixels
* 32,768 colors
* 59.73 Hz refresh rate
* sound: two square channels, one voluntary (semi-flexible) wave channel, and one noise channel.
* PPU - Picture/Pixel Processing Unit
* APU - Audio Processing Unit
* eight registers: a, f, b, c, d, e, h, l (each of them is 1 byte)

## Hex/Assembly/Dissembly 

[Really good ASM tutorial for GBC](https://eldred.fr/gb-asm-tutorial/concepts.html)

* `$` and `0x` both refer to hexidecimal numbers
* one hex character (a nibble) can replace 4 binary digits (why hex is so useful!)

```asm
GlobalLabel: ; This is a global label that has a ':'
  ld h1, .local ; local is local to 'GlobalLabel' and refers to db $FF's address.

.local   
  db $FF
  ld e, a ; random!
```

Since labels just refer to memory addresses we can do arithmetic on them!
Huzzah!

#### GB ASM Rules

> **Labels must be at the beginning of a line**!

> **Instructions may not be at the begin of a line**!

* The `f` register is reserved for flags
* Registers can be combined: `b` and `c` can be combined to `bc` and hold 16-bit
    value

**Commands:**

* `ld dest, source` - LOAD value from source into dest, DOES NOT AFFECT FLAGS
* `inc` - increment
* `dec` - decrement
* `add a, X` - add, adds 'X' to 'a' and return back 'a'
* `sub a, X` - subtract 'X' from 'a'
* `cp a, X` - compare 'a' and 'X' - use the flags to determine conditional
* `and`, `or`, `xor` - all operate on the 'a' register
* `daa` - decimal adjust on the accumulator. Turns hex into base 10 on 'a' register
* `nop` - do nothing!
* `scf` - sets the carry flag
* `ccf` - complements (invert) the carry flag

**Modify the PC**
* `jp X` - jump to another place in memory (sets the pc to X)
* `jr X` - jump but **relative** to the `pc`. X is the offset
* `call`
* `rst`
* `ret`

## Jumping!

**Jump Mneumonic**
* Non-zero - nz - Zero reset
* Zero - z - Zero set
* No carry - nc - Carry reset
* Carry - c - Carry set

Example:

```asm
ld a, [wPlayerHP]
and a ; Is it 0?
jp z, GameOver ; If HP hit 0, play Game Over animation
cp 10 ; Is it under 10 HP?
jr nc, .noLowHealthBeep ; If above 10 HP, don't beep
```

> Remember that labels are references to memory locations, which make them very common operands for `jp`

> I recommend using jr by default, and jp when it can't be used

## Tips

> When 'a' is the left operand it can be omitted

`add $38` == `add a, $38`

> Checking for if a equals 0

* `and a` 
* `or a`

> Setting 'a' to 0:

* `xor a` instead of `ld a, 0`

> Clear the carry flag:

* `and a` - clears carry but also sets the Z flag

```asm
ld c, $FE
ld b, $CA
; BC = $CAFE now
```

Accessing memory:

> While $C000 represents the number $C000, [$C000] represents the byte at address $C000.

> You can use the 0b-prefix to specify binary numbers, which is often easier to handle than hex codes for bit fields

```asm
ld [hl], 0b1110_0100
```

> A quick way to stop execution of a CPU:

```asm
; Stop execution
end: jp end
```

## The F Register

* 8-bit register
* Bits 0-3 are ALWAYS zero
* Bits 4-7 are called C, H, N, Z

C: Carry - When incrementing 1111 1111 the 9th bit will be "stored" here by setting the flag
H: Half-carry - Like the carry but only on lower 4 bits. $08 + $39 would set the H flag.
N: Add/Subtract
Z: Zero - set if the instructions result was zero

## PC Flow

1. The byte [pc] is read (this means that if PC = $1234, the byte at $1234 is read)
2. pc is incremented
3. That byte is decoded so the CPU knows which instruction it should execute
4. If the instruction has operand byte(s), it is/they are read as well, one by one, and pc is incremented after each read
5. The instruction is executed proper

## Displaying

* 144px tall by 160px wide
* 60 FPS
* LCD pixels are refreshed line by line (like the old CRTs)
* HBlank - when the line finishes drawing and is moving to the next row
* VBlank - all lines (rows) have been drawn and moving back to the top left

`LY` register is used to determine info about the display:
* 0-143 - the current line being drawn
* 144-153 - in VBlank period
* can read but not write to LY. See:

```asm
  ; Wait until VBlank
.notVBlanking
  ldh a, [rLY] ; Read the LCD's drawing status
  ; NOTE: ldh is a shorter and faster version of ld for addresses in the $FF00-$FFFF range
  cp 144
  jr c, .notVBlanking ; The LCD is VBlanking between LY 144 and 153
```

* VRAM is in memory between `$8000` and `$9FFF`

> the CPU cannot access VRAM while the PPU is accessing it

* The screen can scroll X/Y with `SCX` ($FF42) and `SCY` ($FF43). These two
    registers specify the coords of the top left of the screen
* When reaching edge of tilemap it wraps around
* Colors are picked from a color pallete (`BGP`)

#### Display Steps:
1. Copy tiles to VRAM
1. Write to tilemap
1. Set scrolling (SCY, SCX)
1. Set palette
