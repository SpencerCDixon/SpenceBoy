SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "TestCode", ROM0

Start:
  ld hl, $10
  ld bc, $10
  ld de, $10
  ld sp, $10

  add hl, bc
  add hl, de
  add hl, hl
  add hl, sp

  ; Carry flag should be set
  ld hl, $ffff
  add hl, bc

	nop
	halt

