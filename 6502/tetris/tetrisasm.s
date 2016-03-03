    .fopt        compiler,"cc65 v 2.13.2"
    .setcpu        "6502"
    .smart        on
    .autoimport    on
    .case        on
    .debuginfo    off
    .importzp    sp, sreg, regsave, regbank, tmp1, ptr1, ptr2
    .macpack    longbranch
    .forceimport    __STARTUP__
    .export        _nmi
    .export        _main

.include        "tetrischr.inc"

.segment    "RODATA"
_block:
    .byte $07,$00,$03,$00,$03,$00,$06,$00,$25,$42,$00,$00,$00,$00,$00,$00 ;0
    .byte $07,$00,$03,$00,$03,$00,$06,$00,$25,$42,$00,$00,$00,$00,$00,$00 ;1 1
    .byte $9B,$00,$A8,$00,$00,$00,$00,$00,$9B,$00,$A8,$00,$00,$00,$00,$00 ;2
    .byte $9B,$00,$A8,$00,$00,$00,$00,$00,$9B,$00,$A8,$00,$00,$00,$00,$00 ;3 2
    .byte $70,$00,$AB,$00,$06,$00,$00,$00,$95,$00,$80,$04,$00,$00,$00,$00 ;4
    .byte $70,$00,$AB,$00,$06,$00,$00,$00,$95,$00,$80,$04,$00,$00,$00,$00 ;5 3
    .byte $07,$00,$98,$00,$60,$00,$00,$00,$B0,$04,$A5,$00,$00,$00,$00,$00 ;6
    .byte $07,$00,$98,$00,$60,$00,$00,$00,$B0,$04,$A5,$00,$00,$00,$00,$00 ;7 4
    .byte $07,$00,$03,$00,$48,$00,$00,$00,$00,$07,$25,$0A,$00,$00,$00,$00 ;8
    .byte $95,$00,$30,$00,$60,$00,$00,$00,$2B,$04,$06,$00,$00,$00,$00,$00 ;9 5
    .byte $4B,$00,$03,$00,$06,$00,$00,$00,$07,$00,$28,$04,$00,$00,$00,$00 ;A
    .byte $70,$00,$30,$00,$A5,$00,$00,$00,$25,$09,$00,$06,$00,$00,$00,$00 ;B 6
    .byte $07,$00,$4E,$00,$06,$00,$00,$00,$70,$00,$F5,$04,$00,$00,$00,$00 ;C
    .byte $70,$00,$C5,$00,$60,$00,$00,$00,$D5,$04,$60,$00,$00,$00,$00,$00 ;D 7
_chg:
    .byte $00,$11,$45,$33,$41,$15,$66,$77,$68,$97,$A6,$7B,$C3,$9B,$3E,$A8 ;E chg
_bottom:
    .byte $11,$11,$04,$00,$11,$11,$04,$00,$22,$00,$22,$00,$22,$00,$22,$00 ;0
    .byte $22,$01,$32,$00,$22,$01,$32,$00,$21,$02,$23,$00,$21,$02,$23,$00 ;1
    .byte $11,$02,$33,$00,$22,$02,$13,$00,$12,$01,$31,$00,$22,$02,$33,$00 ;2
    .byte $21,$01,$32,$00,$22,$02,$23,$00,$00,$00,$00,$00,$00,$00,$00,$00 ;3
_left:
    .byte $04,$01,$04,$01,$02,$02,$02,$02,$03,$02,$03,$02,$03,$02,$03,$02 ;4
    .byte $03,$02,$03,$02,$03,$02,$03,$02,$03,$02,$03,$02,$00,$00,$00,$00 ;5
_dataUser:
    .byte $02,$02,$10,$10,$02,$0D,$10,$68,$07,$0D,$00,$00,$00,$00,$00,$00 ;6 player1
    .byte $02,$13,$10,$98,$0B,$0E,$58,$70,$10,$0E,$00,$00,$00,$00,$00,$00 ;7 player2
    
.define DRAW_BOARD    $00
.define DRAW_SP       $02
.define DRAW_NEXT     $04
.define DRAW_NEXT_SP  $06
.define DRAW_SCORE    $08
.define TOP           $00
.define LEFT          $01

;don't forget "lda  ($08,X)"   "lda  ($00),y"  zero page transfer

.define PTR2000             $10
.define PTR2001             $12
.define PTR2002             $14
.define PTR2003             $16
.define PTR2004             $18
.define PTR2005             $1A
.define PTR2006             $1C
.define PTR2007             $1E

;page0 0x0000 ~0x00FF
.define getBlock_i          $20
.define getBlock_j          $21
.define getBlock_idx        $22
.define getBlock_ret        $23
.define setBoard_x          $24
.define setBoard_y          $25
.define setBoard_val        $26
.define setBoard_base       $27
.define getBase_x           $28
.define getBase_y           $29
.define getBase_hi          $2A
.define getBase_lo          $2B
.define getSPBase_x         $2F
.define getSPBase_y         $30
.define DrawLine_i          $31
.define DrawLine_y          $32
.define DrawShape_i         $33
.define DrawShape_j         $34
.define Clear_i             $35
.define TouchDo_j           $36
.define TouchDo_k           $37
.define TouchDo_temp        $3F
.define rotate_n            $38
.define currentPlayer       $39
.define readJoystick_player $3A
.define CurSP_player        $3B
.define CurBoard_player     $3C
.define nmiflg              $3D
.define rand0               $3E

;current player
.define PosX             $40
.define PosY             $41
.define NextShapeNo      $42
.define NowShapeNo       $43
.define NowDirectionNo   $44
.define lastkey          $45
.define key1             $46
.define key2             $47
.define Score0           $48
.define Score1           $49
.define Score2           $4A
.define Score3           $4B
.define TimeCount        $4C
.define isOn             $4D


;page1: 0x0100 ~0x01FF stack

;page2: 0x0200 ~0x02FF
;size:100*2(0x0C8) player board 1,2
.define Board            $0200
;size:16*2(0x020)  player  data 1,2
.define Player           $02C8
;size:16(0x010)
.define DrawBuff         $02E8

;* %b - Numerical 8-bit value
;* %w - Numerical 16-bit value
;* %l - Numerical 32-bit value
;* %v - Assembler name of a (global) variable or function
;* %o - Stack offset of a (local) variable
;* %g - Assembler name of a C label
;* %s - The argument is converted to a string
;* %% - The % sign itself

.segment    "CODE"

.proc    _nmi: near
  php
  pha
  dec rand0
  bpl lbl1;<0
    lda #$06
    sta rand0
  lbl1:
  sec;lda #$80
  ror nmiflg;sta nmiflg
  pla
  plp
  rts
.endproc

.proc    _waitvblank: near
    lbl1:
    rol nmiflg;lda nmiflg
    bcc lbl1;bpl lbl1
    clc;lda #$00
    ror nmiflg;sta nmiflg
    ldy #$00
    ldx #$00
    rts
.endproc

.proc    _getBase: near; 会改变Y
    ;lda currentPlayer ;player 0x00,0x10
    clc
    adc currentPlayer;getBase_item
    tax
    lda _dataUser+TOP,X
    clc
    adc getBase_y
    tay
    lsr;>>
    lsr
    lsr
    pha
    tya
    asl;<<
    asl
    asl
    asl
    asl
    clc
    adc _dataUser+LEFT,X
    clc
    adc getBase_x
    clc
    adc #$20
    sta getBase_lo
    pla
    adc #$20
    sta getBase_hi
    rts
.endproc

.proc    _st2005: near
    ;[0x2005] = 0;
    lda #$00
    tay
    sta (PTR2005),Y
    ;[0x2005] = 0;
    sta (PTR2005),Y
    rts
.endproc

.proc    _DrawScore: near
    ldy #$04
    fory:
        ldx Score0-1,Y
        inx
        cpx #$0A
        stx Score0-1,Y
        bne else1
        ldx #$00
        stx Score0-1,Y
    dey
    bne fory

    else1:
    lda #$00
    sta getBase_x
    sta getBase_y
    lda #DRAW_SCORE
    ;sta getBase_item
    jsr _getBase
    
    jsr _waitvblank;绘图PPU前调用 ;XY is 0
    lda getBase_hi
    sta (PTR2006),Y
    lda getBase_lo
    sta (PTR2006),Y
    clc
    ldx #$16
    txa
    adc Score0
    sta (PTR2007),Y
    txa
    adc Score1
    sta (PTR2007),Y
    txa
    adc Score2
    sta (PTR2007),Y
    txa
    adc Score3
    sta (PTR2007),Y
    jsr _st2005
    rts
.endproc

.proc _split:near;入参：C A，返回A
  bcc else1
    lsr
    lsr
    lsr
    lsr
    rts
  else1:
    and #$0F
  rts
.endproc

.proc _getBlock: near
    ;A:getBlock_idx
    ;sta getBlock_idx
    ;getBlock_i <<= 1;
    ;lda getBlock_i
    asl getBlock_i
    ;getBlock_idx += getBlock_i;
    ;clc
    adc getBlock_i
    sta getBlock_idx
    lsr getBlock_i

    ;getBlock_j >>= 1;
    lda getBlock_j
    lsr
    ;getBlock_idx += getBlock_j;
    clc
    adc getBlock_idx
    ;sta getBlock_idx);
    ;ldx getBlock_idx);
    tax
    
    ;if(getBlock_j & 1)
    lda getBlock_j
    lsr;and #$01
    lda _block,X
    jsr _split
    sta getBlock_ret
    rts
.endproc

.proc    _getNowBlock: near
    ;char idx = (NowShapeNo<<5)+(NowDirectionNo<<3)+(getBlock_i<<1)+(getBlock_j>>1);
    ;getBlock_idx = NowShapeNo;
    lda NowShapeNo
    ;getBlock_idx <<= 5;
    asl
    asl
    asl
    asl
    asl
    sta getBlock_idx
        
    ;getBlock_temp = NowDirectionNo;
    lda NowDirectionNo
    ;getBlock_temp <<= 3;
    asl
    asl
    asl
    ;getBlock_idx += getBlock_temp;
    clc
    adc getBlock_idx
    jsr _getBlock
    rts
.endproc

.proc    _getNextBlock: near
    ;char idx = (NextShapeNo<<5)+(i<<1)+(j>>1);
    ;getBlock_idx = NextShapeNo;
    lda NextShapeNo
    ;getBlock_idx <<= 5;
    asl
    asl
    asl
    asl
    asl
    jsr _getBlock
    rts
.endproc

.proc    _getNowBottom: near
    ;char idx = (NowShapeNo<<3)+(NowDirectionNo<<1)+(j>>1);
    ;getBlock_idx = NowShapeNo;
    lda NowShapeNo
    ;getBlock_idx <<= 3;
    asl
    asl
    asl
    sta getBlock_idx
    
    ;getBlock_temp = NowDirectionNo;
    lda NowDirectionNo
    ;getBlock_temp <<= 1;
    asl
    ;getBlock_idx += getBlock_temp;
    clc
    adc getBlock_idx
    sta getBlock_idx

    ;getBlock_j >>= 1;
    lda getBlock_j
    lsr
    ;getBlock_idx += getBlock_j;
    clc
    adc getBlock_idx
    ;sta getBlock_idx);
    ;ldx getBlock_idx);
    tax
    ;if(getBlock_j & 1)
    lda getBlock_j
    lsr;and #$01
    lda _bottom,X
    jsr _split
    sta getBlock_ret
    rts
.endproc

.proc    _getNowLeft: near
    ;char idx = (NowShapeNo<<2)+NowDirectionNo;
    ;getBlock_idx = NowShapeNo;
    lda NowShapeNo
    ;getBlock_idx <<= 2;
    asl
    asl
    ;getBlock_idx += NowDirectionNo;
    clc
    adc NowDirectionNo
    ;getBlock_ret = data3[getBlock_idx];
    tax
    lda _left,X
    sta getBlock_ret
    rts
.endproc

.proc    _calcBoardAddress: near
    ;x:0~9 y:0~19
    ;base =y*5
    ;setBoard_base=setBoard_y;
    lda setBoard_y
    
    ;setBoard_base+=(setBoard_y<<2);
    asl
    asl
    clc
    adc setBoard_y
    sta setBoard_base
    
    ;setBoard_x >>= 1;
    lda setBoard_x
    lsr
    ;setBoard_base+=setBoard_x;
    clc
    adc setBoard_base
    clc
    adc CurBoard_player
    sta setBoard_base
    
    ;setBoard_y = setBoard_x & 1;
    lda setBoard_x
    lsr;and #$01
    rts
.endproc

.proc    _setBoard: near
    jsr _calcBoardAddress
    ;if(setBoard_y)
    bcc else1;beq else1
        ;setBoard_val<<=4;
        lda setBoard_val
        asl
        asl
        asl
        asl
        sta setBoard_val
        ;Board[setBoard_base] = (Board[setBoard_base] & 0x0F) | setBoard_val;;high 4
        ldx setBoard_base
        lda Board,X
        and #$0F
        ora setBoard_val
        sta Board,X
        rts
    else1:
    ;Board[setBoard_base] = (Board[setBoard_base] & 0xF0) | setBoard_val;;low 4
    ldx setBoard_base
    lda Board,X
    and #$F0
    ora setBoard_val
    sta Board,X
    rts
.endproc

.proc    _getBoard: near
    jsr _calcBoardAddress
    ldx setBoard_base
    lda Board,X
    jsr _split
    sta setBoard_val
    rts
.endproc

.proc    _SendBuffToPPU: near
    clc
    adc CurSP_player
    jsr _waitvblank;绘图PPU前调用 ;XY is 0
    sta (PTR2003),Y
    ;for(i=0;i<16;i++)
    ;DrawShape_i=0;
    fori2:
        ;*(char*)(0x2004)=DrawBuff[DrawShape_i];
        lda DrawBuff,X
        sta (PTR2004),Y
    inx
    cpx #$10
    bne fori2
    rts
.endproc

.proc    _getSPBase: near;入参A ：getSPBase_item
    ;lda currentPlayer;player 0x00,0x10
    clc
    adc currentPlayer;getSPBase_item
    tax
    
    ;DrawShape_temp = DrawShape_j;
    lda getSPBase_y
    clc
    adc #$01
    ;DrawShape_temp <<= 3;
    asl
    asl
    asl
    ;DrawShape_temp += DRAW_NEXT_SP_TOP;
    clc
    adc _dataUser+TOP,X
    sec
    ;DrawShape_temp--;
    sbc #$01

    ;DrawBuff[DrawShape_idx++]=DrawShape_temp;
    sta DrawBuff,Y
    iny

    ;DrawBuff[DrawShape_idx++]=getBlock_ret;
    lda getBlock_ret
    sta DrawBuff,Y
    iny

    ;DrawBuff[DrawShape_idx++]=0;
    lda #$00
    sta DrawBuff,Y
    iny
    
    ;DrawShape_temp=(DrawShape_i-1)<<3
    lda getSPBase_x
    asl
    asl
    asl
    clc
    adc _dataUser+LEFT,X

    ;DrawBuff[DrawShape_idx++]=DrawShape_temp;
    sta DrawBuff,Y
    iny
    rts
.endproc

.proc    _DrawShapeNext: near
    ;DrawShape_idx=0;
    ldy #$00
    
    ;DrawShape_i=4;
    lda #$03
    sta getBlock_i;DrawShape_i
    ;for(i=4;i>0;i--)
    fori:
        ;DrawShape_j=4;
        lda #$03
        sta getBlock_j;DrawShape_j
        ;for(j=4;j>0;j--)
        forj:
            ;getBlock_i = DrawShape_i-1;
            ;ldx DrawShape_i
            ;stx getBlock_i
            lda getBlock_i
            sta getSPBase_x

            ;getBlock_j = DrawShape_j-1;
            ;ldx DrawShape_j
            ;stx getBlock_j
            lda getBlock_j
            sta getSPBase_y

            jsr _getNextBlock
            ;if(getBlock_ret)
            ;lda getBlock_ret); a is getBlock_ret
            beq else1
                lda #DRAW_NEXT_SP
                ;sta getSPBase_item
                jsr _getSPBase
            else1:
        dec getBlock_j;DrawShape_j
        bpl forj
    dec getBlock_i;DrawShape_i
    bpl fori

    ;*(char*)(0x2003)=16; ;i<<2
    lda #$10
    jsr _SendBuffToPPU
    rts
.endproc

.proc    _DrawShapeNow: near
    ;DrawShape_idx=0;
    ldy #$00

    ;DrawShape_i=4;
    lda #$03
    sta getBlock_i;DrawShape_i
    ;for(i=4;i>0;i--)
    fori:
        ;DrawShape_j=4;
        lda #$03
        sta getBlock_j;DrawShape_j
        ;for(j=4;j>0;j--)
        forj:
            ;getBlock_i=DrawShape_i-1;
            ;ldx DrawShape_i
            ;stx getBlock_i
            lda getBlock_i
            ;getSPBase_x = DrawShape_i - 1 + PosX
            ;lda DrawShape_i);
            clc
            adc PosX
            sta getSPBase_x

            
            ;getBlock_j=DrawShape_j-1;
            ;ldx DrawShape_j
            ;stx getBlock_j
            
            lda getBlock_j
            ;getSPBase_y = DrawShape_j-1+19-PosY
            ;lda DrawShape_j);
            clc
            adc #$13;19
            sec
            sbc PosY
            sta getSPBase_y

            jsr _getNowBlock
            ;if(getBlock_ret)
            ; lda getBlock_ret); a is getBlock_ret
            beq else1
                lda #DRAW_SP
                ;sta getSPBase_item
                jsr _getSPBase
            else1:
        dec getBlock_j;DrawShape_j
        bpl forj
    dec getBlock_i;DrawShape_i
    bpl fori

    ;*(char*)(0x2003)=0; ;i<<2
    lda #$00
    jsr _SendBuffToPPU
    rts
.endproc

.proc    _Clear: near
    ;for(i=0;i<100;i++)
    ldx #$64;99 times
    ldy CurBoard_player
    lda #$00
    fori:
        sta Board,Y
        iny
    dex
    bne fori

    ;calc base address
    ;A is 0;lda #$00
    sta getBase_x
    sta getBase_y
    lda #DRAW_BOARD
    ;sta getBase_item
    jsr _getBase

    
    lda #$14;20 times
    sta Clear_i
    fori2:
        jsr _waitvblank;绘图PPU前调用 ;XY is 0
        ;*(char*)(0x2006)=getBase_hi;
        lda getBase_hi
        sta (PTR2006),Y
        ;*(char*)(0x2006)=getBase_lo;
        lda getBase_lo
        sta (PTR2006),Y
        clc
        adc #$20
        sta getBase_lo
        lda getBase_hi
        adc #$00
        sta getBase_hi
        
        txa;lda #$00
        ;for(i=10;i>0;i--)
        ldx #$0A;10 times
        forf:
            ;*(char*)(0x2007)=0;
            sta (PTR2007),Y
        dex
        bne forf
        jsr _st2005
    dec Clear_i
    bne fori2
    
    ldx #$09
    stx Score0
    stx Score1
    stx Score2
    stx Score3
    jsr _DrawScore
    rts
.endproc

.proc    _NextShape: near
    ;NowShapeNo = NextShapeNo;
    lda NextShapeNo
    sta NowShapeNo

    ;NowDirectionNo = 0;
    lda #$00
    sta NowDirectionNo

    ;PosY = 19;
    lda #$13
    sta PosY
    ;PosX = 3;
    lda #$03
    sta PosX

    lda rand0 ;jsr_getRnd7
    ;NextShapeNo = getRnd7_ret;
    ;lda getRnd7_ret); a is getRnd7_ret
    sta NextShapeNo
    rts
.endproc

.proc    _Touch: near
    ;for(Touch_i=4;Touch_i>0;Touch_i--)
    lda #$03
    sta getBlock_j;Touch_i
    fori:
        ;getBlock_j = Touch_i-1;
        ;ldx Touch_i
        ;stx getBlock_j

        jsr _getNowBottom
        ;if(getBlock_ret)
        ;lda getBlock_ret); a is getBlock_ret
        beq else1
            ;if(getBlock_ret-1==PosY)goto ret1;
            tax;ldx getBlock_ret); a is getBlock_ret
            dex
            cpx PosY
            beq ret1
            
            ;setBoard_x=PosX+Touch_i-1;
            lda PosX
            clc
            adc getBlock_j;Touch_i
            sta setBoard_x

            ;setBoard_y=PosY-getBlock_ret;
            lda PosY
            sec
            sbc getBlock_ret
            sta setBoard_y
            
            jsr _getBoard
            ;if(setBoard_val)goto ret1;
            ;lda setBoard_val);  a is setBoard_val
            bne ret1
        else1:
    dec getBlock_j;Touch_i
    bpl fori
    
    ;return 0;
    clc;lda #$00
    ;sta Touch_ret);
    rts
    ret1:
    ;return 1;
    sec;lda #$01
    ;sta Touch_ret);
    rts
.endproc

.proc    _getBoardBase: near; 会改变Y
    lda #$13;19-getBase_y
    sec
    sbc getBase_y
    sta getBase_y
    lda #DRAW_BOARD
    ;sta getBase_item
    jsr _getBase
    rts
.endproc

.proc    _DrawLine: near
    ;getBase_x=0;
    lda #$00
    sta getBase_x
    ;getBase_y=DrawLine_y;
    lda DrawLine_y
    sta getBase_y
    jsr _getBoardBase
    ;DrawBuff[0]=getBase_hi;
    ldx #$00
    lda getBase_hi
    sta DrawBuff,X
    ;DrawBuff[1]=getBase_lo;
    inx
    lda getBase_lo
    sta DrawBuff,X
    
    lda #$00
    sta setBoard_x;setBoard_x is count 0~9

    ;for(l=0;l<10;l++)
    lda #$0A;10 times
    sta DrawLine_i
    fori1:
        ;setBoard_x=l;
        ;setBoard_y=DrawLine_y;
        lda DrawLine_y
        sta setBoard_y
        jsr _getBoard
        ;DrawBuff[2+l]=setBoard_val;
        ;lda setBoard_val); a is setBoard_val
        ldx setBoard_x
        inx
        inx
        sta DrawBuff,X

        inc setBoard_x;setBoard_x++
    dec DrawLine_i
    bne fori1
    
    jsr _waitvblank;绘图PPU前调用 ;XY is 0
    ;*(char*)(0x2006)=DrawBuff[0];
    lda DrawBuff,X
    sta (PTR2006),Y
    inx;x=1
    ;*(char*)(0x2006)=DrawBuff[1];
    lda DrawBuff,X
    sta (PTR2006),Y
    inx;x=2
    
    ;for(i=2;i<12;i++)
    fori2:
        ;*(char*)(0x2007)=DrawBuff[i];
        lda DrawBuff,X
        sta (PTR2007),Y
        inx
    cpx #$0C
    bne fori2
    jsr _st2005
    rts
.endproc

.proc    _TouchDo: near
    ;add to board
    ;{
        ;TouchDo_idx=0;
        ldy #$00
        ;for(TouchDo_i=3;TouchDo_i>=0;TouchDo_i--)
        lda #$03
        sta getBlock_i;TouchDo_i
        fori1:
            ;for(TouchDo_j=3;TouchDo_j>=0;TouchDo_j--)
            lda #$03
            sta getBlock_j;TouchDo_j
            forj1:
                ;getBlock_i=TouchDo_i;
                lda getBlock_i;ldx TouchDo_i
                ;stx getBlock_i
                ;setBoard_x=TouchDo_i+PosX;
                ;getBase_x =TouchDo_i+PosX;
                ;ldx TouchDo_i);
                ;dex");
                ;txa
                clc
                adc PosX
                sta setBoard_x
                sta getBase_x
                
                ;getBlock_j=TouchDo_j;
                ;ldx TouchDo_j
                ;stx getBlock_j

                ;setBoard_y=PosY-(TouchDo_j);
                ;getBase_y =PosY-(TouchDo_j);
                lda PosY
                ;inx");
                ;txa");
                sec
                sbc getBlock_j;getBlock_j is TouchDo_j
                sta setBoard_y
                sta getBase_y

                jsr _getNowBlock
                ;if(getBlock_ret)
                ;lda getBlock_ret); a is getBlock_ret
                beq else1
                
                    ;setBoard_val=getBlock_ret;
                    lda getBlock_ret
                    sta setBoard_val
                    jsr _setBoard

                    sty TouchDo_temp
                    jsr _getBoardBase;会改变Y
                    ldy TouchDo_temp
                    ;DrawBuff[TouchDo_idx++]=getBase_hi;
                    ;lda getBase_hi); a is getBase_hi
                    sta DrawBuff,Y
                    iny
                    
                    ;DrawBuff[TouchDo_idx++]=getBase_lo;
                    lda getBase_lo
                    sta DrawBuff,Y
                    iny

                    ;DrawBuff[TouchDo_idx++]=getBlock_ret;
                    lda getBlock_ret
                    sta DrawBuff,Y
                    iny
                else1:
            dec getBlock_j;TouchDo_j
            bpl forj1
        dec getBlock_i;TouchDo_i
        bpl fori1
    
        ;idx=0;
        jsr _waitvblank;绘图PPU前调用 ;XY is 0
        ;for(TouchDo_i=4;TouchDo_i>0;TouchDo_i--)
        fori2:
            ;*(char*)(0x2006)=DrawBuff[idx++];
            lda DrawBuff,X
            sta (PTR2006),Y
            inx
            ;*(char*)(0x2006)=DrawBuff[idx++];
            lda DrawBuff,X
            sta (PTR2006),Y
            inx
            ;*(char*)(0x2007)=DrawBuff[idx++];
            lda DrawBuff,X
            sta (PTR2007),Y
            inx
        cpx #$0C
        bne fori2
        jsr _st2005
    ;}

    ;clear line
    ;{
        ;for(TouchDo_j=19;TouchDo_j>=0;TouchDo_j--)
        lda #$13
        sta TouchDo_j
        forj3:
            ;TouchDo_line=1;
            ;lda #$01");
            ;sta TouchDo_line);
            ;for(TouchDo_i=9;TouchDo_i>=0;TouchDo_i--)
            lda #$09
            sta setBoard_x;TouchDo_i 保证setBoard_x在_getBoard 中未被修改
            fori3:
                ;setBoard_x=TouchDo_i;
                ;ldx TouchDo_i
                ;stx setBoard_x
                ;setBoard_y=TouchDo_j;    
                ldx TouchDo_j
                stx setBoard_y
                jsr _getBoard
                ;if(setBoard_val==0)
                ;lda setBoard_val); a is setBoard_val
                beq continue;bne else2
                    ;TouchDo_line=0;
                    ;lda #$00"); a is 0
                    ;sta TouchDo_line);
                    ;lda #$00
                    ;jmp else5;break1
                ;else2:
            dec setBoard_x;TouchDo_i
            bpl fori3
            ;lda #$01
            ;break1:
            ;if(TouchDo_line)
            ;ldx TouchDo_line);
            ;beq else5
            ;bne if5);
            ;jmp else5);

            jsr _DrawScore
            ;for(TouchDo_k=0;TouchDo_k<9;TouchDo_k++)
            lda #$09;10 times
            sta setBoard_x;TouchDo_k 保证setBoard_x在_getBoard 中未被修改
            fork1:
                ;setBoard_x=TouchDo_k-1;
                ;ldx TouchDo_k
                ;stx setBoard_x
                
                ;if(TouchDo_j-1>=0)
                ldx TouchDo_j
                dex
                bmi else3
                    ;setBoard_y=TouchDo_j-1;
                    stx setBoard_y
                    jsr _getBoard;in setBoard_val
                    ;setBoard_val=data1[setBoard_val+0xE0];
                    ;lda setBoard_val); a is setBoard_val
                    ;clc
                    ;adc #$E0
                    tax
                    lda _chg,X
                    and #$0F
                    sta setBoard_val
                    jsr _setBoard
                else3:

                ;setBoard_y=TouchDo_j+1;因为一般不会刷最上面一行，所以不用判断TouchDo_j+1是否大于等于20
                ldx TouchDo_j
                inx
                stx setBoard_y
                jsr _getBoard
                ;setBoard_val=data1[setBoard_val+0xF0];
                ;lda setBoard_val); a is setBoard_val
                ;clc
                ;adc #$F0
                tax
                lda _chg,X
                lsr
                lsr
                lsr
                lsr
                sta setBoard_val
                jsr _setBoard
                
            dec setBoard_x;TouchDo_k
            bpl fork1
            
            ;if(TouchDo_j-1>=0)
            ldx TouchDo_j
            dex
            bmi else4
                ;DrawLine_y=TouchDo_j-1;
                stx DrawLine_y
                jsr _DrawLine
            else4:
            
            ;for(TouchDo_k=TouchDo_j;TouchDo_k<19;TouchDo_k++)
            ldx TouchDo_j
            ;dex
            stx TouchDo_k
            fork2:
                ;for(TouchDo_i=9;TouchDo_i>=0;TouchDo_i--)
                lda #$09
                sta setBoard_x;TouchDo_i
                fori4:
                    ;setBoard_x=TouchDo_i;
                    ;ldx TouchDo_i
                    ;stx setBoard_x
                    
                    ;setBoard_y=TouchDo_k+1;
                    ldx TouchDo_k
                    inx
                    stx setBoard_y
                    jsr _getBoard

                    ;setBoard_y=TouchDo_k;
                    ldx TouchDo_k
                    stx setBoard_y
                    jsr _setBoard
                dec setBoard_x;TouchDo_i
                bpl fori4
                
                ;DrawLine_y=TouchDo_k;
                ldx TouchDo_k
                stx DrawLine_y
                jsr _DrawLine
            inc TouchDo_k
            lda TouchDo_k
            cmp #$13
            bne fork2
            
            continue:
        dec TouchDo_j
        bmi forjext
        jmp forj3
        forjext:
    ;}
    
    ;is gameover
    ;for(TouchDo_i=0;TouchDo_i<=9;TouchDo_i++)
    lda #$09
    sta setBoard_x;TouchDo_i
    fori5:
        ;setBoard_x=TouchDo_i-1;
        ;ldx TouchDo_i
        ;stx setBoard_x
        ;setBoard_y=18;
        ldx #$12
        stx setBoard_y
        jsr _getBoard

        ;if(setBoard_val)
        ;lda setBoard_val); a is setBoard_val
        beq else6
            jsr _Clear
            jmp break2
        else6:
    dec setBoard_x;TouchDo_i
    bpl fori5
    break2:
    
    jsr _NextShape
    rts
.endproc

.proc    _AnyTouch: near ; 返回：C
    ;if(PosX<0)
    lda PosX
    bpl else1
    beq else1
        ;AnyTouch_ret=1;
        sec;lda #$01
        rts
    else1:

    jsr _getNowLeft
    ;if(getBlock_ret+PosX-10>0)  ->  (getBlock_ret+PosX>=11)
    ;lda getBlock_ret); a is getBlock_ret
    clc
    adc PosX
    cmp #$0B
    bmi else2
        ;AnyTouch_ret=1;
        sec;lda #$01
        ;sta AnyTouch_ret);
        rts
    else2:
    
    ;for(AnyTouch_i=4;AnyTouch_i>0;AnyTouch_i--)
    lda #$03
    sta getBlock_i;AnyTouch_i
    fori:
        ;for(AnyTouch_j=4;AnyTouch_j>0;AnyTouch_j--)
        lda #$03
        sta getBlock_j;AnyTouch_j
        forj:
            ;getBlock_j = AnyTouch_j-1;
            ;ldx AnyTouch_j
            ;stx getBlock_j

            jsr _getNowBottom
            ;if(getBlock_ret-1-PosY>0)  ->  (getBlock_ret-1-1>=PosY)
            tax;ldx getBlock_ret); a is getBlock_ret
            dex
            dex
            cpx PosY
            bmi else3
                ;AnyTouch_ret=1;
                sec;lda #$01
                rts
            else3:
            
            ;getBlock_i=AnyTouch_i-1;
            ;lda AnyTouch_i
            ;sta getBlock_i
            lda getBlock_i
            ;setBoard_x=PosX+AnyTouch_i-1;
            clc
            adc PosX
            sta setBoard_x


            ;getBlock_j=AnyTouch_j-1;
            ;ldx AnyTouch_j
            ;stx getBlock_j
            ;setBoard_y=PosY-(AnyTouch_j-1);
            lda PosY
            sec
            sbc getBlock_j
            sta setBoard_y
            
            jsr _getNowBlock

            jsr _getBoard

            ;if(getBlock_ret==0||setBoard_val==0){}else
            ;lda setBoard_val); a is setBoard_val
            beq else4
            lda getBlock_ret
            beq else4
                ;AnyTouch_ret=1;
                sec;lda #$01
                rts
            else4:
        dec getBlock_j;AnyTouch_j
        bpl forj
    dec getBlock_i;AnyTouch_i
    bpl fori
    ;AnyTouch_ret=0;
    clc;lda #$00
    rts
.endproc

.proc _rotate: near
    ;NowDirectionNo=(NowDirectionNo+rotate_n)&3;
    lda NowDirectionNo
    clc
    adc rotate_n
    and #$03
    sta NowDirectionNo
    ;rotate_tempPosX=PosX;
    lda PosX
    pha;sta rotate_tempPosX
    
    jsr _getNowLeft
    ;if(PosX+getBlock_ret>10) -> (PosX+getBlock_ret>=11)
    ;lda getBlock_ret); a is getBlock_ret
    clc
    adc PosX
    cmp #$0B
    bmi else1
        ;PosX=10-getBlock_ret;
        lda #$0A
        sec
        sbc getBlock_ret
        sta PosX
    else1:
    
    ;旋转偏移后，如果触碰的话，返回保存的值，并旋回
    jsr _AnyTouch
    ;if(AnyTouch_ret)jmp jp1);
    ;lda AnyTouch_ret);
    pla
    bcc else2
        ;PosX=rotate_tempPosX;
        ;lda rotate_tempPosX
        sta PosX
        ;NowDirectionNo=(NowDirectionNo-rotate_n)&3;
        lda NowDirectionNo
        sec
        sbc rotate_n
        and #$03
        sta NowDirectionNo
    else2:
    rts
.endproc

.proc    _movel: near
    inc PosX
    jsr _AnyTouch
    bcc else1
        dec PosX
    else1:
    rts
.endproc

.proc    _mover: near
    dec PosX
    jsr _AnyTouch
    bcc else1
        inc PosX
    else1:
    rts
.endproc

.proc    _down: near
    loop1:
        jsr _Touch
        dec PosY
    bcc loop1
    inc PosY
    jsr _TouchDo
    rts
.endproc

.proc    _slowdown: near
    jsr _Touch
    bcc else1
        jsr _TouchDo
        rts
    else1:
    dec PosY
    rts
.endproc

.proc    _readJoystick: near
    ldx readJoystick_player

    ;*(char*)(0x4016/7)=01;
    lda #$01
    sta key1;key1=1;  1<<8
    sta $4016,X
    ;*(char*)(0x4016/7)=00;
    lda #$00
    sta $4016,X
    

    fori:
        lda $4016,X
        ror
        rol key1
    bcc fori;key1=1;  1<<8
    lda key1
    rts
.endproc

.proc    _Clear4by4: near
    lda #$00
    sta getBase_x
    sta getBase_y
    lda #DRAW_NEXT
    ;sta getBase_item
    jsr _getBase
    jsr _waitvblank;绘图PPU前调用 ;XY is 0
    ldx #$04
    fori1:
        lda getBase_hi
        sta (PTR2006),Y
        lda getBase_lo
        sta (PTR2006),Y
        lda #$00
        sta (PTR2007),Y
        sta (PTR2007),Y
        sta (PTR2007),Y
        sta (PTR2007),Y

        lda getBase_lo
        clc
        adc #$20
        sta getBase_lo
        lda getBase_hi
        adc #$00
        sta getBase_hi
    dex
    bne fori1
    
    jsr _st2005
    rts
.endproc

.proc    _mainSub: near
    ;jsr _loadplayer
    ;{
        ldy currentPlayer
        ldx #$0F
        for_loadplayer:
            lda Player,Y
            sta PosX,X
            dey
        dex
        bpl for_loadplayer
    ;}
    lda isOn
    bne else2
        ;check player1 key
        jsr _readJoystick
        ;lda key1); a is key1
        beq else4
            sta lastkey
            jsr _Clear
            jsr _Clear4by4
            jsr _NextShape
            jsr _NextShape
            lda #$32
            sta TimeCount
            ;lda #$01
            sta isOn
        else4:
        jmp _saveplayer
    else2:

    ;one second count
    ;if(TimeCount--==0)
    dec TimeCount
    bne else1
        ;TimeCount=50;
        lda #$32
        sta TimeCount
        jsr _slowdown
    else1:
    
    jsr _readJoystick
    ;key2 = key1 ^ lastkey;
    ;lda key1); a is key1
    eor lastkey
    ;key2 = key2 & key1;
    and key1;取得有变化并且被按下的键，就是刚被按下的瞬间
    ;if(key2!=0)
    sta key2
    lsr key2;if(key2&0x01){movelr_n=1;movelr();};button_RIGHT
    bcc elseKey1
        jsr _movel
    elseKey1:
    lsr key2;if(key2&0x02){movelr_n=-1;movelr();};button_LEFT
    bcc elseKey2
        jsr _mover
    elseKey2:
    lsr key2;if(key2&0x04)down();;button_DOWN
    bcc elseKey3
        jsr _down
    elseKey3:
    lsr key2;if(key2&0x08)slowdown();;button_UP
    bcc elseKey4
        jsr _slowdown
    elseKey4:
    lsr key2;if(key2&0x10);;button_START
    lsr key2;if(key2&0x20);;button_SELECT
    lsr key2;if(key2&0x40){rotate_n=-1;rotate();};button_B
    bcc elseKey5
        lda #$FF
        sta rotate_n
        jsr _rotate
    elseKey5:
    lsr key2;if(key2&0x80){rotate_n=1;rotate();};button_A
    bcc elseKey6
        lda #$01
        sta rotate_n
        jsr _rotate
    elseKey6:
    jsr _DrawShapeNow
    jsr _DrawShapeNext
    ;lastkey = key1;
    lda key1
    sta lastkey
    _saveplayer:
    ;jsr _saveplayer
    ;{
        ldy currentPlayer
        ldx #$0F
        for_saveplayer:
            lda PosX,X
            sta Player,Y
            dey
        dex
        bpl for_saveplayer
    ;}
    rts
.endproc

.proc    _Player1: near
    ;currentPlayer = 0;
    lda #$00
    sta currentPlayer
    ;readJoystick_player = 0;
    sta readJoystick_player
    ;CurSP_player=0;
    sta CurSP_player
    ;CurBoard_player=0;
    sta CurBoard_player
    rts
.endproc

.proc    _Player2: near
    ;currentPlayer = 0x10;
    lda #$10
    sta currentPlayer
    ;readJoystick_player = 1;
    lda #$01
    sta readJoystick_player
    ;CurSP_player=0x20;
    lda #$20
    sta CurSP_player
    ;CurBoard_player=100;
    lda #$64
    sta CurBoard_player
    rts
.endproc

.proc    _initTitle: near
    ldx #$11
    loop1:
      stx $2007
      inx
    cpx #$16
    bne loop1
    rts
.endproc

.proc    _main: near
    ;init ptr
    ;{
        lda #$20
        ldy #$00
        ldx #$00;PTR2000
        fori:
            sty $10,X
            iny
            inx
            sta $10,X
            inx
        cpx #$10 ;PTR2007+1
        bne fori
    ;}
	;open nmi
	lda #%10000000;#% 二进制
	sta (PTR2000),Y
	jsr _waitvblank;Y is 0
    ;set color
    ;{
        ;*(char*)(0x2006)=0x3F;
        lda #$3F
        sta (PTR2006),Y
        ;*(char*)(0x2006)=0x00;
        lda #$00
        sta (PTR2006),Y
        ;*(char*)(0x2007)=0x2C;
        lda #$2C
        sta (PTR2007),Y
        ;*(char*)(0x2007)=0x30;
        lda #$30
        sta (PTR2007),Y
        ;*(char*)(0x2007)=0x00;
        lda #$00
        sta (PTR2007),Y
        ;*(char*)(0x2007)=0x3F;
        lda #$3F
        sta (PTR2007),Y

        ;*(char*)(0x2006)=0x3F;
        lda #$3F
        sta (PTR2006),Y
        ;*(char*)(0x2006)=0x11;
        lda #$11
        sta (PTR2006),Y
        ;*(char*)(0x2007)=0x30;
        lda #$30
        sta (PTR2007),Y
        ;*(char*)(0x2007)=0x22;
        lda #$22
        sta (PTR2007),Y
        ;*(char*)(0x2007)=0x3F;
        lda #$3F
        sta (PTR2007),Y
    ;}
    ;set background black
    ;{
    jsr _waitvblank;Y is 0
        ;*(char*)(0x2006)=0x20;
        lda #$20
        sta (PTR2006),Y
        ;*(char*)(0x2006)=0x20;
        sta (PTR2006),Y
        
        ;total 0x380
        ldx #$80;lda #$80
        ;sta main_i
        ldy #$03;lda #$03
        ;sta main_j
        lda #$10
        for4:
            sta $2007
            cpx #$00
            bne else5
                dey;dec main_j
            else5:
            dex;dec main_i
        cpx #$00;lda main_i
        bne for4
        cpy #$00;lda main_j
        bne for4
    ;}
    ;show init title
    ;{
    jsr _waitvblank;Y is 0
        ;*(char*)(0x2006)=0x20;
        lda #$20
        sta (PTR2006),Y
        ;*(char*)(0x2006)=0xC5;
        lda #$C5
        sta (PTR2006),Y
        jsr _initTitle

        ;*(char*)(0x2006)=0x20;
        lda #$20
        sta (PTR2006),Y
        ;*(char*)(0x2006)=0xC5;
        lda #$D5
        sta (PTR2006),Y
        jsr _initTitle
    ;}
    ;{
        ;open rander
        lda #%00011110;#% 二进制
        sta (PTR2001),Y
        jsr _st2005
    ;}
    ;main cycle
    ;{
        while1:
            ;if(!player1On)
            jsr _Player1
            jsr _mainSub

            ;if(!player2On)
            jsr _Player2
            jsr _mainSub

        jmp while1
    ;}
    rts
.endproc



