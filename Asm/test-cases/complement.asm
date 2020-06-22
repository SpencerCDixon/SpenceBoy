SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "GameCode", ROM0

Start:
	ld a, %11110000;
	cpl
	; assert(a == 00001111) (15)
	nop
	halt
