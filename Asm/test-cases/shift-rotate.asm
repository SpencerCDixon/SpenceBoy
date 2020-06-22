SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "GameCode", ROM0

Start:
	; Assert carry in shift works properly
	ld a, %11000011;
	sla a
	; assert(b == 10000110)
	; assert(flag.carry)

	ld b, %10000000;
	sla b
	; assert(b == 0)
	; assert(flag.zero)

	ld c, %00001111
	sla c

	ld d, %00000001
	sla d

	ld e, %00000001
	sla e

	ld h, %00000001
	sla h

	ld l, %00000001
	sla l

	nop
	halt
