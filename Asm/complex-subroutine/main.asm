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

USER_RAM_START     EQU $C000 ; up to $E000

; Sets data to a constant value in a way that is NOT safe to use when writing to VRAM while LCD is on (but faster than mSetVRAM)
; A  - constant value to set
; HL - memory position of the start of the copying destination
; BC - the number of bytes to be written 
mSet:
	inc b
	inc c
	jr  .skip
.loop:
	ld [hl+], a 
.skip:
	dec c
	jr  nz, .loop
	dec b
	jr nz, .loop
	ret

Start:
	nop
	di

	; Load stack pointer into HRAM
	ld sp, $ffff 

	ld hl, USER_RAM_START
	ld bc, $1000

	ld a, $02
	; Set first 16 bytes of WRAM to 02
	call mSet

	xor a
	xor b

	halt 
