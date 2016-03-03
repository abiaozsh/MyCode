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

;555
;4
;3333
;
;asl5: +6 -9
;asl4: +5 -8
;asl3: +4 -8


.segment    "RODATA"
_block:
;    .byte $07,$00,$03,$00,$03,$00,$06,$00,$25,$42,$00,$00,$00,$00,$00,$00 ;0
;    .byte $07,$00,$03,$00,$03,$00,$06,$00,$25,$42,$00,$00,$00,$00,$00,$00 ;1 1 长条
;    .byte $9B,$00,$A8,$00,$00,$00,$00,$00,$9B,$00,$A8,$00,$00,$00,$00,$00 ;2
;    .byte $9B,$00,$A8,$00,$00,$00,$00,$00,$9B,$00,$A8,$00,$00,$00,$00,$00 ;3 2 方
;    .byte $70,$00,$AB,$00,$06,$00,$00,$00,$95,$00,$80,$04,$00,$00,$00,$00 ;4
;    .byte $70,$00,$AB,$00,$06,$00,$00,$00,$95,$00,$80,$04,$00,$00,$00,$00 ;5 3 Z
;    .byte $07,$00,$98,$00,$60,$00,$00,$00,$B0,$04,$A5,$00,$00,$00,$00,$00 ;6
;    .byte $07,$00,$98,$00,$60,$00,$00,$00,$B0,$04,$A5,$00,$00,$00,$00,$00 ;7 4 反Z
;    .byte $07,$00,$03,$00,$48,$00,$00,$00,$00,$07,$25,$0A,$00,$00,$00,$00 ;8
;    .byte $95,$00,$30,$00,$60,$00,$00,$00,$2B,$04,$06,$00,$00,$00,$00,$00 ;9 5 反L
;    .byte $4B,$00,$03,$00,$06,$00,$00,$00,$07,$00,$28,$04,$00,$00,$00,$00 ;A
;    .byte $70,$00,$30,$00,$A5,$00,$00,$00,$25,$09,$00,$06,$00,$00,$00,$00 ;B 6 L
;    .byte $07,$00,$4E,$00,$06,$00,$00,$00,$70,$00,$F5,$04,$00,$00,$00,$00 ;C
;    .byte $70,$00,$C5,$00,$60,$00,$00,$00,$D5,$04,$60,$00,$00,$00,$00,$00 ;D 7 T

    .byte $70,$34,$38,$6C,$50,$21,$22,$43,$70,$34,$38,$6C,$50,$21,$22,$43;1 长条
    .byte $B0,$91,$84,$A5,$B0,$91,$84,$A5,$B0,$91,$84,$A5,$B0,$91,$84,$A5;2 方
    .byte $71,$B4,$A5,$68,$50,$91,$85,$46,$71,$B4,$A5,$68,$50,$91,$85,$46;3 Z
    .byte $70,$84,$95,$69,$B1,$42,$54,$A5,$70,$84,$95,$69,$B1,$42,$54,$A5;4 反Z
    .byte $70,$34,$88,$49,$72,$54,$25,$A6,$50,$91,$35,$69,$B0,$21,$42,$64;5 反L
    .byte $B0,$41,$34,$68,$70,$84,$25,$46,$71,$35,$58,$A9,$50,$21,$92,$66;6 L
    .byte $70,$E4,$45,$68,$71,$54,$F5,$46,$71,$54,$C5,$69,$50,$D1,$42,$65;7 T
_bottom:
    .byte $11,$11,$04,$00,$11,$11,$04,$00
    .byte $22,$00,$22,$00,$22,$00,$22,$00 ;0
    .byte $22,$01,$32,$00,$22,$01,$32,$00
    .byte $21,$02,$23,$00,$21,$02,$23,$00 ;1
    .byte $11,$02,$33,$00,$22,$02,$13,$00
    .byte $12,$01,$31,$00,$22,$02,$33,$00 ;2
    .byte $21,$01,$32,$00,$22,$02,$23,$00 ;3
_left:
    .byte $14,$14,$22,$22,$23,$23,$23,$23,$23,$23,$23,$23,$23,$23         ;4
_chg:
    .byte $00,$11,$45,$33,$41,$15,$66,$77,$68,$97,$A6,$7B,$C3,$9B,$3E,$A8 ;E chg
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

.define POS_ZERO      $16 ;22
.define POS_TITLE     $11 ;17
.define POS_BLACK     $10 ;16

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
.define getBlock_i          $30
.define getBlock_j          $31
.define getBlock_temp       $32
.define getBlock_ret        $33
.define setBoard_x          $34
.define setBoard_y          $35
.define setBoard_val        $36
.define setBoard_base       $37
.define getBase_x           $38
.define getBase_y           $39
.define getBase_hi          $3A
.define getBase_lo          $3B
.define getSPBase_x         $3C
.define getSPBase_y         $3D
.define DrawLine_i          $3E
.define DrawLine_y          $3F
.define DrawShape_i         $40
.define DrawShape_j         $41
.define Clear_i             $42
.define TouchDo_j           $43
.define TouchDo_k           $44
.define TouchDo_temp        $45
.define rotate_n            $46
.define currentPlayer       $47
.define readJoystick_player $48
.define CurSP_player        $49
.define CurBoard_player     $4A
.define nmiflg              $4B
.define rand7               $4C
.define main_temp           $4D
.define getBlock_idx        $4E

.define getBlockV2_idx      $4F

;current player
.define CURRENT_PLAYER   $50
.define PosX             $50
.define PosY             $51
.define NextShapeNo      $52
.define NowShapeNo       $53
.define NowDirectionNo   $54
.define lastkey          $55
.define key1             $56
.define key2             $57
.define Score0           $58
.define Score1           $59
.define Score2           $5A
.define Score3           $5B
.define TimeCount        $5C
.define isOn             $5D


;page1: 0x0100 ~0x01FF stack

;page2: 0x0200 ~0x02FF
;size:100*2(0x0C8) player board 1,2
.define Board            $0200
;size:16*2(0x020)  player  data 1,2
.define Player           $02C8
;size:16*2(0x020) now and next
.define DrawBuff         $0300

;* %b - Numerical 8-bit value
;* %w - Numerical 16-bit value
;* %l - Numerical 32-bit value
;* %v - Assembler name of a (global) variable or function
;* %o - Stack offset of a (local) variable
;* %g - Assembler name of a C label
;* %s - The argument is converted to a string
;* %% - The % sign itself

.segment    "CODE"

.proc _nmi: near
  php
  pha
  dec rand7
  bpl lbl1;<0
    lda #$06
    sta rand7
  lbl1:
  sec;lda #$80
  ror nmiflg;sta nmiflg
  pla
  plp
  rts
.endproc

.proc _waitvblank: near
    lbl1:
    rol nmiflg;lda nmiflg
    bcc lbl1;bpl lbl1
    clc;lda #$00
    ror nmiflg;sta nmiflg
    ldy #$00
    rts
.endproc

.proc _getBase: near; 会改变Y,入参：A:getBase_item   返回：A:getBase_hi
    ;A:getBase_item
    clc
    adc currentPlayer;player 0x00,0x10
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

.proc _st2005: near
    ;[0x2005] = 0;
    lda #$00
    tay
    sta (PTR2005),Y
    ;[0x2005] = 0;
    sta (PTR2005),Y
    rts
.endproc

.proc _DrawScore: near
    ldx #$03
    fory:
        inc Score0,X
        lda Score0,X
        cmp #($0A + POS_ZERO)
        bne break1
        lda #POS_ZERO
        sta Score0,X
    dex
    bpl fory
    break1:
    
    lda #$00
    sta getBase_x
    sta getBase_y
    lda #DRAW_SCORE
    ;sta getBase_item
    jsr _getBase
    
    jsr _waitvblank;绘图PPU前调用 ;Y is 0
    lda getBase_hi
    sta (PTR2006),Y
    lda getBase_lo
    sta (PTR2006),Y
    clc

    lda Score0
    sta (PTR2007),Y
    lda Score1
    sta (PTR2007),Y
    lda Score2
    sta (PTR2007),Y
    lda Score3
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
    asl
    asl
    adc getBlockV2_idx
    tax
    lda _block,X
    pha
    and #$03
    sta getBlock_j
    pla
    lsr
    lsr
    pha
    and #$03
    sta getBlock_i
    pla
    lsr
    lsr
    sta getBlock_ret
    rts
.endproc

.proc _combine_NowShapeNo_NowDirectionNo: near
    lda NowShapeNo
    asl
    asl
    clc
    adc NowDirectionNo
    rts
.endproc

.proc _getNowBlock: near
    ;char idx = (NowShapeNo<<5)+(NowDirectionNo<<3)+(getBlock_i<<1)+(getBlock_j>>1);
    jsr _combine_NowShapeNo_NowDirectionNo
    jsr _getBlock
    rts
.endproc

.proc _getNowBottom: near
    ;char idx = (NowShapeNo<<3)+(NowDirectionNo<<1)+(j>>1);
    jsr _combine_NowShapeNo_NowDirectionNo
    asl;<<
    asl;<<
    adc getBlock_j
    lsr;>>
    tax
    lda _bottom,X
    jsr _split
    sta getBlock_ret
    rts
.endproc

.proc _isRightTouch: near
    ;char idx = (NowShapeNo<<1)+(NowDirectionNo>>1);
    jsr _combine_NowShapeNo_NowDirectionNo
    lsr
    tax
    lda _left,X
    jsr _split
    sta getBlock_ret ;必须设值
    ;if(getBlock_ret+PosX-10>0)  ->  !(getBlock_ret+PosX<11)
    ;lda getBlock_ret); a is getBlock_ret
    clc
    adc PosX
    cmp #$0B;11
    rts
.endproc

.proc _calcBoardAddress: near
    ;x:0~9 y:0~19
    ;setBoard_base = (setBoard_y*10) + (setBoard_x>>1) + CurBoard_player
    ;base =y*5
    ;setBoard_base=setBoard_y;
    lda setBoard_y
    asl
    asl
    ;(clc)
    adc setBoard_y
    asl;A=setBoard_y*10
    ;(clc)
    adc setBoard_x
    lsr
    clc
    adc CurBoard_player
    sta setBoard_base
    
    ;setBoard_y = setBoard_x & 1;
    lda setBoard_x
    lsr;and #$01
    rts
.endproc

.proc _setBoard: near
    jsr _calcBoardAddress;out:C
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

.proc _getBoard: near
    jsr _calcBoardAddress;out:C
    ldx setBoard_base
    lda Board,X
    jsr _split;in C
    sta setBoard_val
    rts
.endproc

.proc _getSPBaseAndSetDrawBuff: near;入参A ：getSPBase_item
    ;lda currentPlayer;player 0x00,0x10
    clc
    adc currentPlayer;getSPBase_item
    tax
    
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
    
    ;DrawBuff[DrawShape_idx++]=0;
    lda #$00
    sta DrawBuff,Y
    iny
    
    ;DrawBuff[DrawShape_idx++]=getBlock_ret;
    lda getBlock_ret
    sta DrawBuff,Y
    iny
    
    ;DrawShape_temp = ((getSPBase_y+1)<<3+(_dataUser+TOP,X))-1
    ;DrawShape_temp = ((getSPBase_y)<<3+8-1+(_dataUser+TOP,X))
    ;DrawShape_temp = DrawShape_j;
    lda getSPBase_y
    asl
    asl
    asl
    clc
    adc #$07
    clc
    adc _dataUser+TOP,X

    ;DrawBuff[DrawShape_idx++]=DrawShape_temp;
    sta DrawBuff,Y
    iny

    rts
.endproc

.proc _DrawShapeNowAndNext: near
    ;DrawShape_idx=0;
    ldy #$00
    
    lda #$03
    sta getBlockV2_idx
    fori:
      lda NextShapeNo
      asl
      asl
      jsr _getBlock
      
      lda getBlock_i
      sta getSPBase_x

      lda getBlock_j
      sta getSPBase_y

      lda #DRAW_NEXT_SP
      ;sta getSPBase_item
      jsr _getSPBaseAndSetDrawBuff

      
      jsr _getNowBlock

      lda getBlock_i
      ;getSPBase_x = getBlock_i + PosX
      clc
      adc PosX
      sta getSPBase_x

      lda getBlock_j
      ;getSPBase_y = getBlock_j + 19 - PosY
      clc
      adc #$13;19
      sec
      sbc PosY
      sta getSPBase_y

      lda #DRAW_SP
      ;sta getSPBase_item
      jsr _getSPBaseAndSetDrawBuff
    dec getBlockV2_idx
    bpl fori

    ;*(char*)(0x2003)=16; ;i<<2
    lda #$10
    ;jsr _SendBuffToPPU
    clc
    adc CurSP_player
    jsr _waitvblank;绘图PPU前调用 ;Y is 0
    ldx #$1F
    sta (PTR2003),Y
    ;for(i=0;i<16;i++)
    ;DrawShape_i=0;
    fori2:
        ;*(char*)(0x2004)=DrawBuff[DrawShape_i];
        lda DrawBuff,X
        sta (PTR2004),Y
    dex
    bpl fori2
    rts
.endproc

.proc _Clear: near
    ;for(i=0;i<100;i++)
    ldx #$64;99 times
    ldy CurBoard_player
    lda #$00
    fori:
        sta Board,Y
        iny
    dex
    bne fori

    lda #$13;20 times
    sta DrawLine_y
    fori2:
        ldx DrawLine_y
        jsr _DrawLine
    dec DrawLine_y
    bpl fori2
    
    ldx #($09 + POS_ZERO)
    stx Score0
    stx Score1
    stx Score2
    stx Score3
    jsr _DrawScore
    rts
.endproc

.proc _NextShape: near
    ;NowShapeNo = NextShapeNo;
    lda NextShapeNo
    sta NowShapeNo

    lda rand7
    sta NextShapeNo

    ;PosY = 19;
    lda #$13
    sta PosY
    ;PosX = 3;
    lda #$03
    sta PosX

    rts
.endproc

.proc _isTouch: near
    lda #$03
    sta getBlock_j
    fori:
        jsr _getNowBottom
        ;if(getBlock_ret)
        ;lda getBlock_ret); a is getBlock_ret
        beq else1
            ;if(getBlock_ret-1==PosY)goto ret1;触底
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
    
    clc;not Touch
    rts
    ret1:
    sec;is Touch
    rts
.endproc

.proc _getBoardBase: near; 会改变Y
    lda #$13;19-getBase_y
    sec
    sbc getBase_y
    sta getBase_y
    lda #DRAW_BOARD
    ;sta getBase_item
    jsr _getBase
    rts
.endproc

.proc _DrawLine: near
    ;getBase_y=DrawLine_y;
    ;lda DrawLine_y
    stx getBase_y
    ;getBase_x=0;
    lda #$00
    sta getBase_x
    jsr _getBoardBase;会改变Y

    ;for(l=0;l<10;l++)
    ldy #$09;10 times
    fori1:
        sty setBoard_x
        ;setBoard_x=l;
        ;setBoard_y=DrawLine_y;
        lda DrawLine_y
        sta setBoard_y
        jsr _getBoard
        ;DrawBuff[2+l]=setBoard_val;
        ;lda setBoard_val); a is setBoard_val
        pha
    dey
    bpl fori1
    
    jsr _waitvblank;绘图PPU前调用 ;Y is 0
    lda getBase_hi
    sta (PTR2006),Y
    lda getBase_lo
    sta (PTR2006),Y
    
    ;for(i=2;i<12;i++)
    ldx #$09
    fori2:
        ;*(char*)(0x2007)=DrawBuff[i];
        pla
        sta (PTR2007),Y
        dex
    bpl fori2
    jsr _st2005
    rts
.endproc

.proc _adjUpDown: near
  stx setBoard_y
  jsr _getBoard;in setBoard_val
  tax
  lda _chg,X
  rts
.endproc

.proc _TouchDo: near
    lda #$03
    sta getBlockV2_idx
    fori:
      jsr _getNowBlock

      lda getBlock_i
      clc
      adc PosX
      sta setBoard_x
      sta getBase_x
                
      lda PosY
      sec
      sbc getBlock_j
      sta setBoard_y
      sta getBase_y

      lda getBlock_ret
      pha
      
      sta setBoard_val
      jsr _setBoard

      jsr _getBoardBase;会改变Y
      
      lda getBase_lo
      pha
      
      lda getBase_hi
      pha
    dec getBlockV2_idx
    bpl fori
    
    ;idx=0;
    jsr _waitvblank;绘图PPU前调用 ;Y is 0
    ;for(TouchDo_i=4;TouchDo_i>0;TouchDo_i--)
    ldx #$03
    fori2:
        ;*(char*)(0x2006)=DrawBuff[idx++];
        pla
        sta (PTR2006),Y
        ;*(char*)(0x2006)=DrawBuff[idx++];
        pla
        sta (PTR2006),Y
        ;*(char*)(0x2007)=DrawBuff[idx++];
        pla
        sta (PTR2007),Y
    dex
    bpl fori2
    jsr _st2005

    ;clear line
    ;{
        ;for(TouchDo_j=19;TouchDo_j>=0;TouchDo_j--)
        lda #$13
        sta TouchDo_j
        forj3:
        
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
                ;if(setBoard_val==0);如果当前行中有空白，跳过，不需要做任何事情
                ;lda setBoard_val); a is setBoard_val
                beq continue;bne else2
            dec setBoard_x;TouchDo_i
            bpl fori3

            ;当前行满一行，加分
            jsr _DrawScore
            
            ;调整上下两行的样子
            ;for(TouchDo_k=0;TouchDo_k<9;TouchDo_k++)
            lda #$09;10 times
            sta setBoard_x;TouchDo_k 保证setBoard_x在_getBoard 中未被修改
            fork1:
                ;if(TouchDo_j-1>=0)
                ldx TouchDo_j
                dex
                bmi else3
                    ;setBoard_y=TouchDo_j-1;
                    jsr _adjUpDown
                    and #$0F
                    sta setBoard_val
                    jsr _setBoard
                else3:

                ;setBoard_y=TouchDo_j+1;因为一般不会刷最上面一行，所以不用判断TouchDo_j+1是否大于等于20
                ldx TouchDo_j
                inx
                jsr _adjUpDown
                lsr
                lsr
                lsr
                lsr
                sta setBoard_val
                jsr _setBoard
            dec setBoard_x;TouchDo_k
            bpl fork1
            
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
                    ;ldx TouchDo_k
                    dec setBoard_y
                    jsr _setBoard
                dec setBoard_x;TouchDo_i
                bpl fori4
                
                ;DrawLine_y=TouchDo_k;
                ldx TouchDo_k
                dex
                bmi else4
                    ;DrawLine_y=TouchDo_j-1;
                    stx DrawLine_y
                    jsr _DrawLine
                else4:
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

.proc _AnyTouch: near ; 返回：C
    ;if(PosX<0)
    lda PosX
    bmi ret1

    jsr _isRightTouch
    bpl ret1

    ;for(AnyTouch_j=4;AnyTouch_j>0;AnyTouch_j--)
    lda #$03
    sta getBlock_j;AnyTouch_j
    forj:

        jsr _getNowBottom
        ;if(getBlock_ret-1-PosY>0)  ->  (getBlock_ret-1-1>=PosY)
        tax;ldx getBlock_ret); a is getBlock_ret
        dex
        dex
        cpx PosY
        bpl ret1
        
    dec getBlock_j;AnyTouch_j
    bpl forj

    
    lda #$03
    sta getBlockV2_idx
    fori:
      jsr _getNowBlock

      lda getBlock_i
      clc
      adc PosX
      sta setBoard_x

      lda PosY
      sec
      sbc getBlock_j
      sta setBoard_y

      jsr _getBoard

      bne ret1
    dec getBlockV2_idx
    bpl fori

    clc;not AnyTouch
    rts
    ret1:
    sec
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
    
    jsr _isRightTouch;如果旋转后伸出右边界的话， 往里推
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

.proc _slowdown: near
    jsr _isTouch
    bcc else1
        jsr _TouchDo
        rts
    else1:
    dec PosY
    rts
.endproc

.proc _readJoystick: near
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

.proc _Clear4by4: near
    lda #$00
    sta getBase_x
    lda #$03
    sta getBase_y
    fori1:
        lda #DRAW_NEXT;sta getBase_item
        jsr _getBase
        jsr _waitvblank;绘图PPU前调用 ;Y is 0
        lda getBase_hi
        sta (PTR2006),Y
        lda getBase_lo
        sta (PTR2006),Y
        tya;lda #$00
        sta (PTR2007),Y
        sta (PTR2007),Y
        sta (PTR2007),Y
        sta (PTR2007),Y
        jsr _st2005
    dec getBase_y
    bpl fori1
    rts
.endproc

.proc _mainSub: near
    ;jsr _loadplayer
    ;{
        ldy currentPlayer
        ldx #$0F
        for_loadplayer:
            lda Player,Y
            sta CURRENT_PLAYER,X
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
            sta isOn;lda #$01
        else4:
        jmp _saveplayer
    else2:

    ;one second count
    ;if(TimeCount--==0)
    dec TimeCount
    bne else1
        ;TimeCount=50;
        lda #$32;50
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
        ;jsr _movel
        inc PosX
        jsr _AnyTouch
        bcc else3
            dec PosX
        else3:
    elseKey1:
    lsr key2;if(key2&0x02){movelr_n=-1;movelr();};button_LEFT
    bcc elseKey2
        ;jsr _mover
        dec PosX
        jsr _AnyTouch
        bcc else5
            inc PosX
        else5:
    elseKey2:
    lsr key2;if(key2&0x04)down();;button_DOWN
    bcc elseKey3
        ;jsr _down
        loop1:
            jsr _isTouch
            dec PosY
        bcc loop1
        inc PosY
        jsr _TouchDo
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
    jsr _DrawShapeNowAndNext
    ;lastkey = key1;
    lda key1
    sta lastkey
    _saveplayer:
    ;jsr _saveplayer
    ;{
        ldy currentPlayer
        ldx #$0F
        for_saveplayer:
            lda CURRENT_PLAYER,X
            sta Player,Y
            dey
        dex
        bpl for_saveplayer
    ;}
    rts
.endproc

.proc _initTitle: near
    ;*(char*)(0x2006)=0x20;
    lda #$20
    sta (PTR2006),Y
    ;*(char*)(0x2006)=0xC5;
    txa
    sta (PTR2006),Y
    ldx #POS_TITLE ;17
    loop1:
      stx $2007
      inx
    cpx #(POS_TITLE+5)
    bne loop1
    rts
.endproc

.proc _setPalette: near
    ldy #$3F
    sty $2006

    sta $2006
    ldy #$00
    lda #$2C
    sta (PTR2007),Y
    ;*(char*)(0x2007)=0x30;
    lda #$30
    sta (PTR2007),Y
    ;*(char*)(0x2007)=0x00;
    txa
    sta (PTR2007),Y
    ;*(char*)(0x2007)=0x3F;
    lda #$3F
    sta (PTR2007),Y
    rts
.endproc

.proc _main: near
    ;init ptr
    ;{
        lda #$20
        ldy #$00
        ldx #$00;PTR2000
        fori:
            sty PTR2000,X
            iny
            inx
            sta PTR2000,X
            inx
        cpx #$10 ;PTR2007+1
        bne fori
    ;}
    
    ;open nmi
    lda #%10000000;#% 二进制
    sta (PTR2000),Y;Y is 0
    ;set color
    ;{
    ;main_temp
        
        jsr _waitvblank;Y is 0
        lda #$00
        ldx #$00
        ;$3F00   | $10   |       | Image Palette
        jsr _setPalette
        
        lda #$10
        ldx #$22
        ;$3F10   | $10   |       | Sprite Palette
        jsr _setPalette
    ;}
    ;set background black
    ;{
        jsr _waitvblank;Y is 0
        ;*(char*)(0x2006)=0x20;
        lda #$20
        sta (PTR2006),Y
        ;*(char*)(0x2006)=0x20;
        sta (PTR2006),Y
        
        ;total 0x0380(32*28=896)
        ldy #$03;lda #$03
        ;sta main_j
        ldx #$80;lda #$80
        ;sta main_i
        for4:
            lda #POS_BLACK
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
        ldx #$C5
        jsr _initTitle

        ldx #$D5
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
            ;jsr _Player1
            ;currentPlayer = 0;
            lda #$00
            sta currentPlayer
            ;readJoystick_player = 0;
            sta readJoystick_player
            ;CurSP_player=0;
            sta CurSP_player
            ;CurBoard_player=0;
            sta CurBoard_player
            jsr _mainSub

            ;jsr _Player2
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
            jsr _mainSub
        jmp while1
    ;}
    rts
.endproc



