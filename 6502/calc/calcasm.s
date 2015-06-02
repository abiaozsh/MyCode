    .fopt        compiler,"cc65 v 2.13.2"
    .setcpu        "6502"
    .smart        on
    .case        on
    .debuginfo    off
    .macpack    longbranch
    .forceimport    __STARTUP__
    .export        _nmi
    .export        _main

.include "calcchr.inc"

;256B -> 5num
;128B -> 10num
;64B -> 20num  << 154 (154digit) 77
;32B -> 40num  << 77   (77digit) 39
;16B -> 80num  << 38.5 (39digit) 20


                                   ;16B 32B 64B
.define NUMSIZE             $20    ;$10 $20 $40
.define SIGN_EXP            $1F    ;$0F $1F $3F
.define PRECISION           $1F    ;$0F $1F $3F
.define DIGITS              $27    ;$14 $27 $4D

.define nmiflg              $01
.define debug               $02
.define key1                $0A
.define mulByte_lo          $0B
.define mulByte_hi          $0C
.define mulByte_a           $0D
.define mulByte_tmp_lo      $0E
.define mulByte_tmp_hi      $0F
.define divByte_lo          mulByte_lo    
.define divByte_hi          mulByte_hi    
.define divByte_a           mulByte_a     
.define divByte_tmp_lo      mulByte_tmp_lo
.define divByte_tmp_hi      mulByte_tmp_hi

;0
;1
;2
;3
;4
;5
;6
;7
;8
;9
;A
;B
;C
;D
;E
;F


.define getBase_x           $10
.define getBase_y           $11
.define getBase_lo          $12
.define getBase_hi          $13
.define getBase_tmp         $14
.define print_byte          $15
.define print_byte2         $16
.define print_lo            $17
.define print_hi            $18
.define print_addlo         $19
.define print_addhi         $1A
.define copy_fromLo         $1B
.define copy_fromHi         $1C
.define copy_toLo           $1D
.define copy_toHi           $1E
.define showNum_addrLo      $1F
.define showNum_addrHi      $20
.define showNum_temp        $21
.define showNum_i           $22
.define showNum_i2          $23
.define showNum_exitF       $24
.define showNum_exp         $25
.define getSignExp_num_lo   $26
.define getSignExp_num_hi   $27
.define getSignExp_sign     $28
.define getSignExp_exp      $29

.define addNum_num1_lo      $30
.define addNum_num1_hi      $31
.define addNum_sign1        $32
.define addNum_exp1         $33
.define addNum_num2_lo      $34
.define addNum_num2_hi      $35
.define addNum_sign2        $36
.define addNum_exp2         $37
.define addNum_p1lo         $38
.define addNum_p1hi         $39
.define addNum_p2lo         $3A
.define addNum_p2hi         $3B
.define addNum_disExp       $3C
.define addNum_cnt          $3D
.define addNum_carry        $3E

.define mulNum_num1_lo      addNum_num1_lo
.define mulNum_num1_hi      addNum_num1_hi
.define mulNum_sign1        addNum_sign1
.define mulNum_exp1         addNum_exp1
.define mulNum_num2_lo      addNum_num2_lo
.define mulNum_num2_hi      addNum_num2_hi
.define mulNum_sign2        addNum_sign2
.define mulNum_exp2         addNum_exp2

.define mulNum_shiftedlo    addNum_p1lo
.define mulNum_shiftedhi    addNum_p1hi
.define mulNum_buff_lo      addNum_p2lo
.define mulNum_buff_hi      addNum_p2hi
.define mulNum_shift        addNum_disExp
.define mulNum_cnt          addNum_cnt
.define mulNum_carry        addNum_carry
.define mulNum_n            $40
.define mulNum_idxMain      $41
.define mulNum_cutoff       $42
.define mulNum_tempexp      $43

.define divNum_num1_lo		addNum_num1_lo
.define divNum_num1_hi      addNum_num1_hi
.define divNum_sign1		addNum_sign1
.define divNum_exp1			addNum_exp1
.define divNum_num2_lo		addNum_num2_lo
.define divNum_num2_hi      addNum_num2_hi
.define divNum_sign2		addNum_sign2
.define divNum_exp2			addNum_exp2

.define divNum_p1lo         mulNum_shiftedlo
.define divNum_p1hi         mulNum_shiftedhi
.define divNum_buffR_lo     mulNum_buff_lo
.define divNum_buffR_hi     mulNum_buff_hi
.define divNum_cutoff       mulNum_shift
.define divNum_len    		mulNum_cnt
.define divNum_shift        mulNum_carry
.define divNum_shift2       mulNum_n
.define divNum_c       		mulNum_idxMain
.define divNum_d       		mulNum_cutoff
.define divNum_i			mulNum_tempexp
.define divNum_q    		$44
.define divNum_ebx 			$45
.define divNum_buffB_lo     $46
.define divNum_buffB_hi     $47

.define sinNum_num_lo		$50
.define sinNum_num_hi       $51
.define sinNum_sign         $52
.define sinNum_exp          $53
.define sinNum_idx          $54

.define main_i              $90
.define main_j              $91
.define main_1              $92
.define main_2              $93
.define main_3              $94
.define main_4              $95


;page1 stack

;page2 display buff/calc buff
;size = 192
.define displaybuff      $0200
;size = 64
.define calcbuff         $02C0

.define mulTempBuff      $0200
;	unsigned int* r = new unsigned int[Precision + 1];
.define divTempBuffR     $0200
;	unsigned int* b = new unsigned int[Precision];
.define divTempBuffB     $0280

;page3 sin
.define sin_num          $0300
.define sin_num2         $0320
.define sin_i            $0340
.define sin_a            $0360
;page4 sin
.define sin_b            $0380
.define sin_x2           $03A0
.define sin_1            $03C0

;page5~page7 mem
.define numn1            $03E0
.define numn2            $0400


;extra memory
;RAM: start = $6000, size = $2000, define = yes;



;asl Shift Left One Bit (Memory or Accumulator)
; C <- [76543210] <- 0
; N Z C I D V 
; + + + - - - 

;lsr Shift One Bit Right (Memory or Accumulator)
; 0 -> [76543210] -> C
; N Z C I D V
; - + + - - - 

;rol Rotate One Bit Left (Memory or Accumulator)
; C <- [76543210] <- C
; N Z C I D V
; + + + - - - 

;ror Rotate One Bit Right (Memory or Accumulator)
; C -> [76543210] -> C
; N Z C I D V
; + + + - - - 


.segment "RODATA"
rom1:
.byte $01,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00   ;0
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$01   ;0
romnum3:
.byte $03,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00   ;0
.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$01   ;0

romnum_pi64:
.byte $03,$24,$3F,$6A,$88,$85,$A3,$08,$D3,$13,$19,$8A,$2E,$03,$70,$73
.byte $44,$A4,$09,$38,$22,$29,$9F,$31,$D0,$08,$2E,$FA,$98,$EC,$4E,$6C
.byte $89,$45,$28,$21,$E6,$38,$D0,$13,$77,$BE,$54,$66,$CF,$34,$E9,$0C
.byte $6C,$C0,$AC,$29,$B7,$C9,$7C,$50,$DD,$3F,$84,$D5,$B5,$B5,$47,$01

.segment "CODE"

.include "calc.inc"

.proc sinNum: near;(unsigned int* Num_Array, int Precision, int* pSign, int* pExponent)
	lda #>rom1
	sta copy_fromHi
	lda #<rom1
	sta copy_fromLo
	lda #>sin_1
	sta copy_toHi
	lda #<sin_1
	sta copy_toLo
	jsr copy
    ;TODO regular by pi

    ;unsigned int* num = clone(Precision, Num_Array);
    ;int Snum = (*pSign);
    ;int Enum = (*pExponent);
	lda sinNum_num_hi
	sta copy_fromHi
	lda sinNum_num_lo
	sta copy_fromLo
	lda #>sin_num
	sta copy_toHi
	lda #<sin_num
	sta copy_toLo
	jsr copy

    ;unsigned int* num_i = newArray(Precision, 1);
    ;int Snum_i = 1;
    ;int Enum_i = 1;
	lda #>sin_i
	sta copy_toHi
	lda #<sin_i
	sta copy_toLo
	jsr init1
	
    ;unsigned int* num_a = clone(Precision, Num_Array);
    ;int Snum_a = (*pSign);
    ;int Enum_a = (*pExponent);
	lda sinNum_num_hi
	sta copy_fromHi
	lda sinNum_num_lo
	sta copy_fromLo
	lda #>sin_a
	sta copy_toHi
	lda #<sin_a
	sta copy_toLo
	jsr copy

    ;unsigned int* num_b = newArray(Precision, 1);
    ;int Snum_b = 1;
    ;int Enum_b = 1;
	lda #>sin_b
	sta copy_toHi
	lda #<sin_b
	sta copy_toLo
	jsr init1

    ;unsigned int* num_x2 = clone(Precision, Num_Array);
    ;int Snum_x2 = (*pSign);
    ;int Enum_x2 = (*pExponent);
	lda sinNum_num_hi
	sta copy_fromHi
	lda sinNum_num_lo
	sta copy_fromLo
	lda #>sin_x2
	sta copy_toHi
	lda #<sin_x2
	sta copy_toLo
	jsr copy

    ;unsigned int* num2 = newArray(Precision, 1);
    ;int Snum2 = 1;
    ;int Enum2 = 1;
	lda #>sin_num2
	sta copy_toHi
	lda #<sin_num2
	sta copy_toLo
	jsr init1

	;int limit = Precision;

	;x2=x*x
	;mul(limit, num_x2, &Snum_x2, &Enum_x2, num, Snum, Enum, b);
	lda #>sin_x2 
	sta mulNum_num1_hi
	lda #<sin_x2
	sta mulNum_num1_lo
	lda #>sin_num
	sta mulNum_num2_hi
	lda #<sin_num
	sta mulNum_num2_lo
	jsr mulNum

	lda #$01
	sta sinNum_sign
	loop:
    ;for (int i = 1 ; ; i++)
		;//a=a*x2
		;mul(limit, num_a, &Snum_a, &Enum_a, num_x2, Snum_x2, Enum_x2, b);
		lda #>sin_a
		sta mulNum_num1_hi
		lda #<sin_a
		sta mulNum_num1_lo
		lda #>sin_x2
		sta mulNum_num2_hi
		lda #<sin_x2
		sta mulNum_num2_lo
		jsr mulNum

		;//b=b*(i)*(i+1)
		;//i++
		;incr(num_i, &Enum_i);
		lda #>sin_i
		sta addNum_num1_hi
		lda #<sin_i
		sta addNum_num1_lo
		lda #>sin_1
		sta addNum_num2_hi
		lda #<sin_1
		sta addNum_num2_lo
		jsr addNum

		;//b=b*i
		;mul(limit, num_b, &Snum_b, &Enum_b, num_i, Snum_i, Enum_i, b);
		lda #>sin_b
		sta mulNum_num1_hi
		lda #<sin_b
		sta mulNum_num1_lo
		lda #>sin_i
		sta mulNum_num2_hi
		lda #<sin_i
		sta mulNum_num2_lo
		jsr mulNum

		;//i++
		;incr(num_i, &Enum_i);
		lda #>sin_i
		sta addNum_num1_hi
		lda #<sin_i
		sta addNum_num1_lo
		lda #>sin_1
		sta addNum_num2_hi
		lda #<sin_1
		sta addNum_num2_lo
		jsr addNum

		;//b=b*i
		;mul(limit, num_b, &Snum_b, &Enum_b, num_i, Snum_i, Enum_i, b);
		lda #>sin_b
		sta mulNum_num1_hi
		lda #<sin_b
		sta mulNum_num1_lo
		lda #>sin_i
		sta mulNum_num2_hi
		lda #<sin_i
		sta mulNum_num2_lo
		jsr mulNum

		;for(int idx = 0; idx < Precision; idx++)
		;{
		;    num2[idx] = num_a[idx];
		;}
		;Snum2 = Snum_a;
		;Enum2 = Enum_a;
		lda #>sin_a
		sta copy_fromHi
		lda #<sin_a
		sta copy_fromLo
		lda #>sin_num2
		sta copy_toHi
		lda #<sin_num2
		sta copy_toLo
		jsr copy

		;//num2=a/b
		;divi(limit, num2, &Snum2, &Enum2, num_b, Snum_b, Enum_b, r_value, b);
		lda #>sin_num2
		sta divNum_num1_hi
		lda #<sin_num2
		sta divNum_num1_lo
		lda #>sin_b
		sta divNum_num2_hi
		lda #<sin_b
		sta divNum_num2_lo
		jsr divNum
        ;//result+=num2
        ;if (i % 2 == 1)
        ;{
        ;    Snum2 = -Snum2;
        ;}

		lda sinNum_sign
		beq endif
			lda #>sin_num2
			sta getSignExp_num_hi
			lda #<sin_num2
			sta getSignExp_num_lo

			ldy #SIGN_EXP
			lda (getSignExp_num_lo),Y
			eor #$80
			sta (getSignExp_num_lo),Y
		endif:

        ;add(Precision, Num_Array, pSign, pExponent, num2, Snum2, Enum2);
		lda sinNum_num_hi
		sta addNum_num1_hi
		lda sinNum_num_lo
		sta addNum_num1_lo
		lda #>sin_num2
		sta addNum_num2_hi
		lda #<sin_num2
		sta addNum_num2_lo
		jsr addNum

        ;if ((*pExponent) - Enum2 > (Precision - 2))
        ;{
        ;    break;
        ;}
		lda sinNum_num_hi
		sta getSignExp_num_hi
		lda sinNum_num_lo
		sta getSignExp_num_lo
		jsr getSignExp
		lda getSignExp_exp
		sta sinNum_exp
		
		lda #>sin_num2
		sta getSignExp_num_hi
		lda #<sin_num2
		sta getSignExp_num_lo
		jsr getSignExp
		lda getSignExp_exp

		lda sinNum_exp
		sec
		sbc getSignExp_exp
		cmp #PRECISION - 1
		bpl exit

        ;limit = Precision - (*pExponent - Enum2);
        ;if(limit > Precision) limit = Precision;
        ;if(limit <= 0) limit = 1;

		lda sinNum_sign
		eor #$01
		sta sinNum_sign
	jmp loop
	exit:
	rts
.endproc




.proc _main: near
	lda #$00
	sta nmiflg
	sta debug
	
	lda #%00000000 ; no nmi
	sta $2000
	lda #%00001010
	sta $2001
	jsr waitvblank
    ;set color
    ;{
        lda #$3F
        sta $2006
        lda #$00
        sta $2006

		lda #$30
        sta $2007
        lda #$2C
        sta $2007
        lda #$00
        sta $2007
        lda #$3F
        sta $2007
    ;}

	jsr initscr
	jsr newline

	;;;;;;;;;;;;;;;start;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	lda #>romnum3
	sta copy_fromHi
	lda #<romnum3
	sta copy_fromLo
	lda #>numn1
	sta copy_toHi
	lda #<numn1
	sta copy_toLo
	jsr copy

	loop1:
	lda #>numn1
	sta copy_fromHi
	lda #<numn1
	sta copy_fromLo
	lda #>numn2
	sta copy_toHi
	lda #<numn2
	sta copy_toLo
	jsr copy
	
	lda #>numn2
	sta sinNum_num_hi
	lda #<numn2
	sta sinNum_num_lo
	jsr sinNum

	lda #>numn1
	sta addNum_num1_hi
	lda #<numn1
	sta addNum_num1_lo
	lda #>numn2
	sta addNum_num2_hi
	lda #<numn2
	sta addNum_num2_lo
	jsr addNum
	
	;show
	lda #>numn1
	sta showNum_addrHi
	lda #<numn1
	sta showNum_addrLo
	jsr showNum
	jsr newline
	jsr showNum16
	jsr newline

	jmp loop1



	
	while:jmp while

	
	rts
.endproc

