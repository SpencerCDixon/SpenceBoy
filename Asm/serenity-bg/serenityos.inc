; ///////////////////////
; //                   //
; //  File Attributes  //
; //                   //
; ///////////////////////

; Filename: serenityos.png
; Pixel Width: 256px
; Pixel Height: 256px

; /////////////////
; //             //
; //  Constants  //
; //             //
; /////////////////

serenityos_tile_map_size EQU $0400
serenityos_tile_map_width EQU $20
serenityos_tile_map_height EQU $20

serenityos_tile_data_size EQU $0950
serenityos_tile_count EQU $95

; ////////////////
; //            //
; //  Map Data  //
; //            //
; ////////////////

serenityos_map_data:
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$01,$02,$03,$00,$00
DB $04,$05,$06,$07,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$08,$09,$0A,$0B,$0C
DB $0D,$0E,$0F,$10,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$11,$12,$13,$14
DB $00,$15,$16,$17,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$18,$19,$1A,$1B
DB $1C,$1D,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$1E,$1F,$20,$21,$00
DB $22,$23,$1D,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$24,$25,$26,$00,$27
DB $28,$29,$2A,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$2B,$00,$2C,$00,$2D
DB $2E,$2F,$30,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$31,$32,$33,$1D,$00
DB $20,$34,$35,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$36,$37,$38,$39,$3A
DB $3B,$3C,$3D,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$3E,$3F,$3C,$40,$41
DB $42,$43,$44,$45,$46,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$47,$48,$49,$4A
DB $4B,$4C,$4D,$3F,$4E,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$4F,$50,$51,$52,$3C
DB $3C,$53,$54,$55,$56,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$57,$58,$59,$3C,$3C
DB $3C,$3C,$5A,$5B,$3C,$5C,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$5D,$5E,$5F,$3C,$3C
DB $3C,$3C,$60,$61,$3C,$62,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$63,$64,$65,$3C,$3C
DB $3C,$3C,$66,$67,$3C,$68,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$69,$6A,$6B,$3C,$3C
DB $3C,$3C,$6C,$6D,$6E,$6F,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$70,$71,$72,$3C,$3C
DB $3C,$3C,$73,$74,$75,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$76,$77,$78,$79,$3C
DB $3C,$7A,$7B,$7C,$7D,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$7E,$7F,$80,$81
DB $3C,$82,$83,$84,$85,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$86,$87,$88
DB $3C,$89,$8A,$8B,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$8C,$3C,$3C,$8D
DB $3C,$8E,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$8F,$90,$91,$92
DB $93,$94,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00

; /////////////////
; //             //
; //  Tile Data  //
; //             //
; /////////////////

serenityos_tile_data:
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$01,$03,$01,$02,$03,$02,$02
DB $00,$00,$0F,$1F,$3D,$70,$60,$C0,$80,$80,$00,$00,$1C,$00,$3F,$1E
DB $00,$00,$80,$C0,$E0,$70,$38,$18,$04,$0C,$02,$06,$02,$03,$03,$01
DB $00,$00,$00,$00,$00,$00,$00,$00,$01,$00,$01,$03,$06,$06,$08,$0C
DB $00,$00,$03,$00,$0F,$1F,$38,$70,$E0,$C0,$80,$00,$00,$00,$00,$00
DB $00,$00,$20,$C0,$7C,$F8,$0E,$06,$01,$03,$00,$01,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$80,$80,$80,$C0,$C0,$40
DB $06,$02,$02,$06,$02,$06,$06,$02,$03,$02,$01,$03,$01,$01,$00,$00
DB $1F,$3F,$3F,$1F,$0F,$1F,$07,$0F,$05,$03,$00,$00,$00,$80,$80,$C0
DB $01,$81,$C1,$80,$C1,$C0,$C0,$C0,$C0,$E0,$60,$20,$30,$20,$30,$20
DB $00,$80,$80,$80,$80,$80,$80,$80,$C0,$80,$C0,$80,$C0,$80,$80,$80
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$01,$01,$01
DB $10,$18,$10,$30,$60,$20,$60,$40,$40,$C0,$C0,$80,$80,$80,$80,$00
DB $00,$00,$03,$07,$1F,$0F,$1F,$1F,$3F,$3F,$3F,$7F,$D6,$4F,$00,$C0
DB $C0,$00,$E0,$F0,$F8,$F0,$F0,$F0,$E0,$F0,$C0,$E0,$80,$00,$00,$00
DB $60,$40,$00,$60,$60,$20,$60,$20,$20,$20,$60,$20,$20,$60,$40,$60
DB $F0,$60,$18,$3F,$08,$07,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $60,$60,$C1,$C0,$80,$01,$01,$01,$01,$01,$03,$01,$00,$03,$4E,$3F
DB $80,$80,$80,$80,$00,$80,$80,$00,$00,$00,$00,$00,$00,$00,$80,$00
DB $03,$01,$00,$03,$03,$02,$03,$02,$02,$02,$02,$02,$03,$02,$03,$02
DB $C0,$80,$C0,$80,$C0,$80,$00,$C0,$60,$40,$20,$60,$17,$38,$17,$0F
DB $00,$00,$00,$00,$00,$01,$01,$03,$06,$06,$0C,$1C,$B8,$70,$A0,$C0
DB $C0,$40,$80,$C0,$80,$80,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $01,$00,$03,$03,$0E,$04,$1C,$08,$18,$10,$00,$30,$60,$20,$60,$40
DB $DB,$E0,$80,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $40,$E0,$18,$30,$08,$0C,$03,$07,$07,$02,$08,$04,$10,$08,$10,$10
DB $00,$03,$13,$0F,$61,$FF,$40,$80,$00,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$E0,$C0,$78,$30,$18,$0C,$04,$06,$02,$03,$01,$01
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$80
DB $00,$00,$00,$00,$01,$01,$03,$01,$03,$03,$03,$07,$06,$07,$07,$0E
DB $C0,$40,$C0,$80,$80,$80,$00,$80,$80,$00,$00,$00,$00,$00,$00,$00
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$01,$00,$00,$01
DB $10,$20,$40,$20,$40,$40,$80,$40,$40,$80,$80,$80,$80,$00,$00,$00
DB $01,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $C0,$C0,$F0,$E0,$F0,$78,$78,$7C,$3E,$7C,$7F,$3E,$3F,$3F,$3F,$3F
DB $0E,$0E,$1E,$0E,$0E,$1E,$1C,$1E,$3E,$1C,$1E,$3C,$1E,$3C,$3C,$3C
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$01,$00,$00,$01,$00,$01
DB $01,$01,$02,$01,$3B,$00,$C7,$02,$82,$82,$82,$82,$C6,$82,$D4,$EE
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$01,$10,$21,$20,$60,$20
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$80,$80,$80
DB $1F,$3F,$1F,$3F,$3F,$1F,$3F,$1F,$3F,$1F,$3F,$1F,$3F,$1F,$1F,$3F
DB $C0,$80,$C0,$C0,$E0,$C0,$C0,$E0,$E0,$E0,$F0,$E0,$E0,$F0,$F0,$F0
DB $3C,$3C,$7C,$3C,$7C,$3C,$3C,$7C,$3C,$7C,$7E,$7C,$7E,$7C,$7C,$7E
DB $FE,$FC,$FE,$7C,$52,$3C,$02,$04,$02,$04,$06,$00,$06,$00,$06,$00
DB $61,$60,$61,$71,$7F,$3F,$3F,$3F,$0E,$1F,$00,$00,$00,$00,$00,$00
DB $C0,$80,$C0,$80,$80,$80,$00,$80,$00,$00,$00,$00,$00,$00,$00,$00
DB $1F,$3F,$1F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$7F,$3F,$7F,$3F
DB $F8,$F0,$F0,$F8,$F0,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$FC,$F8,$FC,$F8
DB $7E,$7E,$7E,$7E,$7F,$7E,$7E,$7F,$7F,$7F,$7F,$7F,$3F,$7F,$3F,$7F
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$80,$00,$80,$80,$80,$C0
DB $04,$02,$02,$02,$02,$02,$02,$02,$03,$00,$02,$01,$01,$01,$01,$00
DB $3F,$7F,$7F,$7F,$7F,$7F,$FF,$7F,$7F,$FF,$FF,$FF,$FF,$FF,$FF,$FF
DB $FC,$F8,$FC,$F8,$FC,$F8,$FC,$F8,$FC,$F8,$F8,$F8,$F8,$F8,$F8,$F8
DB $7F,$3F,$7F,$3F,$3F,$3F,$3F,$3F,$1F,$3F,$3F,$1F,$1F,$1F,$0F,$1F
DB $C0,$E0,$E0,$F0,$F0,$F8,$FE,$FC,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
DB $00,$00,$00,$00,$00,$00,$00,$00,$61,$80,$FF,$FF,$FF,$FF,$FF,$FF
DB $80,$80,$80,$40,$20,$40,$30,$20,$F0,$78,$FE,$FC,$FF,$FF,$FF,$FF
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$80,$00,$FF,$FF
DB $01,$01,$01,$03,$07,$03,$07,$07,$07,$0F,$0F,$1F,$7F,$3F,$FF,$FF
DB $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
DB $F8,$F8,$F0,$F8,$F0,$F8,$F8,$F0,$F0,$F0,$F0,$F0,$E0,$F0,$FA,$FC
DB $0F,$0F,$07,$0F,$07,$07,$03,$07,$03,$03,$01,$01,$01,$00,$00,$00
DB $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$7F
DB $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FE,$FF,$FF,$F8
DB $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$33,$CF,$FF,$00
DB $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$7F,$FF
DB $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FE,$FF
DB $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$BF,$7F
DB $00,$80,$C0,$E0,$F8,$F0,$F8,$FC,$FE,$FE,$FF,$FF,$FF,$FF,$FF,$FF
DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$80,$80,$C0,$C0
DB $7F,$3F,$3F,$1F,$3F,$1F,$3F,$3F,$7F,$3F,$3E,$7F,$7F,$7E,$FD,$7E
DB $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$7F,$DF,$3F,$FF,$07
DB $EF,$F0,$FF,$C0,$DF,$E0,$FB,$FC,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
DB $FF,$00,$FF,$00,$FF,$00,$FD,$03,$8F,$FF,$FF,$FF,$FF,$FF,$FF,$FF
DB $FF,$3F,$BF,$7F,$7F,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
DB $FF,$FE,$FD,$FE,$FB,$FC,$FF,$F8,$EF,$F0,$DF,$E0,$DF,$E0,$EF,$F0
DB $FF,$1F,$F7,$0F,$FB,$07,$FF,$03,$FF,$01,$FE,$01,$FF,$00,$FF,$00
DB $C0,$E0,$E0,$E0,$F0,$F0,$F8,$F0,$F8,$F8,$FC,$F8,$F8,$FC,$FC,$FC
DB $00,$00,$00,$00,$01,$00,$01,$01,$01,$01,$03,$01,$01,$03,$03,$03
DB $FF,$FC,$FB,$FC,$FF,$F8,$F7,$F8,$FF,$F0,$FF,$F0,$EF,$F0,$FE,$E1
DB $FE,$01,$FF,$00,$FD,$03,$FB,$07,$FF,$0F,$DF,$3F,$FF,$7F,$FF,$FF
DB $7F,$FF,$7F,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
DB $F7,$F8,$FB,$FC,$FF,$FC,$FD,$FE,$FF,$FE,$FF,$FF,$FF,$FF,$FF,$FF
DB $FF,$00,$FF,$00,$FF,$00,$FF,$00,$FF,$00,$7F,$80,$FF,$C0,$EF,$F0
DB $FF,$7F,$FF,$3F,$FF,$3F,$DF,$3F,$FF,$1F,$EF,$1F,$FF,$0F,$FF,$0F
DB $FE,$FC,$FC,$FE,$FE,$FE,$FE,$FE,$FE,$FE,$FF,$FE,$FE,$FF,$FE,$FF
DB $03,$03,$07,$03,$03,$07,$07,$07,$07,$07,$0F,$07,$0F,$07,$07,$0F
DB $FD,$E3,$DF,$E7,$FF,$CF,$FF,$DF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
DB $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$E7,$FF,$FF,$CF,$BF,$CF,$FF,$8F
DB $F7,$F8,$FF,$FC,$FE,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$9F,$FF,$AF,$DF
DB $F7,$0F,$FF,$07,$FF,$07,$7B,$87,$BB,$C7,$DF,$E3,$EF,$F3,$FF,$F3
DB $00,$00,$00,$00,$00,$00,$00,$00,$80,$00,$80,$00,$80,$00,$00,$80
DB $0F,$0F,$0F,$0F,$0F,$0F,$1F,$0F,$1F,$0F,$0F,$1F,$0F,$1F,$1F,$1F
DB $FF,$FF,$FF,$FF,$FF,$BF,$FF,$9F,$77,$8F,$7D,$83,$7F,$80,$7F,$80
DB $CF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$BF,$7F,$BF,$7F
DB $FF,$CF,$DF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FE,$FF,$FB,$FC
DB $FD,$F3,$FD,$F3,$FD,$F3,$EF,$F1,$DF,$E1,$BF,$C1,$FF,$01,$FF,$01
DB $00,$80,$00,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80
DB $1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F
DB $7F,$80,$7F,$80,$7F,$80,$7F,$80,$7D,$83,$7B,$87,$F7,$8F,$FF,$8F
DB $FF,$7F,$7F,$FF,$7F,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
DB $FB,$FC,$FD,$FE,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$EF,$FF
DB $FF,$01,$FF,$01,$FF,$01,$FF,$81,$DF,$E1,$FF,$E1,$EF,$F1,$FF,$F1
DB $80,$80,$80,$80,$80,$80,$00,$80,$00,$80,$00,$80,$00,$80,$80,$00
DB $1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$0F,$1F
DB $EF,$9F,$FF,$9F,$FF,$9F,$FF,$9F,$AF,$DF,$B7,$CF,$FF,$C3,$FE,$C1
DB $FF,$FF,$EF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
DB $EF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FD,$FE,$F7,$F8
DB $FF,$F1,$FF,$F1,$FD,$F3,$ED,$F3,$DF,$E3,$FF,$83,$FF,$03,$FF,$03
DB $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FE,$FF,$FF,$FE
DB $80,$00,$80,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $0F,$1F,$1F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$07,$0F,$07,$07,$07,$07
DB $FF,$C0,$DF,$E0,$DF,$E0,$FF,$E0,$FF,$E0,$EF,$F0,$FF,$F0,$FF,$F0
DB $9F,$7F,$FF,$0F,$EF,$1F,$FF,$1F,$DF,$3F,$DF,$3F,$FF,$1F,$EF,$1F
DB $EF,$F0,$FF,$F0,$F7,$F8,$F7,$F8,$F7,$F8,$F7,$F8,$EF,$F0,$FF,$E0
DB $FB,$07,$FF,$07,$FF,$07,$FF,$07,$F7,$0F,$FF,$0F,$FF,$0F,$EF,$1F
DB $FF,$FE,$FE,$FE,$FE,$FE,$FC,$FE,$FE,$FC,$FC,$FC,$FC,$FC,$F8,$FC
DB $03,$07,$03,$03,$01,$03,$01,$01,$00,$01,$00,$00,$00,$00,$00,$00
DB $FF,$F8,$FF,$F8,$FB,$FC,$FF,$FC,$FF,$FE,$FE,$FF,$7F,$FF,$7F,$7F
DB $FF,$0F,$FB,$07,$FF,$00,$FF,$00,$FF,$00,$FF,$00,$FF,$00,$FF,$80
DB $FF,$FF,$FF,$FF,$7F,$FF,$BF,$7F,$DF,$3F,$FF,$0F,$FF,$0F,$FF,$0F
DB $FA,$FD,$F7,$F8,$EF,$F0,$DF,$E0,$BF,$C0,$BF,$C0,$BF,$C0,$BF,$C0
DB $3F,$C0,$FF,$00,$FF,$00,$FF,$00,$FF,$00,$FF,$00,$FF,$00,$FE,$01
DB $FF,$1F,$FF,$1F,$DF,$3F,$FF,$3F,$BF,$7F,$FF,$7F,$7F,$FF,$FF,$FF
DB $FC,$F8,$F8,$F8,$F8,$F8,$F0,$F8,$F0,$F0,$F0,$F0,$F0,$E0,$E0,$E0
DB $3F,$3F,$3F,$1F,$1F,$0F,$0F,$07,$07,$03,$03,$01,$00,$00,$00,$00
DB $FF,$C0,$FF,$E0,$F7,$F8,$FE,$FF,$FF,$FF,$FF,$FF,$7F,$FF,$7F,$3F
DB $EF,$1F,$EF,$1F,$EF,$1F,$7F,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
DB $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$F8,$FD,$F8,$F8,$F9,$F8
DB $BF,$C0,$BF,$C0,$BF,$C0,$DF,$E0,$F9,$FE,$FF,$FF,$FF,$FF,$FF,$FF
DB $FF,$01,$FF,$03,$FF,$07,$EF,$1F,$3F,$FF,$FF,$FF,$FF,$FF,$FF,$FF
DB $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FE,$FE,$FE
DB $C0,$E0,$C0,$C0,$C0,$C0,$C0,$80,$00,$80,$00,$00,$00,$00,$00,$00
DB $0F,$1F,$05,$03,$00,$00,$00,$00,$00,$00,$00,$00,$06,$00,$3F,$7F
DB $FF,$FF,$FF,$FF,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$BF,$DF
DB $F9,$F8,$F9,$F8,$F9,$F8,$F9,$F8,$F1,$F8,$F1,$F8,$F1,$F8,$F1,$F8
DB $FF,$FF,$FF,$FF,$FF,$FF,$9F,$FF,$CB,$87,$C0,$80,$C0,$80,$C0,$80
DB $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$82,$7C,$00,$00,$00,$00
DB $FC,$FC,$F8,$F8,$F0,$F0,$C0,$E0,$C0,$80,$00,$00,$00,$00,$00,$00
DB $01,$00,$01,$03,$03,$03,$03,$07,$07,$07,$07,$07,$07,$07,$03,$07
DB $F9,$F8,$FD,$F8,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
DB $C0,$80,$C0,$80,$C0,$80,$C0,$80,$C0,$80,$C0,$80,$C0,$80,$C0,$80
DB $07,$03,$01,$03,$01,$01,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
DB $FF,$FF,$FF,$FF,$FF,$FF,$7F,$FF,$30,$0F,$00,$00,$00,$00,$00,$00
DB $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$3F,$DF,$07,$0F,$01,$03,$00,$00
DB $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$00,$00
DB $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$C6,$F8,$00,$00
DB $C0,$80,$C0,$80,$C0,$80,$80,$80,$80,$80,$00,$80,$00,$00,$00,$00
