; The gameboy will load our code at address $100
SECTION "Header", ROM0[$100]

; There is a bunch of header junk starting at $104 so we need to immediately jump to our code
EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "GameCode", ROM0

Start:
	ld a, $09
	cp a, $08

	ld a, $10
	cp a, $10

	ld b, $50
	cp a, b

	ld c, $60
	cp a, c

	ld d, $70
	cp a, d

	ld e, $80
	cp a, e

	ld h, $90
	cp a, h

	ld l, $a0
	cp a, l

	nop
	halt


