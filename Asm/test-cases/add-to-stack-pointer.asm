SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "TestCode", ROM0

Start:
	; should not trigger any carries, resets other flags though
	ld sp, $ffa0
	add sp, 1

	nop
	halt

