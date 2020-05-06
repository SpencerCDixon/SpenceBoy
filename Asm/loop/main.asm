INCLUDE "hardware.inc"

SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start ; 

REPT $150 - $104
	db 0
ENDR

SECTION "Game code", ROM0

Start:
	ld a, $03
	dec a
	jr nz, Start 
	halt
