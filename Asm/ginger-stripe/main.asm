; This section is for including files that either need to be in the home section, or files where it doesn't matter 
SECTION "Includes@home",ROM0

; Prior to importing GingerBread, some options can be specified

; Max 15 characters, should be uppercase ASCII
GAME_NAME EQUS "STRIPES"

; Include SGB support in GingerBread. This makes the GingerBread library take up a bit more space on ROM0. To remove support, comment out this line (don't set it to 0)
;SGB_SUPPORT EQU 1 

; Include GBC support in GingerBread. This makes the GingerBread library take up slightly more space on ROM0. To remove support, comment out this line (don't set it to 0)
;GBC_SUPPORT EQU 1

; Set the size of the ROM file here. 0 means 32 kB, 1 means 64 kB, 2 means 128 kB and so on.
ROM_SIZE EQU 0 

; Set the size of save RAM inside the cartridge. 
; If printed to real carts, it needs to be small enough to fit. 
; 0 means no RAM, 1 means 2 kB, 2 -> 8 kB, 3 -> 32 kB, 4 -> 128 kB 
RAM_SIZE EQU 1

INCLUDE "../common/gingerbread.asm"
; INCLUDE "gbbackground.inc"
INCLUDE "stripe.inc"

SECTION "StartOfGameCode",ROM0    
begin: 
	ld hl, $ff40
	ld [hl], $81

	ld hl, stripebackground_tile_data
	ld de, TILEDATA_START
	ld bc, stripebackground_tile_data_size
	call mCopyVRAM

	ld hl, stripebackground_map_data
	ld de, BACKGROUND_MAPDATA_START
	ld bc, stripebackground_tile_map_size
	call mCopyVRAM

	; move x to the far right
	ld a, $00
	ld [SCROLL_X], a 

	; move y to the bottom
	ld a, $00
	ld [SCROLL_Y], a 

	call StartLCD
	nop
	halt
