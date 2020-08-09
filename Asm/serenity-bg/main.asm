INCLUDE "../common/hardware.inc"

; The gameboy will load our code at address $100
SECTION "Header", ROM0[$100]

; There is a bunch of header junk starting at $104 so we need to immediately jump to our code
EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

BACKGROUND_MAPDATA_START    EQU $9800 ; up to $9BFF
TILEDATA_START              EQU $8000 ; up to $97FF

; Key status
KEY_START   EQU %10000000
KEY_SELECT  EQU %01000000
KEY_B       EQU %00100000
KEY_A       EQU %00010000
KEY_DOWN    EQU %00001000
KEY_UP      EQU %00000100
KEY_LEFT    EQU %00000010
KEY_RIGHT   EQU %00000001

SECTION "Game code", ROM0

INCLUDE "./serenityos.inc"

; Copies data in a way that is NOT safe to use when reading/writing to/from VRAM while LCD is on (but faster than mCopyVRAM)
; HL - memory position of the start of the copying source
; DE - memory position of the start of the copying destination
; BC - the number of bytes to be copied
mCopy:
    inc b
    inc c
    jr  .skip
.loop:
    ld a, [hl+]
    ld [de], a
    inc de
.skip:
    dec c
    jr  nz, .loop
    dec b
    jr nz, .loop
    ret

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
	; Set LCDC 
	ld hl, $ff40
	ld [hl], $94 ; 1001_0100

	; Load our tiles into the tile map
	ld hl, serenityos_tile_data
	ld de, TILEDATA_START
	ld bc, serenityos_tile_data_size
	call mCopy

	; Load our indexes
	ld hl, serenityos_map_data
	ld de, BACKGROUND_MAPDATA_START
	ld bc, serenityos_tile_map_size
	call mCopy

	; scy up a bit
	ld hl, $ff42
	ld [hl], $20

	ld hl, $ff43

.loop
	call ReadKeys
	and KEY_LEFT
	jp z, .loop

	ld a, [hl]
	add 1
	ld [hl], a
	jp .loop

	; TODO: Take palette into consideration in the PPU
	; Set the background palette
	; ld hl, $ff47
	; ld [hl], $e4 ; 1110_0100

	; TODO: Take all of these into consideration in the PPU
	; Bit 0: Background and window display on/off
	; Bit 1: Sprite layer on/off
	; Bit 2: Sprite size (0: 8x8, 1: 8x16)
	; Bit 3: Background tile map select (0: $9800-$9bff, 1: $9c00-$9fff)
	; Bit 4: Background and window tile data select (0: $8800-$97ff, 1: $8000-$8fff)
	; Bit 5: Window display on/off
	; Bit 6: Window tile map select (0: $9800-$9bff, 1: $9c00-$9fff)
	; Bit 7: Display on/off
	; ld hl, $ff40
	; ld [hl], $81

; Stop
	nop
	halt
