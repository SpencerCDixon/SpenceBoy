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

USER_RAM_START     EQU $C200 ; up to $E000

; Sets data to a constant value in a way that is NOT safe to use when writing to VRAM while LCD is on (but faster than mSetVRAM)
; A  - constant value to set
; HL - memory position of the start of the copying destination
; BC - the number of bytes to be written 
; mSet:
    ; inc b
    ; inc c
    ; jr  .skip
; .loop:
    ; ld [hl+], a 
; .skip:
    ; dec c
    ; jr  nz, .loop
    ; dec b
    ; jr nz, .loop
    ; ret

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
