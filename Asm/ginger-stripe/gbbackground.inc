; ///////////////////////
; //                   //
; //  File Attributes  //
; //                   //
; ///////////////////////

; Filename: gb-background.png
; Pixel Width: 256px
; Pixel Height: 256px

; /////////////////
; //             //
; //  Constants  //
; //             //
; /////////////////

gbbackground_tile_map_size EQU $0400
gbbackground_tile_map_width EQU $20
gbbackground_tile_map_height EQU $20

gbbackground_tile_data_size EQU $0240
gbbackground_tile_count EQU $24

; ////////////////
; //            //
; //  Map Data  //
; //            //
; ////////////////

gbbackground_map_data:
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$01,$02,$02,$02,$02,$02,$02,$02,$02,$03,$00,$00,$00,$00
DB $00,$00,$00,$04,$02,$02,$02,$02,$02,$02,$02,$02,$05,$00,$00,$00
DB $00,$00,$06,$07,$07,$07,$07,$07,$07,$07,$07,$08,$00,$00,$00,$00
DB $00,$00,$00,$09,$07,$07,$07,$07,$07,$07,$07,$07,$0A,$00,$00,$00
DB $00,$00,$06,$07,$07,$07,$07,$07,$07,$07,$07,$08,$00,$00,$00,$00
DB $00,$00,$00,$09,$07,$07,$07,$07,$07,$07,$07,$07,$0A,$00,$00,$00
DB $00,$00,$06,$07,$07,$07,$07,$07,$07,$07,$07,$08,$00,$00,$00,$00
DB $00,$00,$00,$09,$07,$07,$07,$07,$07,$07,$07,$07,$0A,$00,$00,$00
DB $00,$00,$06,$07,$07,$07,$07,$07,$07,$07,$07,$08,$00,$00,$00,$00
DB $00,$00,$00,$09,$07,$07,$07,$07,$07,$07,$07,$07,$0A,$00,$00,$00
DB $00,$00,$06,$07,$07,$07,$07,$07,$07,$07,$07,$08,$00,$00,$00,$00
DB $00,$00,$00,$09,$07,$07,$07,$07,$07,$07,$07,$07,$0A,$00,$00,$00
DB $00,$00,$06,$07,$07,$07,$07,$07,$07,$07,$07,$08,$00,$00,$00,$00
DB $00,$00,$00,$09,$07,$07,$07,$07,$07,$07,$07,$07,$0A,$00,$00,$00
DB $00,$00,$0B,$0C,$0C,$0C,$0C,$0C,$0C,$0C,$0C,$0D,$00,$00,$00,$00
DB $00,$00,$00,$0E,$0C,$0C,$0C,$0C,$0C,$0C,$0C,$0C,$0F,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$10,$11,$11,$11,$11,$11,$11,$11,$11,$12,$00
DB $00,$13,$11,$11,$11,$11,$11,$11,$11,$11,$14,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$15,$07,$07,$07,$07,$07,$07,$07,$07,$16,$00
DB $00,$17,$07,$07,$07,$07,$07,$07,$07,$07,$18,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$15,$07,$07,$07,$07,$07,$07,$07,$07,$16,$00
DB $00,$17,$07,$07,$07,$07,$07,$07,$07,$07,$18,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$15,$07,$07,$07,$07,$07,$07,$07,$07,$16,$00
DB $00,$17,$07,$07,$07,$07,$07,$07,$07,$07,$18,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$15,$07,$07,$07,$07,$07,$07,$07,$07,$16,$00
DB $00,$17,$07,$07,$07,$07,$07,$07,$07,$07,$18,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$15,$07,$07,$07,$07,$07,$07,$07,$07,$16,$00
DB $00,$17,$07,$07,$07,$07,$07,$07,$07,$07,$18,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$15,$07,$07,$07,$07,$07,$07,$07,$07,$16,$00
DB $00,$17,$07,$07,$07,$07,$07,$07,$07,$07,$18,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$19,$1A,$1A,$1A,$1A,$1A,$1A,$1A,$1A,$1B,$00
DB $00,$1C,$1A,$1A,$1A,$1A,$1A,$1A,$1A,$1A,$1D,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$1E,$1F,$1F,$1F,$1F
DB $1F,$1F,$1F,$1F,$20,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$15,$07,$07,$07,$07
DB $07,$07,$07,$07,$16,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$15,$07,$07,$07,$07
DB $07,$07,$07,$07,$16,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$15,$07,$07,$07,$07
DB $07,$07,$07,$07,$16,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$15,$07,$07,$07,$07
DB $07,$07,$07,$07,$16,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$15,$07,$07,$07,$07
DB $07,$07,$07,$07,$16,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$15,$07,$07,$07,$07
DB $07,$07,$07,$07,$16,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$21,$22,$22,$22,$22
DB $22,$22,$22,$22,$23,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00

; /////////////////
; //             //
; //  Tile Data  //
; //             //
; /////////////////

gbbackground_tile_data:
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$1F,$1F,$1F,$1F
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$FF,$FF,$FF,$FF
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$C0,$C0,$C0,$C0
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$03,$03,$03,$03
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$F8,$F8,$F8,$F8
DB $1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F
DB $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
DB $C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0
DB $03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$03
DB $F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8
DB $1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$00,$00,$00,$00,$00,$00
DB $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$00,$00,$00,$00,$00,$00
DB $C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$00,$00,$00,$00,$00,$00
DB $03,$03,$03,$03,$03,$03,$03,$03,$03,$03,$00,$00,$00,$00,$00,$00
DB $F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$3F,$3F,$3F,$3F,$3F,$3F
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$FF,$FF,$FF,$FF,$FF,$FF
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$80,$80,$80,$80,$80,$80
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$07,$07,$07,$07,$07,$07
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$F0,$F0,$F0,$F0,$F0,$F0
DB $3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F
DB $80,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80
DB $07,$07,$07,$07,$07,$07,$07,$07,$07,$07,$07,$07,$07,$07,$07,$07
DB $F0,$F0,$F0,$F0,$F0,$F0,$F0,$F0,$F0,$F0,$F0,$F0,$F0,$F0,$F0,$F0
DB $3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$00,$00,$00,$00,$00,$00,$00,$00
DB $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$00,$00,$00,$00,$00,$00,$00,$00
DB $80,$80,$80,$80,$80,$80,$80,$80,$00,$00,$00,$00,$00,$00,$00,$00
DB $07,$07,$07,$07,$07,$07,$07,$07,$00,$00,$00,$00,$00,$00,$00,$00
DB $F0,$F0,$F0,$F0,$F0,$F0,$F0,$F0,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F
DB $00,$00,$00,$00,$00,$00,$00,$00,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
DB $00,$00,$00,$00,$00,$00,$00,$00,$80,$80,$80,$80,$80,$80,$80,$80
DB $3F,$3F,$3F,$3F,$3F,$3F,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $FF,$FF,$FF,$FF,$FF,$FF,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $80,$80,$80,$80,$80,$80,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
