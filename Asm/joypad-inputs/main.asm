INCLUDE "../common/hardware.inc"

; The gameboy will load our code at address $100
SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

; Nintendo logo usually goes here
REPT $150 - $104
	db 0
ENDR

SECTION "Game code", ROM0

; Key status
KEY_START   EQU %10000000
KEY_SELECT  EQU %01000000
KEY_B       EQU %00100000
KEY_A       EQU %00010000
KEY_DOWN    EQU %00001000
KEY_UP      EQU %00000100
KEY_LEFT    EQU %00000010
KEY_RIGHT   EQU %00000001

; Reads current keypad status, stores into A register, where each bit corresponds to one key being pressed or not
; Keys are in the following order: Start - Select - B - A - Down - Up - Left - Right
; The constants KEY_START etc. corresponds to the values obtained here if only that key is pressed.
; The code is copied from the Gameboy Programming Manual, http://www.chrisantonellis.com/files/gameboy/gb-programming-manual.pdf
ReadKeys:
	push bc

	; Read D-pad
	ld a, $20
	ld [$FF00], a 
	ld a, [$FF00]
	ld a, [$FF00]
	cpl
	and %00001111
	ld b, a 

	; Read buttons (Start, Select, B, A)
	ld a, $10
	ld [$FF00], a 
	ld a, [$FF00]
	ld a, [$FF00]
	ld a, [$FF00]
	ld a, [$FF00]
	ld a, [$FF00]
	ld a, [$FF00]
	cpl
	and %00001111

	; Combine D-pad with buttons, store in B
	swap a 
	or b 
	ld b, a 

	ld a, $30
	ld [$FF00], a

	; Return the stored result
	ld a, b

	pop bc 
	ret

Start:
	call ReadKeys

	nop
	halt 
