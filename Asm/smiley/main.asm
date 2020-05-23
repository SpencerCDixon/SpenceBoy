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

; Smiley based off: https://medium.com/@ulrikdamm/writing-your-first-gameboy-game-4ea62c76db29

Start:
	ld hl, $9010
	ld de, smiley_sprite
	ld b, 16

; Copy sprite data from ROM into VRAM
copy_loop:
	ld a, [de]
	inc de
	ld [hl+], a
	dec b
	jp nz, copy_loop

    ; Set the sprite index that we're using to 1
	ld hl, $9800
	ld [hl], 1

    ; Set the background palette
	ld hl, $ff47
	ld [hl], $e4 ; 1110_0100

    ;
    ; Bit 0: Background and window display on/off
    ; Bit 1: Sprite layer on/off
    ; Bit 2: Sprite size (0: 8x8, 1: 8x16)
    ; Bit 3: Background tile map select (0: $9800-$9bff, 1: $9c00-$9fff)
    ; Bit 4: Background and window tile data select (0: $8800-$97ff, 1: $8000-$8fff)
    ; Bit 5: Window display on/off
    ; Bit 6: Window tile map select (0: $9800-$9bff, 1: $9c00-$9fff)
    ; Bit 7: Display on/off
	ld hl, $ff40
	ld [hl], $81

; Stop
	nop
	db $db

smiley_sprite: 
	db $00, $00, $00, $00, $24, $24, $00, $00, $81, $81, $7e, $7e, $00, $00, $00, $00
