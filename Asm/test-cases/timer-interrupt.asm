; Constants
IEF_HILO    EQU  %00010000 ; Transition from High to Low of Pin number P10-P13
IEF_SERIAL  EQU  %00001000 ; Serial I/O transfer end
IEF_TIMER   EQU  %00000100 ; Timer Overflow
IEF_LCDC    EQU  %00000010 ; LCDC
IEF_VBLANK  EQU  %00000001 ; V-Blank

; Interrupt Registers
rIF EQU $FF0F
rIE EQU $FFFF

; Interrupt Handlers
SECTION	"vblank interrupt",ROM0[$0040]
  reti
SECTION	"LCDC interrupt",ROM0[$0048]
  reti
SECTION	"Timer overflow interrupt",ROM0[$0050]
  jp DecrementCounter
SECTION	"Serial interrupt",ROM0[$0058]
  reti
SECTION	"p1234 interrupt",ROM0[$0060]
  reti

SECTION "Header", ROM0[$100]

EntryPoint:
	jp Start; 

REPT $150 - $104
	db 0
ENDR

SECTION "TestCode", ROM0

Start:
	; Turn LCD display on
  ld hl, $ff40
  ld [hl], $81

  ; Turn on Timer
  ld hl, $ff07
  ld [hl], $4

  nop
  di   ; Disable interrupts

  ; load 2 into b. We'll decrement b during timer interrupts
  ld b, $2 

  ld sp, $FFFF ; Initialize stack pointer

  ; We only want to handle interrupts for timer
  ld a, IEF_TIMER
  ld [rIE], a
	xor a

  ei

TimerWaitLoop:
  cp b
  jp nz, TimerWaitLoop
	jp End

; Example of a way we can jump to handlers when interrupt is enabled
DecrementCounter:
  dec b
  reti

End:
	nop
	halt

