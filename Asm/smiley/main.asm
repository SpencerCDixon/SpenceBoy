INCLUDE "hardware.inc"

; The gameboy will load our code at address $100
SECTION "Header", ROM0[$100]

; There is a bunch of header junk starting at $104 so we need to immediately jump to our code
EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "Game code", ROM0

; Smiley based off: https://medium.com/@ulrikdamm/writing-your-first-gameboy-game-4ea62c76db29

Start:
	ld hl, $9010
	ld de, smiley_sprite
	ld b, 16

copy_loop:
	ld a, [de]
	inc de
	ld [hl+], a
	dec b
	jp nz, copy_loop

	ld hl, $9800
	ld [hl], 1

	ld hl, $ff47
	ld [hl], $e4

	ld hl, $ff40
	ld [hl], $81

; Stop
	nop
	halt

smiley_sprite: 
	db $00, $00, $00, $00, $24, $24, $00, $00, $81, $81, $7e, $7e, $00, $00, $00, $00

