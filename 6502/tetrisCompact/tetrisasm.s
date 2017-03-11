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
    .byte $70,$34,$38,$6C,$50,$21,$22,$43,$70,$34,$38,$6C,$50,$21,$22,$43;1 长条
    .byte $B0,$91,$84,$A5,$B0,$91,$84,$A5,$B0,$91,$84,$A5,$B0,$91,$84,$A5;2 方
    .byte $71,$B4,$A5,$68,$50,$91,$85,$46,$71,$B4,$A5,$68,$50,$91,$85,$46;3 Z
    .byte $70,$84,$95,$69,$B1,$42,$54,$A5,$70,$84,$95,$69,$B1,$42,$54,$A5;4 反Z
    .byte $70,$34,$88,$49,$72,$54,$25,$A6,$50,$91,$35,$69,$B0,$21,$42,$64;5 反L
    .byte $B0,$41,$34,$68,$70,$84,$25,$46,$71,$35,$58,$A9,$50,$21,$92,$66;6 L
    .byte $70,$E4,$45,$68,$71,$54,$F5,$46,$71,$54,$C5,$69,$50,$D1,$42,$65;7 T
_chg:
    .byte $00,$11,$45,$33,$41,$15,$66,$77,$68,$97,$A6,$7B,$C3,$9B,$3E,$A8 ; chg
_dataUser:
;          DRAW_SCORE,   DRAW_BOARD,   DRAW_NEXT,   DRAW_SP,   DRAW_NEXT_SP,  
    .byte     $07,$2D,      $02,$22,     $02,$2D,   $17,$10,        $17,$68,  $00,$00,$00,$00,$00,$00 ;6 player1
    .byte     $10,$2E,      $02,$33,     $0B,$2E,   $17,$98,        $5F,$70,  $00,$00,$00,$00,$00,$00 ;7 player2

.define DRAW_SCORE    $00
.define DRAW_BOARD    $02
.define DRAW_NEXT     $04
.define DRAW_SP       $06
.define DRAW_NEXT_SP  $08

.define TOP           $00
.define LEFT          $01

.define POS_BLACK     $10 ;16
.define POS_TITLE     $11 ;17
.define POS_ZERO      $16 ;22

;don't forget "lda  ($08,X)"   "lda  ($00),y"  zero page transfer

.define PTR2000             $00
.define PTR2001             $02
.define PTR2002             $04
.define PTR2003             $06
.define PTR2004             $08
.define PTR2005             $0A
.define PTR2006             $0C
.define PTR2007             $0E

;page0 0x0000 ~0x00FF
.define getBlock_idx        $10
.define getBoard_x          $11;getBase_x getBlock_i
.define getBoard_y          $12;getBase_y getBlock_j
.define setBoard_val        $13
.define getBase_hi          $14
.define getBase_lo          $15
.define DrawLine_y          $16
.define TouchDo_j           $17
.define TouchDo_k           $18
.define TouchDo_temp        $19
.define rotate_n            $1A
.define currentPlayer       $1B;(currentPlayer+_dataUser   currentPlayer+Player)
.define readJoystick_player $1C
.define CurSP_player        $1D
.define rand7               $1E

;current player
.define CURRENT_PLAYER      $20
.define PosX                $20
.define PosY                $21
.define NextShapeNo         $22
.define NowShapeNo          $23
.define NowDirectionNo      $24
.define lastkey             $25
.define key                 $26
.define Score0              $27
.define Score1              $28
.define Score2              $29
.define Score3              $2A
.define TimeCount           $2B
.define isOn                $2C

.define PTRBoard            $30


;page1: 0x0100 ~0x01FF stack

;page2: 0x0200 ~0x02FF
;size:16*2(0x020)  player  data 1,2
.define Player           $0200
;size:16*2*2(0x040) now and next SPBuff
.define DrawBuff         $0300
;size:100*2(0x0C8) player board 1
.define Board1           $0400
;size:100*2(0x0C8) player board 2
.define Board2           $0500

.segment    "CODE"

.proc _nmi: near
  ;rts
.endproc

.proc _waitvblank: near
  ldy #$00

  dec rand7
  bpl lbl2;<0
    lda #$06
    sta rand7
  lbl2:

  lbl1:
  lda (PTR2002),Y
  bpl lbl1

  rts
.endproc

.proc _waitvblank2: near
  jsr _waitvblank;绘图PPU前调用 ;Y is 0
  lda getBase_hi
  sta (PTR2006),Y
  lda getBase_lo
  sta (PTR2006),Y
  rts
.endproc

.proc _getBlock: near ;返回 ： getBlock_j getBlock_i  setBoard_val(A)
  asl
  asl
  ora getBlock_idx
  tay
  lda _block,Y
  pha
  and #$03
  sta getBoard_y;getBlock_j;(0000 00XX)
  pla
  lsr
  lsr
  pha
  and #$03
  sta getBoard_x;getBlock_i;(0000 XX00)
  pla
  lsr
  lsr
  sta setBoard_val;getBlock_ret;(xxxx 0000)
  rts
.endproc

.proc _getNowBlock: near
    ;char idx = (NowShapeNo<<4)+(NowDirectionNo<<2)+(getBlock_idx<<1);
    lda NowShapeNo
    asl
    asl
    ora NowDirectionNo
    jsr _getBlock
    rts
.endproc

.proc _calcBoardAddress: near;x:0~9 y:0~19 输出：Y
    ;setBoard_base = (getBoard_y*10) + (getBoard_x) = ((getBoard_y*2)+getBoard_y)*2 + (getBoard_x)
    lda getBoard_y
    asl
    asl
    adc getBoard_y
    asl
    adc getBoard_x
    tay
    rts
.endproc

.proc _setBoard: near
    jsr _calcBoardAddress
    lda setBoard_val
    sta (PTRBoard),Y
    rts
.endproc

.proc _getBoard: near ; A is setBoard_val
    jsr _calcBoardAddress
    lda (PTRBoard),Y
    sta setBoard_val
    rts
.endproc

.proc _getBase: near; 会改变Y,入参：A:getBase_item , currentPlayer , getBoard_y , getBoard_x    返回：getBase_lo , A:getBase_hi
    ;A:getBase_item
    ora currentPlayer;player 0x00,0x10
    tax
    ;getBase_hi=(getBoard_y+TOP)>>3
    lda getBoard_y
    clc
    adc _dataUser+TOP,X;(_dataUser+TOP+currentPlayer)
    tay
    lsr;>>
    lsr
    lsr
    pha;(high)
    tya
    asl;<<
    asl
    asl
    asl
    asl
    ;(clc)
    ora getBoard_x
    clc
    adc _dataUser+LEFT,X;(_dataUser+LEFT+currentPlayer)
    sta getBase_lo
    pla
    adc #$20;(高位基址+$20+进位)
    sta getBase_hi
    rts
.endproc

.proc _getBoardBase: near; 会改变Y 返回：getBase_lo , A:getBase_hi
    lda #19;19-getBoard_y
    sec
    sbc getBoard_y
    sta getBoard_y
    lda #DRAW_BOARD
    ;sta getBase_item
    jsr _getBase
    rts
.endproc

.proc _getSPBaseAndSetDrawBuff: near;入参A ：getSPBase_item
    ora currentPlayer;getSPBase_item
    tay
    
    ;top = ((getBoard_y)<<3+(_dataUser+TOP,X))
    lda getBoard_y
    asl
    asl
    asl
    ;(clc)
    adc _dataUser+TOP,Y;(_dataUser+TOP+currentPlayer)
    sta DrawBuff,X
    inx
    
    ;n=getBlock_ret;
    lda setBoard_val;getBlock_ret
    sta DrawBuff,X
    inx

    ;00;
    lda #$00
    sta DrawBuff,X
    inx
    
    ;left=(DrawShape_i-1)<<3
    lda getBoard_x
    asl
    asl
    asl
    ;(clc)
    adc _dataUser+LEFT,Y;(_dataUser+LEFT+currentPlayer)
    sta DrawBuff,X
    inx
    
    rts
.endproc

.proc _st2005: near
  ;[0x2005] = 0;
  ;lda #$00
  tya; y=0
  sta (PTR2005),Y
  ;[0x2005] = 0;
  sta (PTR2005),Y
  rts
.endproc

.proc _DrawScore: near
    ldx #3
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
    
    lda #0
    sta getBoard_x
    sta getBoard_y
    ;(lda #DRAW_SCORE) DRAW_SCORE = 0
    ;sta getBase_item
    jsr _getBase
    
    jsr _waitvblank2;绘图PPU前调用 ;Y is 0
    ;lda getBase_hi
    ;sta (PTR2006),Y
    ;lda getBase_lo
    ;sta (PTR2006),Y

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

.proc _DrawShapeNowAndNext: near
  ldx CurSP_player

  lda #3
  sta getBlock_idx
  fori:
    lda NextShapeNo;get next block
    asl
    asl
    jsr _getBlock
    
    ;lda getBoard_x;getBlock_i
    ;sta getBoard_x

    ;lda getBoard_y;getBlock_j
    ;sta getBoard_y

    lda #DRAW_NEXT_SP;sta getSPBase_item
    jsr _getSPBaseAndSetDrawBuff

    jsr _getNowBlock

    ;getBoard_x = getBlock_i + PosX
    lda getBoard_x;getBlock_i
    clc
    adc PosX
    sta getBoard_x

    ;getBoard_y = getBlock_j - PosY + 19
    lda getBoard_y;getBlock_j
    sec
    sbc PosY
    clc
    adc #19
    sta getBoard_y

    lda #DRAW_SP;sta getSPBase_item
    jsr _getSPBaseAndSetDrawBuff
  dec getBlock_idx
  bpl fori

  jsr _waitvblank;绘图PPU前调用 ;Y is 0
  lda #((DrawBuff >> 8) & $FF)
  sta $4014;Sprite DMA Register
  rts
.endproc

.proc _DrawLine: near
    lda DrawLine_y
    sta getBoard_y

    ;for(l=0;l<10;l++)
    ldx #$09;10 times
    fori1:
        stx getBoard_x
        ;getBoard_x=l;
        ;getBoard_y=DrawLine_y;
        jsr _getBoard
        ;lda setBoard_val); a is setBoard_val
        pha
    dex
    bpl fori1
    
    ;getBoard_y=DrawLine_y;
    ;lda DrawLine_y
    ;(lda DrawLine_y)
    ;(sta getBoard_y);getBoard_y未改变
    ;getBoard_x=0;
    ;(lda #$00)
    ;(sta getBoard_x);getBoard_x = 0
    jsr _getBoardBase;会改变Y
    jsr _waitvblank2;绘图PPU前调用 ;Y is 0
    ;lda getBase_hi
    ;sta (PTR2006),Y
    ;lda getBase_lo
    ;sta (PTR2006),Y
    
    ;for(i=0;i<9;i++)
    ldx #$09
    fori2:
        pla
        sta (PTR2007),Y
        dex
    bpl fori2
    jsr _st2005
    rts
.endproc

.proc _Clear: near
    ;for(i=0;i<100;i++)
    ldx #$FF;99 times
    lda #$00
    tay;ldy 00
    fori:
        sta (PTRBoard),Y
        iny
    dex
    bne fori

    lda #19;20 times
    sta DrawLine_y
    fori2:
        jsr _DrawLine
    dec DrawLine_y
    bpl fori2
    
    ldx #(9 + POS_ZERO)
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
    lda #19
    sta PosY
    ;PosX = 3;
    lda #3
    sta PosX

    rts
.endproc

.proc _isTouch: near ;返回 C
  lda #$03
  sta getBlock_idx
  fori:
    jsr _getNowBlock

    lda getBoard_x;getBlock_i
    clc
    adc PosX
    bmi ret1
    cmp #10
    bpl ret1
    sta getBoard_x
    
    lda PosY
    ;if (y < 0 || y > 19 || x < 0 || x > 9) {
    sec
    sbc getBoard_y;getBlock_j
    bmi ret1
    cmp #20
    bpl ret1
    sta getBoard_y
    

    ;if (this.private_getBoard(x, y) != 0) {
    jsr _getBoard
    bne ret1;A is setBoard_val
  dec getBlock_idx
  bpl fori
  
  clc;not Touch
  rts
  ret1:
  sec;is Touch
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

.proc _adjUpDown: near
  rol TouchDo_temp;Store C
  stx getBoard_y
  jsr _getBoard;in setBoard_val
  tax; A is setBoard_val
  lda _chg,X
  ror TouchDo_temp
  jsr _split
  sta setBoard_val
  jsr _setBoard
  rts
.endproc

.proc _TouchDo: near
  lda #$03
  sta getBlock_idx
  fori:
    jsr _getNowBlock
    
    ;(lda getBlock_ret)
    pha
    ;sta setBoard_val A is setBoard_val

    lda getBoard_x;getBlock_i
    clc
    adc PosX
    sta getBoard_x

    lda PosY
    sec
    sbc getBoard_y;getBlock_j
    sta getBoard_y

    jsr _setBoard

    jsr _getBoardBase;会改变Y 返回：getBase_lo , A:getBase_hi
    
    lda getBase_lo
    pha
    
    lda getBase_hi
    pha
  dec getBlock_idx
  bpl fori
  
  ;idx=0;
  jsr _waitvblank;绘图PPU前调用 ;Y is 0
  ;for(TouchDo_i=4;TouchDo_i>0;TouchDo_i--)
  ldx #3
  fori2:
      pla
      sta (PTR2006),Y
      pla
      sta (PTR2006),Y
      pla
      sta (PTR2007),Y
  dex
  bpl fori2
  jsr _st2005

  ;clear line
  ;{
      ;for(TouchDo_j=19;TouchDo_j>=0;TouchDo_j--)
      lda #19
      sta TouchDo_j
      forj3:
      
          ;for(TouchDo_i=9;TouchDo_i>=0;TouchDo_i--)
          lda #9
          sta getBoard_x;TouchDo_i 保证getBoard_x在_getBoard 中未被修改
          fori3:
              ldx TouchDo_j
              stx getBoard_y
              jsr _getBoard
              ;if(setBoard_val==0);如果当前行中有空白，跳过，不需要做任何事情
              ;lda setBoard_val); a is setBoard_val
              beq continue;bne else2
          dec getBoard_x;TouchDo_i
          bpl fori3

          ;当前行满一行，加分
          jsr _DrawScore
          
          ;调整上下两行的样子
          ;for(TouchDo_k=0;TouchDo_k<9;TouchDo_k++)
          lda #9;10 times
          sta getBoard_x;TouchDo_k 保证getBoard_x在_getBoard 中未被修改
          fork1:
              ;if(TouchDo_j-1>=0)
              ldx TouchDo_j
              dex
              bmi else3
                  ;getBoard_y=TouchDo_j-1;
                  clc
                  jsr _adjUpDown
              else3:

              ;getBoard_y=TouchDo_j+1;因为一般不会刷最上面一行，所以不用判断TouchDo_j+1是否大于等于20
              ldx TouchDo_j
              inx
              sec
              jsr _adjUpDown
          dec getBoard_x;TouchDo_k
          bpl fork1
          
          ;for(TouchDo_k=TouchDo_j;TouchDo_k<19;TouchDo_k++)
          lda TouchDo_j
          sta TouchDo_k
          fork2:
              ;for(TouchDo_i=9;TouchDo_i>=0;TouchDo_i--)
              lda #9
              sta getBoard_x;TouchDo_i
              fori4:
                  ;getBoard_y=TouchDo_k+1;
                  ldx TouchDo_k
                  inx
                  stx getBoard_y
                  jsr _getBoard

                  ;getBoard_y=TouchDo_k;
                  ;ldx TouchDo_k
                  dec getBoard_y
                  jsr _setBoard
              dec getBoard_x;TouchDo_i
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
          cmp #19
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
  sta getBoard_x;TouchDo_i
  fori5:
      ;getBoard_x=TouchDo_i-1;
      ;ldx TouchDo_i
      ;stx getBoard_x
      ;getBoard_y=18;
      ldx #18
      stx getBoard_y
      jsr _getBoard

      ;if(setBoard_val)
      ;lda setBoard_val); a is setBoard_val
      beq else6
          jsr _Clear
          jmp break2
      else6:
  dec getBoard_x;TouchDo_i
  bpl fori5
  break2:
  
  jsr _NextShape
  rts
.endproc

.proc _rotate: near
    ;NowDirectionNo=(NowDirectionNo+rotate_n)&3;
    lda NowDirectionNo
    pha
    clc
    adc rotate_n
    and #$03
    sta NowDirectionNo
    
    jsr _isTouch;如果旋转后有碰的话， 往里推
    bcc else1 ;有碰
      dec PosX
      jsr _isTouch
      bcc else1
        dec PosX
        jsr _isTouch
        bcc else1
          dec PosX
          jsr _isTouch
          bcc else1
            inc PosX
            inc PosX
            inc PosX
            pla
            sta NowDirectionNo
            rts
    else1:;无碰
    pla
    rts
.endproc

.proc _slowdown: near
    dec PosY
    jsr _isTouch
    bcc else1
      inc PosY
      jsr _TouchDo
      rts
    else1:
    rts
.endproc

.proc _readJoystick: near
    ldx readJoystick_player

    ;*(char*)(0x4016/7)=01;
    lda #$01
    sta key;key1=1;  1<<8
    sta $4016,X
    ;*(char*)(0x4016/7)=00;
    lda #$00
    sta $4016,X
    
    fori:
        lda $4016,X
        ror
        rol key
    bcc fori;key1=1;  1<<8
    lda key
    rts
.endproc

.proc _Clear4by4: near
    lda #$00
    sta getBoard_x
    lda #$03
    sta getBoard_y
    fori1:
        lda #DRAW_NEXT;sta getBase_item
        jsr _getBase
        jsr _waitvblank2;绘图PPU前调用 ;Y is 0
        ;lda getBase_hi
        ;sta (PTR2006),Y
        ;lda getBase_lo
        ;sta (PTR2006),Y
        tya;lda #$00
        sta (PTR2007),Y
        sta (PTR2007),Y
        sta (PTR2007),Y
        sta (PTR2007),Y
        jsr _st2005
    dec getBoard_y
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
            iny
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
            jsr _Clear4by4
            jsr _Clear
            jsr _NextShape
            jsr _NextShape
            lda #25
            sta TimeCount
            sta isOn;lda #$01
        else4:
        jsr _waitvblank
        jmp _saveplayer
    else2:

    ;one second count
    ;if(TimeCount--==0)
    dec TimeCount
    bne else1
        ;TimeCount=50;
        lda #25
        sta TimeCount
        jsr _slowdown
    else1:
    
    jsr _readJoystick
    pha
    ;key2 = key1 ^ lastkey;
    ;lda key1); a is key1
    eor lastkey
    ;key2 = key2 & key1;
    and key;取得有变化并且被按下的键，就是刚被按下的瞬间
    ;if(key2!=0)
    sta key
    ;lastkey = key1;
    pla
    sta lastkey
    lsr key;if(key2&0x01){movelr_n=1;movelr();};button_RIGHT
    bcc elseKey1
        ;jsr _movel
        inc PosX
        jsr _isTouch
        bcc else3
            dec PosX
        else3:
    elseKey1:
    lsr key;if(key2&0x02){movelr_n=-1;movelr();};button_LEFT
    bcc elseKey2
        ;jsr _mover
        dec PosX
        jsr _isTouch
        bcc else5
            inc PosX
        else5:
    elseKey2:
    lsr key;if(key2&0x04)down();;button_DOWN
    bcc elseKey3
        ;jsr _down
        loop1:
            dec PosY
            jsr _isTouch
        bcc loop1
        inc PosY
        jsr _TouchDo
    elseKey3:
    lsr key;if(key2&0x08)slowdown();;button_UP
    bcc elseKey4
        jsr _slowdown
    elseKey4:
    lsr key;if(key2&0x10);;button_START
    lsr key;if(key2&0x20);;button_SELECT
    lsr key;if(key2&0x40){rotate_n=-1;rotate();};button_B
    bcc elseKey5
        lda #$FF
        sta rotate_n
        jsr _rotate
    elseKey5:
    lsr key;if(key2&0x80){rotate_n=1;rotate();};button_A
    bcc elseKey6
        lda #$01
        sta rotate_n
        jsr _rotate
    elseKey6:
    jsr _DrawShapeNowAndNext
    _saveplayer:
    ;jsr _saveplayer
    ;{
        ldy currentPlayer
        ldx #$0F
        for_saveplayer:
            lda CURRENT_PLAYER,X
            sta Player,Y
            iny
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
    ;lda #%10000000;#% 二进制
    ;sta (PTR2000),Y;Y is 0
    ;set color
    ;{
        jsr _waitvblank;Y is 0
        tya;lda #$00
        tax;ldx #$00
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
        lda #(Board1 & $FF)
        ;lda #(Board2 & $FF)
        sta PTRBoard
        
        while1:
            ;jsr _Player1
            ;currentPlayer = 0;
            lda #$00
            sta currentPlayer
            ;readJoystick_player = 0;
            sta readJoystick_player
            ;CurSP_player=0;
            sta CurSP_player

            lda #((Board1 >> 8) & $FF)
            sta PTRBoard+1
            jsr _mainSub

            ;jsr _Player2
            ;currentPlayer = 0x10;
            lda #$10
            sta currentPlayer
            ;CurSP_player=0x20;
            asl;(lda #$20)
            sta CurSP_player
            ;readJoystick_player = 1;
            lda #$01
            sta readJoystick_player
            
            lda #((Board2 >> 8) & $FF)
            sta PTRBoard+1
            jsr _mainSub
        jmp while1
    ;}
.endproc



