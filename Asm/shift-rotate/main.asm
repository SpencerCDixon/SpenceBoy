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
	ld a, %11000011;
	sla a
	; assert(b == 10000110)
	; assert(flag.carry)

	ld b, %10000000;
	sla b
	; assert(b == 0)
	; assert(flag.zero)

	ld c, %00001111
	sla c

	ld d, %00000001
	sla d

	ld e, %00000001
	sla e

	ld h, %00000001
	sla h

	ld l, %00000001
	sla l

	halt
