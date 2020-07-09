SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "TestCode", ROM0

Start:
	; Save 52 to some place in RAM
	ld b, 52
	ld h, $c0
	ld l, $02
	ld [hl], b

	nop
	halt
