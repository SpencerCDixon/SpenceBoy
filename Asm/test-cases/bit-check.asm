SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "TestCode", ROM0

Start:
	ld a, 0
	ld h, %10000000
	bit 7, h

	nop
	halt


