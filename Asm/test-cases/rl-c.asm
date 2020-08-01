SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "Game code", ROM0

Start:
	ld c, $ce
	rl c

	nop
	halt
