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

setBAndA:
	ld a, $02
	ld b, $02
	ret
	
Start:
	; Load 4s
	ld b, $04 
	ld c, $04 

	; Reset
	xor d
	xor e

	; transfer bc to de
	push bc
	pop de

	; Load stack pointer into HRAM
	ld sp, $ffff 

	; Reset a & b
	xor a 
	xor b 

	; Call and return to set a and b to 2
	call setBAndA

	; Override b to 6
	ld b, $06 ; just a sanity check

	; assert(a == 2)
	; assert(b == 6)
	; assert(c == 4)
	; assert(d == 4)
	; assert(e == 4)

	halt 
