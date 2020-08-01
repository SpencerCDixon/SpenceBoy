SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "Game code", ROM0

Start:
	; trigger carry flag
	ld c, $ce
	rl c

	; use the carry flag in left shift of a
	ld a, $3b
	rla
	; assert(a == 0x77)

	nop
	halt
