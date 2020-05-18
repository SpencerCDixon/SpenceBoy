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
	; Assert carry in shift works properly
	ld a, 1;
	xor a

	ld a, %00001010;
	; assert(a == 0)

	ld b, %00001111
	xor b 

	ld c, %11110000
	xor c 

	ld d, %00001111
	xor d 

	ld e, %00001111
	xor e 

	ld h, %00001111
	xor h 

	ld l, %00001111
	xor l 

	halt
