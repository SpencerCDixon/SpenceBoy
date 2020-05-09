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

Start:
	nop
	ld a, 3 ; load a register with 3

loop:
	; do some work in our loop
	dec a
	jp nz, loop

	; Save 52 to some place in RAM
	ld b, 52
	ld h, $c0
	ld l, $02
	ld [hl], b

subtract_by_12:
	ld a, [hl]
	; sub 12
	; ld [hl], a
	; ret
