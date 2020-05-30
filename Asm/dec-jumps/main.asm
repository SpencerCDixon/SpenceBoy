INCLUDE "../common/hardware.inc"

; The gameboy will load our code at address $100
SECTION "Header", ROM0[$100]

; There is a bunch of header junk starting at $104 so we need to immediately jump to our code
EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "Game code", ROM0

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

	halt 
