SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

WRAM_END EQU $E000

SECTION "GameCode", ROM0

Start:
	ld b, $ff
	ld c, $dd

	; Set stack pointer to the end of RAM.
	; Stack grows downwards from end of RAM and will
	; automatically decrement by 1 before writing first value
	; so it's safe to be 1 over the end of RAM.
	ld sp, WRAM_END

	push bc
	pop de
	; assert(d == $ff)
	; assert(e == $ff)

	nop
	halt
