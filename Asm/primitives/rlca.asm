; The gameboy will load our code at address $100
SECTION "Header", ROM0[$100]

; There is a bunch of header junk starting at $104 so we need to immediately jump to our code
EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "GameCode", ROM0

Start:
	ld a, $40 ; 0100_0000
	rlca ; AF: $8000 -> no carry but shifted one

	ld a, $80 ; 1000_0000

	; AF: $0110 -> the 1 gets shifted left to the beginning and carry is set
	rlca ; a == 0000_0001, carry should contain 1

	ld a, $80 ; 1000_0000

	; AF: $0110 -> the 1 gets shifted left to the beginning and carry is set
	rla ; a == 0000_0000, carry should contain 1

	nop
	halt
