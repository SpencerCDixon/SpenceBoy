SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "Game code", ROM0

Start:
	ld a, 0
	ld b, 0
	ld c, 0
	LD DE,$0104		; $0021  Convert and load logo data from cart into Video RAM
	LD HL,$8010		; $0024

  LD A,[DE]		; $0027
  LD C,A
  LD B,$04

.loop
  PUSH BC
  RL C			
  RLA			; 
  POP BC		
  RL C			
  RLA			; 
  DEC B			
  JR NZ, .loop	
  LD [HL+],A		
  INC HL		
  LD [HL+],A	
  INC HL	

	; jp Start
	nop
	halt
