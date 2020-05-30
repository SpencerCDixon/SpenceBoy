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

copyData:
.loop
	ld a, [de]
	inc de
	ld [hl+], a
	dec b
	jp nz, .loop
	ret

; Rows are 32 tiles wide
; de - the sprite to render
; TODO: This is not working as expected in SameBoy -- something is wrong :-(
writeRow:
	ld c, 32
.loop
	ld b, 16
	call copyData
	ld a, e
	sub 16
	ld e, a
	dec c
	jp nz, .loop
	ret

Start:
	ld hl, $9000
	ld de, black_stripe
	call writeRow

	ld hl, $9200
	ld de, white_stripe
	call writeRow

	; ld hl, $9400
	; ld de, black_stripe
	; call writeRow

	; ld hl, $9600
	; ld de, white_stripe
	; call writeRow

	; ld hl, $9010 
	; ld de, black_stripe
	; ld b, 16
	; call copyData

	; ld hl, $9020
	; ld de, checker_stripe
	; ld b, 16
	; call copyData

	; ld hl, $9030
	; ld de, top_half_black
	; ld b, 16
	; call copyData

	; Set the sprite index that we're using to 1
	ld hl, $9800
	ld [hl], 0

    ; Set the background palette
	ld hl, $ff47
	ld [hl], $e4 ; 1110_0100

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
	halt

SECTION "BGTileData",ROM0[$1000]

; checker_stripe: 
	; db $cc, $cc, $cc, $cc, $cc, $cc, $cc, $cc, $cc, $cc, $cc, $cc, $cc, $cc, $cc, $cc

black_stripe: 
	db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff

white_stripe: 
	db $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00

; top_half_black: 
	; db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff, $00, $00, $00, $00, $00, $00, $00, $00

; bot_half_black: 
	; db $00, $00, $00, $00, $00, $00, $00, $00, $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff
