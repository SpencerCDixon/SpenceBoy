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

WaitForNonBusyLCD: MACRO
    ld  a,[rSTAT]   
    and STATF_BUSY  
    jr  nz,@-4     ; Jumps up 4 bytes in the code (two lines in this case)
ENDM

Start:
	ld a, $20
	WaitForNonBusyLCD
	ld a, $42

; Stop
	nop
	halt
