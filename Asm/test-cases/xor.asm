SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "TestCode", ROM0

Start:
	; Assert carry in shift works properly
	ld a, 1;
	xor a

	ld a, %00001010;
	; assert(a == 0)

	ld b, %00001111
	xor b 

	ld c, %11110000
	xor c 

	ld d, %00001111
	xor d 

	ld e, %00001111
	xor e 

	ld h, %00001111
	xor h 

	ld l, %00001111
	xor l 

	ld a, %11001100
	xor %00110011

	nop
	halt
