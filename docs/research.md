# Research

* [Pandoc CPU specs](http://bgb.bircd.org/pandocs.htm)
* [Solid blog post on high level implementation](https://mattbruv.github.io/gameboy-crust/)
* [Awesome GB Dev](https://github.com/gbdev/awesome-gbdev)
* [Building a GB Game](https://teamlampoil.se/book/gbasmdev.pdf)


## GBC - Game Boy Color

* Game Boy Color software can ask the console to double its CPU speed.
* 144h x 160w screen
* up to 40 sprites
* 8x8 or 8x16 pixels
* 32,768 colors
* 59.73 Hz refresh rate
* sound: two square channels, one voluntary (semi-flexible) wave channel, and one noise channel.
* PPU - Picture/Pixel Processing Unit
* APU - Audio Processing Unit

## Hex/Assembly/Dissembly

[Really good ASM tutorial for GBC](https://eldred.fr/gb-asm-tutorial/concepts.html)

* `$` and `0x` both refer to hexidecimal numbers
* one hex character (a nibble) can replace 4 binary digits (why hex is so useful!)
