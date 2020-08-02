SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "TestCode", ROM0

Start:
	xor a
	ld a, $ff
	dec a

	; ld b, 0
	; dec b
	
	; ld c, 1
	; dec b

	nop
	halt 
