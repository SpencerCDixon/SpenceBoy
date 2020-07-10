SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "TestCode", ROM0

Start:

	; Test code starts here
	ld a, 1
	res 0, a

	ld b, 3
	res 0, b

	ld c, 0
	res 0, c

	nop
	halt

