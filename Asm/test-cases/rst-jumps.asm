SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "TestCode", ROM0

Start:
	jp IncrementARegister

IncrementARegister:
	inc a
	cp a, 2
	jp z, End
	rst $00

End:
  nop
	halt
