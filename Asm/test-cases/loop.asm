SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "Game code", ROM0

Start:
	ld a, 3 ; load a register with 3

loop:
	dec a
	jp nz, loop

	nop
	halt 
