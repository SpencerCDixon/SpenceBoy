INCLUDE "../common/hardware.inc"

SECTION "Header", ROM0[$100]

EntryPoint:
	di ; Disable interrupts
	jp Start ; 

REPT $150 - $104
	db 0
ENDR

SECTION "Game code", ROM0

Start:

; .waitVBlank
	; ld a, [rLY]
	; cp 144
	; jr c, .waitVBlank

	xor a ; ld a, 0
	ld [rLCDC], a

	ld hl, $9000
	ld de, FontTiles
	ld bc, FontTilesEnd - FontTiles
.copyFont
	ld a, [de]
	ld [hli], a
	inc de
	dec bc
	ld a, b
	or c
	jr nz, .copyFont

	ld hl, $9800 ; This will print the string at the top-left corner of the screen
	ld de, HelloWorldStr
.copyString
	ld a, [de]
	ld [hli], a
	inc de
	and a ; Check if the byte we just copied is zero
	jr nz, .copyString ; Continue if it's not

	; Init display registers
	ld a, %11100100
	ld [rBGP], a

	xor a ; ld a, 0
	ld [rSCY], a
	ld [rSCX], a

	; Shut sound down
	ld [rNR52], a

	; Turn screen on, display background
	ld a, %10000001
	ld [rLCDC], a

; .lockup
	nop
	halt
	; jr .lockup

SECTION "Font", ROM0

FontTiles:
INCBIN "font.chr"
FontTilesEnd:

SECTION "Hello World string", ROM0

HelloWorldStr:
	db "Hello World!", 0 ; 'db' places bytes of data (dw) for 16-bit words
