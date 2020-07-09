SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "TestCode", ROM0

mSet:
    inc b
    inc c
    jr  .skip
.loop:
    ld [hl+], a 
.skip:
    dec c
    jr  nz, .loop
    dec b
    jr nz, .loop
    ret

Start:
	ld a, 3
	
	; Set 4096 bytes to '03'. This can take a while!
	ld hl, $c000
	ld b, $10 
	ld c, $00
	call mSet

	nop
	halt 
