SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "TestCode", ROM0

Start:

	ld a, %10000001
	sra a

	ld b, %01001000
	sra b

	nop
	halt
