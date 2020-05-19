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
	; Reset register values 
	ld a, 0
	ld b, 0
	ld c, 0
	ld d, 0
	ld e, 0
	ld h, 0
	ld l, 0
	ld sp, 0

	; Multi-register incs don't change flags at all
	inc bc
	inc de
	inc hl
	inc sp

	; TODO: explicit tests that half carry and zero flags are getting set
	; and that subtract flag gets reset.
	inc a
	inc b
	inc c
	inc d
	inc e
	inc h
	inc l

	halt
