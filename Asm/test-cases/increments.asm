SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "TestCode", ROM0

Start:
	; Reset register values 
	ld a, 0
	ld b, 0
	ld c, 0
	ld d, 0
	ld e, 0
	ld h, 0
	ld l, 0
	ld sp, 0

	; Multi-register incs don't change flags at all
	inc bc
	inc de
	inc hl
	inc sp

	; TODO: explicit tests that half carry and zero flags are getting set
	; and that subtract flag gets reset.
	inc a
	inc b
	inc c
	inc d
	inc e
	inc h
	inc l

	nop
	halt
