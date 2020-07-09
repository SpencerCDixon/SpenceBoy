SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "TestCode", ROM0

Start:

	ld a, %10000000
	rl a

	ld b, %01000000
	rl b

	ld c, %10000001
	rl c

	nop
	halt

