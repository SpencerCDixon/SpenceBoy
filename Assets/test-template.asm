SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "TestCode", ROM0

Start:

	; Test code starts here

	nop
	halt

