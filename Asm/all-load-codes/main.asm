INCLUDE "../common/hardware.inc"

; The gameboy will load our code at address $100
SECTION "Header", ROM0[$100]

; There is a bunch of header junk starting at $104 so we need to immediately jump to our code
EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "Game code", ROM0

Start:
	ld b, %00000001
	ld b, b
	; assert(b == 1)

	ld d, b
	ld h, b
	; assert(d == 1)
	; assert(h == 1)

	ld c, 42
	ld b, c
	; assert(b == 42)

	ld c, 50
	ld d, c
	ld h, c
	; assert(d == 50)
	; assert(h == 50)

	ld c, 32
	ld h, $c0
	ld l, $00
	ld [hl], c
	; assert(ram[0] == 32)

	ld a, 32
	ld b, $c0
	ld c, $02
	ld [bc], a
	; assert(ram[3] == 32)

	ld a, 32
	ld d, $c0
	ld e, $04
	ld [de], a
	; assert(ram[4] == 32)

	ld a, 33
	ld [hl+], a
	ld [hl+], a
	; assert(ram[0] == 33)
	; assert(ram[1] == 33)

	dec hl
	ld a, 48
	ld [hl-], a
	ld [hl-], a
	; assert(ram[0] == 48)
	; assert(ram[1] == 48)

	; ld b, 52
	; ld h, $c0
	; ld l, $02
	; ld [hl], b

	halt
