
.proc _nmi: near; _nmi _nmi _nmi _nmi _nmi _nmi _nmi _nmi _nmi _nmi _nmi _nmi _nmi _nmi _nmi _nmi _nmi _nmi _nmi
	rts
.endproc

.proc waitvblank: near; waitvblank waitvblank waitvblank waitvblank waitvblank waitvblank waitvblank waitvblank
    lda $2002
    bmi else1
        lbl1:
        lda $2002
        bmi lbl1
        lbl2:
        lda $2002
        bpl lbl2
        rts
    else1:
    lda $2002
    bpl else1
	rts
.endproc

.proc readJoystick:near
    ;*(char*)(0x4016/7)=01;
    lda #$01
    sta $4016
    ;*(char*)(0x4016/7)=00;
    lda #$00
    sta $4016
    ;key1=0;
    sta key1

    ldy #$08
    fori:
        ;key1=(key1<<1)|*(char*)(0x4016/7)&1;
        lda $4016
        and #$01
        asl key1
        ora key1
        sta key1
    dey
    bne fori
    lda key1
	
    rts
.endproc

.proc initscr:near
	lda #$20
	sta print_addlo
	lda #$20
	sta print_addhi
	rts
.endproc

.proc newline:near
	lda print_addlo
	and #$E0
	clc
	adc #$20
	sta print_addlo
	lda print_addhi
	adc #$00
	sta print_addhi
	
	lda print_addhi
	cmp #$23
	bne skip
	lda print_addlo
	cmp #$A0
	bne skip
		lda #$20
		sta print_addlo
		lda #$20
		sta print_addhi
	skip:

	jsr waitvblank
	lda print_addhi
	sta $2006
	lda print_addlo
	sta $2006
	
	ldx #$40
	lda #$00
	loop:
		sta $2007
	dex
	bne loop
	
	rts
.endproc

.proc space:near
	lda #$FF
	sta print_byte2
	jsr print
	rts
.endproc

;print_byte2
.proc print:near;print print print print print print print print print print print print print print print
	jsr waitvblank
	lda print_addhi
	sta $2006
	lda print_addlo
	sta $2006

	lda print_byte2
	sta $2007
    lda #$00
    sta $2005
    lda #$00
    sta $2005

	inc print_addlo
	bne addhi
		inc print_addhi
	addhi:

	lda print_addhi
	cmp #$23
	bne skip
	lda print_addlo
	cmp #$A0
	bne skip
		lda #$20
		sta print_addlo
		lda #$20
		sta print_addhi
	skip:

	lda print_addlo
	and #$1F
	bne skip2
		jsr waitvblank
		lda print_addhi
		sta $2006
		lda print_addlo
		sta $2006
		
		lda #$40
		sta print_byte2
		lda #$00
		loop:
			sta $2007
		dec print_byte2
		bne loop
	skip2:
	
	rts
.endproc


.proc print16:near;print print print print print print print print print print print print print print print 
	; get low
	lda print_byte
	and #$0F
	sta print_lo
	inc print_lo

	; get high
	lda print_byte
	lsr
	lsr
	lsr
	lsr
	sta print_hi
	inc print_hi

	lda print_hi
	sta print_byte2
	jsr print
	lda print_lo
	sta print_byte2
	jsr print
	
	rts
.endproc

.proc print10:near;print print print print print print print print print print print print print print print 
	; get high
	lda print_byte
	clc
	adc #$42
	sta print_byte2
	jsr print
	
	rts
.endproc

.proc mulByte:near;mulByte mulByte mulByte mulByte mulByte mulByte mulByte mulByte mulByte mulByte mulByte 
	;init
	lda mulByte_lo
	sta mulByte_tmp_lo

	lda #$00
	sta mulByte_hi
	sta mulByte_lo
	sta mulByte_tmp_hi
	
	ldx #$08
	loop:
		lsr mulByte_a
		bcc carry0
			lda mulByte_lo
			clc
			adc mulByte_tmp_lo
			sta mulByte_lo
			
			lda mulByte_hi
			adc mulByte_tmp_hi
			sta mulByte_hi
		carry0:
		asl mulByte_tmp_lo
		rol mulByte_tmp_hi
	dex
	bne loop
	rts
.endproc

.proc divByte:near ;divByte divByte divByte divByte divByte divByte divByte divByte divByte divByte divByte 
	;init
	lda divByte_a
	beq overflow
	sta divByte_tmp_hi
	lda #$00
	sta divByte_tmp_lo
	sta divByte_a

	ldx #$08
	loop:
		;shift>>
		lsr divByte_tmp_hi
		ror divByte_tmp_lo
		
		;try sub
		lda divByte_hi
		cmp divByte_tmp_hi
		bne next
			lda divByte_lo
			cmp divByte_tmp_lo
		next:
		bcc nosub ;jb jump below(cf==1)    < 
			;set carry
			rol divByte_a
			;sub
			lda divByte_lo
			sec
			sbc divByte_tmp_lo
			sta divByte_lo
			lda divByte_hi
			sbc divByte_tmp_hi
			sta divByte_hi
			jmp continue
		nosub:
		asl divByte_a
		continue:
	dex
	bne loop

	lda divByte_hi
	bne overflow
		lda divByte_lo
		sta divByte_hi
		lda divByte_a
		sta divByte_lo
		rts
	overflow:
	lda #$FF
	sta divByte_a
	rts
.endproc


.proc copy: near
	ldy #NUMSIZE
	loop1:
	dey
		lda (copy_fromLo),Y
		sta (copy_toLo),Y
	iny
	dey
	bne loop1
	rts
.endproc

.proc init1: near
	ldy #NUMSIZE - 2
	loop1:
		lda #$00
		sta (copy_toLo),Y
	dey
	bne loop1
	lda #$01
	sta (copy_toLo),Y
	ldy #NUMSIZE - 1
	sta (copy_toLo),Y
	rts
.endproc

.proc	showNum16: near
	lda #$00
	sta showNum_i
	loop1:
		ldy showNum_i
		lda (showNum_addrLo),Y
		sta print_byte
		jsr print16
	inc showNum_i
	lda showNum_i
	cmp #NUMSIZE
	bne loop1
	
	rts
.endproc

.proc	showNum: near	;showNum showNum showNum showNum showNum showNum showNum showNum showNum showNum 
	;show sign show sign show sign show sign show sign
	ldy #SIGN_EXP
	lda (showNum_addrLo),Y
	and #$80
	bne else1
		lda #$FD;+
		sta print_byte
		jsr print10
	jmp endif1
	else1:
		lda #$FE;-
		sta print_byte
		jsr print10
	endif1:
	;/show sign show sign show sign show sign show sign

	ldy #SIGN_EXP
	;exp
	lda (showNum_addrLo),Y
	and #$7F
	asl
	asl
	bcs if2cs
	beq if2eq
	jmp else2
	if2cs:
		;<1; >-1
		sec
		ror ;>>
		sec
		ror ;>>
		sta showNum_exp
		jmp endif2
	if2eq:
		;<1; >-1
		lsr ;>>
		lsr ;>>
		sta showNum_exp
		jmp endif2
	else2:
		lsr
		lsr
		sta showNum_exp ;save exp
		; >=1; <=-1
		
		;copy data to calcbuff
		lda showNum_exp
		sta showNum_i
		lda #$00
		sta showNum_i2
		loop1:
			ldy showNum_i
			dey
			lda (showNum_addrLo),Y
			
			ldy showNum_i2
			sta calcbuff,Y
			iny
			sty showNum_i2
		dec showNum_i
		bne loop1
		;/copy data to calcbuff

		;div to displaybuff
		lda #$00
		sta showNum_i2
		
		start:
			lda #$00
			sta divByte_hi
			sta showNum_exitF
			
			lda showNum_exp
			sta showNum_i
			loop2:
				ldy showNum_i
				dey
				lda calcbuff,Y
				sta divByte_lo
				
				lda #$64
				sta divByte_a
				
				jsr divByte
				
				ldy showNum_i
				dey
				lda divByte_lo
				sta calcbuff,Y
				; is zero
				ora showNum_exitF
				sta showNum_exitF
			dec showNum_i
			bne loop2
			
			;lda divByte_hi
			;sta print_byte
			;jsr print10
			ldy showNum_i2
			lda divByte_hi
			sta displaybuff,Y
			iny
			sty showNum_i2
			
			lda showNum_exitF
			;lda showNum_zeroflg
		bne start
		;/div to displaybuff
		
		;show displaybuff
		loop4:
			ldy showNum_i2
			dey
			lda displaybuff,Y
			sta print_byte
			jsr print10
		dec showNum_i2
		bne loop4
		
	endif2:


	
	lda #$FF;.
	sta print_byte
	jsr print10

	;set calcbuff
	lda #$00
	sta showNum_i2
	loop5:
	lda showNum_exp
	beq exit5
	bpl exit5
		lda #$00
		ldy showNum_i2
		sta calcbuff,Y
		inc showNum_i2
		inc showNum_exp
	jmp loop5
	exit5:
	
	lda showNum_exp
	sta showNum_i

	loop6:
		lda showNum_i2
		cmp #PRECISION
		beq exit6

		lda showNum_i
		cmp #PRECISION
		beq exit6

		ldy showNum_i
		lda (showNum_addrLo),Y
		
		ldy showNum_i2
		sta calcbuff,Y
		
		inc showNum_i
		inc showNum_i2
	jmp loop6
	exit6:
	
	;start mul
	
	lda #DIGITS
	sta showNum_i2
	start2:
	
		lda #$00
		sta showNum_temp ;carry
	
		lda #PRECISION
		sta showNum_i
		loopprint:
			ldy showNum_i
			dey
			lda calcbuff,Y
			
			sta mulByte_lo
			
			lda #$64
			sta mulByte_a
			jsr mulByte
			
			lda mulByte_lo
			clc
			adc showNum_temp
			sta mulByte_lo
			ldy showNum_i
			dey
			sta calcbuff,Y
			
			lda mulByte_hi
			adc #$00
			sta mulByte_hi
			sta showNum_temp
			
		dec showNum_i
		bne loopprint
	
	
		lda showNum_temp
		sta print_byte
		jsr print10
	
	dec showNum_i2
	bne start2

	rts
.endproc

.proc getSignExp:near
	ldy #SIGN_EXP
	;getSign 1
	lda (getSignExp_num_lo),Y
	and #$80
	sta getSignExp_sign

	;getExp 1 (*pExponent)
	ldy #SIGN_EXP
	;exp
	lda (getSignExp_num_lo),Y
	and #$7F
	asl
	asl
	bcs ifcs
	beq ifeq
	jmp elseExp
	ifcs:
		;<1; >-1
		sec
		ror ;>>
		sec
		ror ;>>
		sta getSignExp_exp
		rts
	ifeq:
		;<1; >-1
		lsr ;>>
		lsr ;>>
		sta getSignExp_exp
		rts
	elseExp:
		lsr
		lsr
		sta getSignExp_exp ;save exp
		; >=1; <=-1
		rts
.endproc


.proc setSignExp:near
	lda addNum_num1_lo
	sta getSignExp_num_lo
	lda addNum_num1_hi
	sta getSignExp_num_hi
	lda addNum_sign1 ;(*pSign)
	sta getSignExp_sign
	lda addNum_exp1 ;(*pExponent)
	sta getSignExp_exp

	ldy #SIGN_EXP
	lda getSignExp_exp
	and #$7F
	ora getSignExp_sign
	sta (getSignExp_num_lo),Y
	
	rts
.endproc

.proc getFexp:near
	;int f_exp = 0;
	lda #$00
	sta addNum_carry

	;for (int i = 0; i < Precision; i++)
	ldy #$00
	;{
	loop:
		;if (Num_Array[i] != 0)
		lda (addNum_num1_lo),Y
		beq else
		;{
			sty addNum_carry;f_exp = i;
			jmp exit;break;
		;}
		else:
	;}
	iny
	cpy #PRECISION
	bne loop
	exit:
	
	rts
.endproc


.proc slide:near
	;Num_Array[f_exp];
	lda addNum_num1_hi
	sta addNum_p1hi
	lda addNum_num1_lo
	clc
	adc addNum_carry
	sta addNum_p1lo
	
	;f_exp = Precision - f_exp
	lda #PRECISION
	sec
	sbc addNum_carry
	sta addNum_carry
	
	;for (int i = 0; i < Precision - f_exp; i++)
	ldy #$00
	;{
	loop1:
		;Num_Array[i] = Num_Array[i + f_exp];
		lda (addNum_p1lo),Y
		sta (addNum_num1_lo),Y
	;}
	iny
	cpy addNum_carry
	bne loop1
	
	;for (int i = Precision - f_exp; i < Precision; i++)
	ldy addNum_carry
	;{
	loop2:
		;Num_Array[i] = 0;
		lda #$00
		sta (addNum_num1_lo),Y
	;}
	iny
	cpy #PRECISION
	bne loop2
	
	rts
.endproc


.proc addA:near;int addA(unsigned int* Num_Array, unsigned int* num_Num_Array, int cnt, int disExp)
	;int carry = 0;
	lda #$00
	sta addNum_carry
	
	;//叠合部分加法
	;if (cnt>0)
	lda addNum_cnt
	beq else1
	bmi else1
	;{
		;unsigned int* p1 = &Num_Array[disExp - 1];
		lda addNum_num1_hi
		sta addNum_p1hi
		
		lda addNum_num1_lo
		clc
		adc addNum_disExp
		sta addNum_p1lo
		;unsigned int* p2 = &num_Num_Array[-1];

		;//位数
		ldy addNum_cnt;mov ecx,cnt
		;//被加数
		;mov edi,p1
		;//加数
		;mov esi,p2
		clc;clc
		loop1: ;lp:
			dey
			;//载入被加数
			lda (addNum_p1lo),Y;mov eax,[edi+ecx*4]
			;//被加数+加数+进位标志
			adc (addNum_num2_lo),Y;adc eax,[esi+ecx*4]
			;//保存结果
			sta (addNum_p1lo),Y;mov [edi+ecx*4],eax
			iny
		dey;dec ecx
		bne loop1;jnz lp
		;//传出进位标志
		lda #$00;mov edx,0
		adc #$00;adc edx,0
		sta addNum_carry;mov carry,edx
	;}
	else1:

	;//继续向上进位
	;if (disExp>0)
	lda addNum_disExp
	beq else2
	bmi else2
	;{
		;unsigned int* p1 = &Num_Array[-1];
		;lda addNum_num1_hi
		;sta addNum_p1hi
		;lda addNum_num1_lo
		;sta addNum_p1lo

		ldy addNum_disExp;mov ecx,disExp
		;mov edi,p1
		;//载入进位标志
		;mov edx,carry
		clc ;clc
		loop2: ;lpb:
			dey
			;//载入被加数
			lda (addNum_num1_lo),Y;mov eax,[edi+ecx*4]
			;//加进位标志
			adc addNum_carry;adc eax,edx
			;//保存结果
			sta (addNum_num1_lo),Y;mov [edi+ecx*4],eax
			bcc exit;jnc ext
			iny
		dey;dec ecx
		bne loop2;jnz lpb
		exit: ;ext:
		lda #$00;mov edx,0
		adc #$00;adc edx,0
		sta addNum_carry;mov carry,edx
	;}
	else2:
	
	;//全部结束后仍有进位
	;if (carry == 1)
	lda addNum_carry
	beq else3
	;{
		;//全体向后移一位
		;(*pExponent)++;
		inc addNum_exp1
		jsr setSignExp
		
		;for (int i = Precision - 1; i > 0; i--)
		ldy #PRECISION
		dey
		;{
		loop3:
			;Num_Array[i] = Num_Array[i - 1];
			dey
			lda (addNum_num1_lo),Y
			iny
			sta (addNum_num1_lo),Y
		;}
		dey
		bne loop3
		
		;Num_Array[0] = 1;
		ldy #$00
		lda #$01
		sta (addNum_num1_lo),Y
	;}
	else3:
	
    rts
.endproc

.proc addB:near;int addB(unsigned int* Num_Array, unsigned int* num_Num_Array, int cnt, int disExp)
	;int carry = 0;
	lda #$00
	sta addNum_carry

	;//叠合部分加法
	;if (cnt>0)
	lda addNum_cnt
	beq else1
	bmi else1
	;{
		;unsigned int* p1 = &Num_Array[-1];

		;unsigned int* p2 = &num_Num_Array[disExp - 1];
		lda addNum_num2_hi
		sta addNum_p1hi
		
		lda addNum_num2_lo
		clc
		adc addNum_disExp
		sta addNum_p1lo

		;unsigned int* p3 = &Num_Array[disExp - 1];
		lda addNum_num1_hi
		sta addNum_p2hi
		
		lda addNum_num1_lo
		clc
		adc addNum_disExp
		sta addNum_p2lo

		ldy addNum_cnt;mov ecx,cnt
		;//被加数高部
		;mov edi,p1
		;//加数
		;mov esi,p2
		;//被加数低部，原数据被废弃
		;mov ebx,p3
		;//清除进位标志
		clc;clc
		loop1:;lp:
			dey
			;//载入被加数高部
			lda (addNum_num1_lo),Y;mov eax,[edi+ecx*4]
			;//被加数+加数+进位标志
			adc (addNum_p1lo),Y;adc eax,[esi+ecx*4]
			;//保存到被加数低部
			sta (addNum_p2lo),Y;mov [ebx+ecx*4],eax
			iny
		dey;dec ecx
		bne loop1;jnz lp
		;//传出进位标志
		lda #$00;mov edx,0
		adc #$00;adc edx,0
		sta addNum_carry;mov carry,edx
	;}
	else1:

	;//继续向上进位
	;if (disExp>0)
	lda addNum_disExp
	beq else2
	bmi else2
	;{
		;unsigned int* p1 = &Num_Array[-1];
		;unsigned int* p2 = &num_Num_Array[-1];

		;{
			ldy addNum_disExp;mov ecx,disExp
			;mov edi,p1
			;mov esi,p2
			lda addNum_carry;mov edx,carry
			lsr;add edx,0xFFFFFFFF
			loop2:;lpb:
				dey
				lda (addNum_num2_lo),Y;mov eax,[esi+ecx*4]
				adc #$00;adc eax,0
				sta (addNum_num1_lo),Y;mov [edi+ecx*4],eax
				iny
			dey;dec ecx
			bne loop2;jnz lpb
			;//传出进位标志
			lda #$00;mov edx,0
			adc #$00;adc edx,0
			sta addNum_carry;mov carry,edx
		;}
	;}
	else2:
	;return carry;

	;if (carry == 1)
	lda addNum_carry
	beq else3
	;{
		;*pExponent = num_Exponent + 1;
		ldx addNum_exp2
		inx
		stx addNum_exp1
		jsr setSignExp

		;for (int i = Precision - 1; i > 0; i--)
		ldy #PRECISION
		dey
		;{
		loop3:
			;Num_Array[i] = Num_Array[i - 1];
			dey
			lda (addNum_num1_lo),Y
			iny
			sta (addNum_num1_lo),Y
		;}
		dey
		bne loop3
		
		;Num_Array[0] = 1;
		ldy #$00
		lda #$01
		sta (addNum_num1_lo),Y
	;}
	jmp endif3
	else3:;else
	;{
		;*pExponent = num_Exponent;
		ldx addNum_exp2
		stx addNum_exp1
		jsr setSignExp
	;}
	endif3:
	
	rts
.endproc




;//高幂减低幂 100 - 10
.proc subA:near;void subA(unsigned int* Num_Array, unsigned int* num_Num_Array, int cnt, int disExp)
	;int carry = 0;
	lda #$01
	sta addNum_carry

	;if (cnt>0)
	lda addNum_cnt
	beq else1
	bmi else1
	;{
		;unsigned int* p1 = &Num_Array[disExp - 1];
		lda addNum_num1_hi
		sta addNum_p1hi
		
		lda addNum_num1_lo
		clc
		adc addNum_disExp
		sta addNum_p1lo
		
		;unsigned int* p2 = &num_Num_Array[-1];

		ldy addNum_cnt;mov ecx,cnt
		;mov edi,p1
		;mov esi,p2
		sec
		loop1:;lp:
			dey
			lda (addNum_p1lo),Y;mov eax,[edi+ecx*4]
			sbc (addNum_num2_lo),Y;sbb eax,[esi+ecx*4]
			sta (addNum_p1lo),Y;mov [edi+ecx*4],eax
			iny
		dey;dec ecx
		bne loop1;jnz lp
		lda #$00;mov edx,0
		adc #$00;adc edx,0
		sta addNum_carry;mov carry,edx
	;}
	else1:

	;if (disExp>0 && carry == 1)
	lda addNum_disExp
	beq else2
	bmi else2
	lda addNum_carry
	bne else2
	;{

		;unsigned int* p1 = &Num_Array[-1];
		ldy addNum_disExp;mov ecx,disExp
		;mov edi,p1
		loop2:;lpb:
			dey
			lda (addNum_num1_lo),Y;mov eax,[edi+ecx*4]
			sec
			sbc #$01;sub eax,1
			sta (addNum_num1_lo),Y;mov [edi+ecx*4],eax
			bcs exit;jnc ext
			iny
		dey;dec ecx
		bne loop2;jnz lpb
		exit:;ext:
	;}
	else2:
	
	jsr getFexp
	
	;if (f_exp != 0)
	lda addNum_carry
	beq else4
	;{
		;*pExponent -= f_exp;
		lda addNum_exp1
		sec
		sbc addNum_carry
		sta addNum_exp1
		jsr setSignExp

		jsr slide
	;}
	else4:
	
	rts
.endproc

;//10 - 100
.proc subB:near;void subB(unsigned int* Num_Array, unsigned int* num_Num_Array, int cnt, int disExp)
	;int carry = 0;
	lda #$01
	sta addNum_carry
	
	;if (cnt>0)
	lda addNum_cnt
	beq else1
	bmi else1
	;{
		;unsigned int* p1 = &Num_Array[-1];
		;unsigned int* p2 = &num_Num_Array[disExp - 1];
		lda addNum_num2_hi
		sta addNum_p1hi
		
		lda addNum_num2_lo
		clc
		adc addNum_disExp
		sta addNum_p1lo
		;unsigned int* p3 = &Num_Array[disExp - 1];
		lda addNum_num1_hi
		sta addNum_p2hi
		
		lda addNum_num1_lo
		clc
		adc addNum_disExp
		sta addNum_p2lo

		ldy addNum_cnt;mov ecx,cnt
		;mov edi,p1
		;mov esi,p2
		;mov ebx,p3
		sec;clc
		loop1:;lp:
			dey
			lda (addNum_p1lo),Y;mov eax,[esi+ecx*4]
			sbc (addNum_num1_lo),Y;sbb eax,[edi+ecx*4]
			sta (addNum_p2lo),Y;mov [ebx+ecx*4],eax
			iny
		dey;dec ecx
		bne loop1;jnz lp
		;//last one
		lda #$00;mov edx,0
		adc #$00;adc edx,0
		sta addNum_carry;mov carry,edx
	;}
	else1:

	;if (disExp>0)
	lda addNum_disExp
	beq else2
	bmi else2
	;{
		;unsigned int* p1 = &Num_Array[-1];
		;unsigned int* p2 = &num_Num_Array[-1];

		ldy addNum_disExp;mov ecx,disExp
		;mov edi,p1
		;mov esi,p2
		lda addNum_carry;mov edx,carry
		lsr;add edx,0xFFFFFFFF
		loop2:;lpb:
			dey
			lda (addNum_num2_lo),Y;mov eax,[esi+ecx*4]
			sbc #$00;sbb eax,0
			sta (addNum_num1_lo),Y;mov [edi+ecx*4],eax
			iny
		dey;dec ecx
		bne loop2;jnz lpb
	;}
	else2:

	jsr getFexp

	;if (f_exp != 0)
	lda addNum_carry
	beq else4
	;{
		;*pExponent = num_Exponent - f_exp;
		lda addNum_exp2
		sec
		sbc addNum_carry
		sta addNum_exp1
		jsr setSignExp

		jsr slide
	;}
	jmp endif4
	else4:
	;{
		;*pExponent = num_Exponent;
		lda addNum_exp2
		sta addNum_exp1
		jsr setSignExp
	;}
	endif4:
	
	rts
.endproc

;//99-88
.proc subC:near;void subC(unsigned int* Num_Array, unsigned int* num_Num_Array, int cnt)
	;unsigned int* p1 = Num_Array;
	;unsigned int* p2 = num_Num_Array;


	ldy addNum_cnt;mov ecx,cnt
	;mov edi,p1
	;mov esi,p2
	sec;clc
	loop1:;lp:
		dey
		lda (addNum_num1_lo),Y;mov eax,[edi+ecx*4]
		sbc (addNum_num2_lo),Y;sbb eax,[esi+ecx*4]
		sta (addNum_num1_lo),Y;mov [edi+ecx*4],eax
		iny
	dey;dec ecx
	bne loop1;jnz lp
	;//last one
	;mov eax,[edi]
	;sbb eax,[esi]
	;mov [edi],eax

	jsr getFexp

	;if (f_exp != 0)
	lda addNum_carry
	beq else4
	;{
		;*pExponent -= f_exp;
		lda addNum_exp1
		sec
		sbc addNum_carry
		sta addNum_exp1
		jsr setSignExp

		jsr slide
	;}
	else4:
	
	rts
.endproc

;//88-99
.proc subD:near;int subD(unsigned int* Num_Array, unsigned int* num_Num_Array, int cnt)
	;unsigned int* p1 = Num_Array;
	;unsigned int* p2 = num_Num_Array;

	ldy addNum_cnt;mov ecx,cnt
	;mov edi,p1
	;mov esi,p2
	sec;clc
	loop1:;lp:
		dey
		lda (addNum_num2_lo),Y;mov eax,[esi+ecx*4]
		sbc (addNum_num1_lo),Y;sbb eax,[edi+ecx*4]
		sta (addNum_num1_lo),Y;mov [edi+ecx*4],eax
		iny
	dey;dec ecx
	bne loop1;jnz lp
	;//last one
	;mov eax,[esi]
	;sbb eax,[edi]
	;mov [edi],eax
	
	jsr getFexp

	;if (f_exp != 0)
	lda addNum_carry
	beq else4
	;{
		;*pExponent -= f_exp;
		lda addNum_exp1
		sec
		sbc addNum_carry
		sta addNum_exp1
		jsr setSignExp
		
		jsr slide
	;}
	else4:
	;(*pSign) = -(*pSign);
	lda addNum_sign2
	sta addNum_sign1
	jsr setSignExp

	rts
.endproc




.proc addNum:near;addNumaddNumaddNumaddNumaddNumaddNumaddNumaddNumaddNumaddNumaddNumaddNumaddNumaddNumaddNumaddNum

	lda addNum_num1_lo
	sta getSignExp_num_lo
	lda addNum_num1_hi
	sta getSignExp_num_hi
	jsr getSignExp
	lda getSignExp_sign
	sta addNum_sign1 ;(*pSign)
	lda getSignExp_exp
	sta addNum_exp1 ;(*pExponent)
	

	lda addNum_num2_lo
	sta getSignExp_num_lo
	lda addNum_num2_hi
	sta getSignExp_num_hi
	jsr getSignExp
	lda getSignExp_sign
	sta addNum_sign2 ;num_Sign
	lda getSignExp_exp
	sta addNum_exp2 ;num_Exponent
	

	;//符号相等 加法
	;if ((*pSign) == num_Sign)
	lda addNum_sign1
	cmp addNum_sign2
	bne else1
	;{
		;if ((*pExponent) >= num_Exponent)
		lda addNum_exp1
		cmp addNum_exp2
		bmi else11
		;{
			;幂差
			;int disExp = *pExponent - num_Exponent;
			lda addNum_exp1
			sec
			sbc addNum_exp2
			sta addNum_disExp
			
			;低幂足够大，使加法有意义
			;if (Precision-disExp>0)
			lda #PRECISION
			cmp addNum_disExp
			bmi else111
			beq else111
			;{
				;//叠合部分
				;int cnt = Precision - disExp;
				lda #PRECISION
				sec
				sbc addNum_disExp
				sta addNum_cnt
				
				;//进位标志
				;int carry = addA(Num_Array, num_Num_Array, cnt, disExp);
				jsr addA
				
				rts
			;}
			else111:
			rts
		;}
		else11:
		;{
			;int disExp = num_Exponent - *pExponent;
			lda addNum_exp2
			sec
			sbc addNum_exp1
			sta addNum_disExp

			
			;两数偏差小于精度,加法有意义
			;if (Precision-disExp>0)
			lda #PRECISION
			cmp addNum_disExp
			bmi else112
			beq else112
			;{
				;int cnt = Precision - disExp;
				lda #PRECISION
				sec
				sbc addNum_disExp
				sta addNum_cnt

				;int carry = addB(Num_Array, num_Num_Array, cnt, disExp);
				jsr addB
				rts
			;}
			else112: ;加法无意义，两数阶差太大，将加数赋给被加数
			;{
				;for (int i = 0; i < Precision; i++)
				ldy #NUMSIZE
				dey
				;{
				loop3:
					;Num_Array[i] = num_Num_Array[i];
					dey
					lda (addNum_num2_lo),Y
					sta (addNum_num1_lo),Y
					iny
				;}
				dey
				bne loop3
				;*pExponent = num_Exponent;
				lda addNum_exp2
				sta addNum_exp1
				jsr setSignExp
			;}
		;}
		rts
	;}
	else1:;// 符号相反 减法
	;{
		;if ((*pExponent) > num_Exponent)
		lda addNum_exp1
		cmp addNum_exp2
		beq else2eq
		bmi else2mi
		;pl
		;{
			;int disExp = *pExponent - num_Exponent;
			lda addNum_exp1
			sec
			sbc addNum_exp2
			sta addNum_disExp

			;if (Precision-disExp>0)
			lda #PRECISION
			cmp addNum_disExp
			bmi else121
			beq else121
			;{
				;int cnt = Precision - disExp;
				lda #PRECISION
				sec
				sbc addNum_disExp
				sta addNum_cnt

				;subA(Num_Array, num_Num_Array, cnt, disExp);
				jsr subA
			;}
			else121:
			rts
		;}
		else2mi: ;else if ((*pExponent) < num_Exponent)
		;{
			;int disExp = num_Exponent - *pExponent;
			lda addNum_exp2
			sec
			sbc addNum_exp1
			sta addNum_disExp

			;if (Precision-disExp>0)
			lda #PRECISION
			cmp addNum_disExp
			bmi else122
			beq else122
			;{
				;int cnt = Precision - disExp;
				lda #PRECISION
				sec
				sbc addNum_disExp
				sta addNum_cnt

				;subB(Num_Array, num_Num_Array, cnt, disExp);
				jsr subB
			;}
			else122:
			;(*pSign) = -(*pSign);
			lda addNum_sign2
			sta addNum_sign1
			jsr setSignExp
			rts
		;}
		else2eq: ;else// (Exponent == num_Exponent)
		;{
			;int f_comp = 0;
			lda #$00
			sta addNum_carry
			
			;for (int i = 0; i < Precision; i++)
			ldy #$00
			;{
			loop1:
				;if (Num_Array[i] > num_Num_Array[i])
				lda (addNum_num1_lo),Y
				cmp (addNum_num2_lo),Y
				beq endif_eq
				bcs point_pl
				;{
					;f_comp = 1;
					;break;
					jmp point_mi
				;}
				;if (Num_Array[i] < num_Num_Array[i])
				;{
					;f_comp = -1;
					;break;
				;}
				endif_eq:
			;}
			iny
			cpy #PRECISION
			bne loop1
			jmp point_eq
			

			;if (f_comp == 1)
			point_pl:
			;{
				;int cnt = Precision;
				lda #PRECISION
				sta addNum_cnt
				;subC(Num_Array, num_Num_Array, cnt);
				jsr subC
				rts
			;}

			;else if (f_comp == -1)
			point_mi:
			;{
				;int cnt = Precision;
				lda #PRECISION
				sta addNum_cnt
				;subD(Num_Array, num_Num_Array, cnt);
				jsr subD
				rts
			;}

			;else if (f_comp == 0)
			point_eq:
			;{
				;(*pSign) = 1;
				;(*pExponent) = 0;
				;for (int i = 0; i < Precision; i++)
				;{
					;Num_Array[i] = 0;
				;}
				ldy #NUMSIZE
				lda #$00
				loop2:
					dey
					sta (addNum_num1_lo),Y
					iny
				dey
				bne loop2
				
			;}
		;}
	;}
	
	rts
.endproc


	
;//v1.31
.proc mulNum: near;void mul(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent, unsigned int* buf_Num_Array)
	lda #>mulTempBuff
	sta mulNum_buff_hi
	lda #<mulTempBuff
	sta mulNum_buff_lo

	lda mulNum_num1_lo
	sta getSignExp_num_lo
	lda mulNum_num1_hi
	sta getSignExp_num_hi
	jsr getSignExp
	lda getSignExp_sign
	sta mulNum_sign1 ;(*pSign)
	lda getSignExp_exp
	sta mulNum_exp1 ;(*pExponent)
	
	lda mulNum_num2_lo
	sta getSignExp_num_lo
	lda mulNum_num2_hi
	sta getSignExp_num_hi
	jsr getSignExp
	lda getSignExp_sign
	sta mulNum_sign2 ;num_Sign
	lda getSignExp_exp
	sta mulNum_exp2 ;num_Exponent
	
	;int cutOff = 0;
	lda #$00
	sta mulNum_cutoff

	;for (int i = 0; i < Precision; i++)
	ldy #$00
	;{
	loop1:
		;buf_Num_Array[i] = Num_Array[i];
		lda (mulNum_num1_lo),Y
		sta (mulNum_buff_lo),Y
		;if(Num_Array[i]!=0)
		;lda (mulNum_num1_lo),Y
		beq else0
		;{
			;cutOff = i;
			sty mulNum_cutoff
		;}
		else0:
		;Num_Array[i] = 0;
		lda #$00
		sta (mulNum_num1_lo),Y
	;}
	iny
	cpy #PRECISION
	bne loop1

	;cutOff++
	inc mulNum_cutoff
	
	;int temp_Exponent = (*pExponent) + num_Exponent;
	lda mulNum_exp1
	clc
	adc mulNum_exp2
	sta mulNum_tempexp
	
	;*pExponent = 0;
	lda #$00
	sta mulNum_exp1

	;*pSign = (*pSign) * num_Sign;
	lda mulNum_sign1
	eor mulNum_sign2
	sta mulNum_sign1
	
	
	;for (int idxMain = 0; idxMain < Precision; idxMain++)
	lda #$00
	sta mulNum_idxMain
	;{
	loop2:
	
		;int n = num_Num_Array[idxMain];
		ldy mulNum_idxMain
		lda (mulNum_num2_lo),Y
		sta mulNum_n

		;if (n!=0)
		lda mulNum_n
		bne if1
		jmp else1
		if1:
		;beq else1
		;{
			;int shift = idxMain + (*pExponent);//(*pExponent)修正量
			lda mulNum_idxMain
			clc
			adc mulNum_exp1
			sta mulNum_shift
			
			;int cnt = ((1 + cutOff) < (Precision - shift) ? (1 + cutOff) : (Precision - shift))
			lda #PRECISION
			sec
			sbc mulNum_shift
			cmp mulNum_cutoff
			bmi else2
				lda mulNum_cutoff
			else2:
			sta mulNum_cnt

			;int carry = 0;
			;if (cnt>0)
			lda mulNum_cnt
			beq else3
			bmi else3
			;{
				;unsigned int* p0 = &Num_Array[shift - 1];
				lda mulNum_num1_hi
				sta mulNum_shiftedhi
				
				lda mulNum_num1_lo
				clc
				adc mulNum_shift
				sta mulNum_shiftedlo

				;unsigned int* p1 = &buf_Num_Array[-1];
				;mulNum_buff_lo
				
				ldy mulNum_cnt;mov ecx,cnt
				;mov edi,p0
				;mov esi,p1
				;mov ebx,0
				lda #$00
				sta mulNum_carry
				loop3:;lp:
					dey
					;mov eax,n
					lda mulNum_n
					sta mulByte_lo
					;cdq
					;mul DWORD PTR [esi+ecx*4] // low:eax,high:edx
					lda (mulNum_buff_lo),Y
					sta mulByte_a
					
					jsr mulByte
					
					;add eax,[edi+ecx*4]
					clc
					lda mulByte_lo
					adc (mulNum_shiftedlo),Y
					sta mulByte_lo
					;adc edx,0
					lda mulByte_hi
					adc #$00
					sta mulByte_hi
					
					;add eax,ebx
					;clc
					;lda mulByte_lo
					;adc mulNum_carry
					;sta mulByte_lo
					clc
					lda mulByte_lo
					adc mulNum_carry
					sta (mulNum_shiftedlo),Y

					;adc edx,0
					;lda mulByte_hi
					;adc #$00
					;sta mulByte_hi
					lda mulByte_hi
					adc #$00
					sta mulNum_carry

					;mov [edi+ecx*4],eax
					;lda mulByte_lo
					;sta (mulNum_shiftedlo),Y
					
					;mov ebx,edx
					;lda mulByte_hi
					;sta mulNum_carry
					
					iny
				dey;dec ecx
				bne loop3;jnz lp
				;mov carry,ebx
			;}
			else3:

			;//继续进位
			;if (shift>0)
			lda mulNum_shift
			beq else4
			bmi else4
			;{
				;unsigned int* p0 = &Num_Array[-1];
				ldy mulNum_shift;mov ecx,shift
				;mov edi,p0
				;mov edx,carry
				loop4:;lp2:
					dey
					lda (mulNum_num1_lo),Y;mov eax,[edi+ecx*4]
					clc
					adc mulNum_carry;add eax,edx
					sta (mulNum_num1_lo),Y;mov [edi+ecx*4],eax
					;mov edx,0
					lda #$00
					sta mulNum_carry
					bcc exit;jnc ext
					;mov edx,1
					lda #$01
					sta mulNum_carry
					iny
				dey;dec ecx
				bne loop4;jnz lp2
				exit:;ext:
				;mov carry,edx
			;}
			else4:

			;if (carry!=0)
			lda mulNum_carry
			beq else5
			;{
				;(*pExponent)++;
				inc mulNum_exp1
				
				;for (int i = Precision - 1; i > 0; i--)
				ldy #PRECISION
				dey
				loop5:
				;{
					;Num_Array[i] = Num_Array[i - 1];
					dey
					lda (mulNum_num1_lo),Y
					iny
					sta (mulNum_num1_lo),Y
				;}
				dey
				bne loop5
				
				;Num_Array[0] = carry;
				ldy #$00
				lda mulNum_carry
				sta (mulNum_num1_lo),Y
			;}
			else5:
		;}
		else1:
	;}
	inc mulNum_idxMain
	lda mulNum_idxMain
	cmp #PRECISION
	;bne loop2
	beq exit2
	jmp loop2
	exit2:
	
	;(*pExponent) += temp_Exponent - 1;
	lda mulNum_exp1
	clc
	adc mulNum_tempexp
	sta mulNum_exp1
	dec mulNum_exp1
	jsr setSignExp

	rts
.endproc


.proc BitScanReverse: near
	cmp #$00
	beq exit
	ldy #$09
	loop:
	asl
	dey
	bcc loop
	tya
	rts
	exit:
	rts
.endproc

.proc divNumShift: near
	ldx divNum_shift
	loop:
	asl
	dex
	bne loop
	rts
.endproc

.proc divNumShift2: near
	ldx divNum_shift2
	loop:
	lsr
	dex
	bne loop
	rts
.endproc

.proc divNum: near;void div(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent, unsigned int* r, unsigned int* b)

	lda #>divTempBuffB
	sta divNum_buffB_hi
	lda #<divTempBuffB
	sta divNum_buffB_lo
	
	lda #>divTempBuffR
	sta divNum_buffR_hi
	lda #<divTempBuffR
	sta divNum_buffR_lo

	lda divNum_num1_lo
	sta getSignExp_num_lo
	lda divNum_num1_hi
	sta getSignExp_num_hi
	jsr getSignExp
	lda getSignExp_sign
	sta divNum_sign1 ;(*pSign)
	lda getSignExp_exp
	sta divNum_exp1 ;(*pExponent)
	
	lda divNum_num2_lo
	sta getSignExp_num_lo
	lda divNum_num2_hi
	sta getSignExp_num_hi
	jsr getSignExp
	lda getSignExp_sign
	sta divNum_sign2 ;num_Sign
	lda getSignExp_exp
	sta divNum_exp2 ;num_Exponent

	;if(num_Num_Array[0]==0)throw "divide by zero";
	;int cutOff = 0;

	;(*pExponent) = (*pExponent) - num_Exponent + 1;
	lda divNum_exp1
	sec
	sbc divNum_exp2
	sta divNum_exp1
	inc divNum_exp1

	;(*pSign) = (*pSign) * num_Sign;
	lda divNum_sign1
	eor divNum_sign2
	sta divNum_sign1
	
	;规范化 移位
	;int shift;
	;unsigned long shift2;
	;_BitScanReverse(&shift2, num_Num_Array[0]);
	ldy #$00
	lda (divNum_num2_lo),Y
	jsr BitScanReverse
	sta divNum_shift2

	;shift2++;
	;inc divNum_shift2 ; not needed
	
	;shift = 32 - shift2;
	lda #$08
	sec
	sbc divNum_shift2
	sta divNum_shift
	
	;if(shift!=0)
	;{
	lda divNum_shift
	beq else1
		;unsigned int c = num_Num_Array[0];
		ldy #$00
		lda (divNum_num2_lo),Y
		sta divNum_c

		;for (int i = 0; i < Precision - 1; i++)
		ldy #$00
		;{
		loop1:
		cpy #PRECISION - 1
		beq break1

			;unsigned int d = c;
			lda divNum_c
			sta divNum_d

			;c = num_Num_Array[i + 1];
			iny
			lda (divNum_num2_lo),Y
			dey
			sta divNum_c

			;b[i] = (d << shift) | (c >> shift2);
			lda divNum_d
			jsr divNumShift
			sta (divNum_buffB_lo),Y
			lda divNum_c
			jsr divNumShift2
			ora (divNum_buffB_lo),Y
			sta (divNum_buffB_lo),Y

			;if(b[i]!=0)
			;lda (divNum_buffB_lo),Y
			beq endif2
			;{
				;cutOff = i;
				sty divNum_cutoff
			;}
			endif2:
			
		iny
		;}
		jmp loop1
		break1:
		
		;b[Precision - 1] = num_Num_Array[Precision - 1] << shift;
		ldy #PRECISION - 1
		lda (divNum_num2_lo),Y
		jsr divNumShift
		sta (divNum_buffB_lo),Y
		
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

		;c = Num_Array[0];
		ldy #$00
		lda (divNum_num1_lo),Y
		sta divNum_c
		
		;r[0] = (c >> shift2);
		lda divNum_c
		jsr divNumShift2
		ldy #$00
		sta (divNum_buffR_lo),Y

		;for (int i = 0; i < Precision; i++)
		ldy #$00
		;{
		loop2:
		cpy #PRECISION
		beq break2
			iny
			;unsigned int d = c;
			lda divNum_c
			sta divNum_d

			;c = Num_Array[i + 1];
			lda (divNum_num1_lo),Y
			sta divNum_c

			;r[i + 1] = (d << shift) | (c >> shift2);
			lda divNum_d
			jsr divNumShift
			sta (divNum_buffR_lo),Y
			lda divNum_c
			jsr divNumShift2
			ora (divNum_buffR_lo),Y
			sta (divNum_buffR_lo),Y
		;}
		jmp loop2
		break2:
		
		;r[Precision] = Num_Array[Precision - 1] << shift;
		ldy #PRECISION - 1
		lda (divNum_num1_lo),Y
		jsr divNumShift
		iny
		sta (divNum_buffR_lo),Y
	;}
	jmp endif1
	else1:
	;{
		;r[0]=0;
		lda #$00
		;ldy #$00
		tay
		sta (divNum_buffR_lo),Y
		;for (int i = 0; i < Precision; i++)
		;ldy #$00
		;{
		loop3:
		cpy #PRECISION
		beq break3
			;b[i] = num_Num_Array[i];
			lda (divNum_num2_lo),Y
			sta (divNum_buffB_lo),Y

			;if(b[i]!=0)
			lda (divNum_buffB_lo),Y
			beq endif3
			;{
				;cutOff = i;
				sty divNum_cutoff
			;}
			endif3:

			;r[i+1] = Num_Array[i];
			lda (divNum_num1_lo),Y
			iny
			sta (divNum_buffR_lo),Y
		;dey
		;iny
		;}
		jmp loop3
		break3:
	;}
	endif1:

	;cutOff++;
	inc divNum_cutoff
	
	;运算
	;for (int idx_main = 0; idx_main < Precision; idx_main++)
	lda #$00
	sta divNum_i
	
	;{
	loop_main:
	ldy #PRECISION
	cpy divNum_i
	bne loop_mainX
	jmp break_main
	loop_mainX:
	;beq break_main

		
		;unsigned int q;//商
		
		;mov ecx,idx_main
		;mov ebx,r
		
		;mov edx,[ebx + ecx*4]//rh = r[idx_main]
		ldy divNum_i
		lda (divNum_buffR_lo),Y
		sta divByte_hi

		;mov eax,[ebx + ecx*4 + 4]//rl = r[idx_main + 1]
		;ldy divNum_i
		iny
		lda (divNum_buffR_lo),Y
		sta divByte_lo

		;mov ebx,b//b[0]
		ldy #$00
		lda (divNum_buffB_lo),Y
		sta divByte_a
		
;		;div DWORD PTR [ebx]
;		jsr divByte
;
;		;mov q,eax
;		lda divByte_lo
;		sta divNum_q
		
		cmp divByte_hi
		beq else6
			;div DWORD PTR [ebx]
			jsr divByte
			lda divByte_lo
			jmp endif6
		else6:
			lda #$FF
		endif6:
		sta divNum_q
		;mov q,eax

		;乘减
		;unsigned int* p1 = &r[idx_main];//idx_main + 1 - 1
		lda divNum_buffR_hi
		sta divNum_p1hi
		lda divNum_buffR_lo
		clc
		adc divNum_i
		sta divNum_p1lo
		
		;unsigned int* p2 = &b[-1];
		;lda divNum_buffB_hi
		;sta divNum_p2hi
		;ldy divNum_buffB_lo
		;dey
		;sty divNum_p2lo

		;int len = ((1 + cutOff) < (Precision - idx_main) ? (1 + cutOff) : (Precision - idx_main))
		lda #PRECISION
		sec
		sbc divNum_i
		cmp divNum_cutoff
		bmi else4
			lda divNum_cutoff
		else4:
		sta divNum_len

		
		;mov edi,p1
		;mov esi,p2
		;mov ecx,len
		;ldy divNum_len
		tay
		;mov ebx,0
		lda #$00
		sta divNum_ebx
		lp:
			;mov eax,q
			lda divNum_q
			sta mulByte_a
			;cdq
			;mul DWORD PTR [esi+ecx*4] //p2
			dey
			lda (divNum_buffB_lo),Y
			iny
			;sta mulByte_lo
			
			;jsr mulByte
			;{
				;init
				;lda mulByte_lo
				sta mulByte_tmp_lo

				lda #$00
				sta mulByte_hi
				sta mulByte_lo
				sta mulByte_tmp_hi

				ldx #$08
				loopmulByte:
					lsr mulByte_a
					bcc carry0mulByte
						lda mulByte_lo
						clc
						adc mulByte_tmp_lo
						sta mulByte_lo
						
						lda mulByte_hi
						adc mulByte_tmp_hi
						sta mulByte_hi
					carry0mulByte:
					asl mulByte_tmp_lo
					rol mulByte_tmp_hi
				dex
				bne loopmulByte
			;}
			
			lda mulByte_lo
			clc
			adc divNum_ebx
			eor #$FF
			sta mulByte_lo
			
			lda mulByte_hi
			adc #$00
			sta divNum_ebx

			lda mulByte_lo
			sec ;(+1)
			adc (divNum_p1lo),Y
			sta (divNum_p1lo),Y
			cmp mulByte_lo
			bcc elseflg
			beq elseflg
				inc divNum_ebx
			elseflg:

		;dec ecx
		dey
		;jnz lp
		bne lp

		;sub [edi],ebx //p1
		;ldy #$00
		lda (divNum_p1lo),Y
		sec
		sbc divNum_ebx
		sta (divNum_p1lo),Y
		
		;addback 0,1,2 times
		;while (r[idx_main]>0x7FFFFFFF)
		;{
		loop4:
		ldy #$00
		lda (divNum_p1lo),Y
		and #$80
		beq break4

			;mov edi,p1
			;mov esi,p2
			;mov ecx,len
			ldy divNum_len
			;clc
			clc
			lp2:
				;mov eax,[edi+ecx*4]
				lda (divNum_p1lo),Y
				;adc eax,[esi+ecx*4]
				dey
				adc (divNum_buffB_lo),Y
				iny
				;mov [edi+ecx*4],eax
				sta (divNum_p1lo),Y
			;dec ecx
			dey
			;jnz lp2
			bne lp2
			;ldy #$00
			;mov eax,[edi]
			lda (divNum_p1lo),Y
			;adc eax,0
			adc #$00
			;mov [edi],eax
			sta (divNum_p1lo),Y
			;q--;
			dec divNum_q
		;}
		jmp loop4
		break4:

		;Num_Array[idx_main] = q;
		lda divNum_q
		ldy divNum_i
		sta (divNum_num1_lo),Y
		
	inc divNum_i
	;}
	jmp loop_main
	break_main:

	;if(Num_Array[0]==0)
	ldy #$00
	lda (divNum_num1_lo),Y
	bne else5
	;{
		;(*pExponent)--;
		dec divNum_exp1
		;for (int idx_main = 0; idx_main < Precision - 1; idx_main++)
		ldy #$00
		;{
		loop_end:
		cpy #PRECISION - 1
		beq break_end
			;Num_Array[idx_main] = Num_Array[idx_main + 1];
			iny
			lda (divNum_num1_lo),Y
			dey
			sta (divNum_num1_lo),Y
		iny
		;}
		jmp loop_end
		break_end:
	;}
	else5:

	jsr setSignExp
	rts
.endproc
