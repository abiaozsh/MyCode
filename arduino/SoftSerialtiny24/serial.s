	.file	"serial.cpp"
__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__CCP__  = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1
	.global __do_copy_data
	.global __do_clear_bss
 ;  GNU C++ (WinAVR 20081205) version 4.3.2 (avr)
 ; 	compiled by GNU C version 3.4.5 (mingw-vista special r3), GMP version 4.2.3, MPFR version 2.3.2.
 ;  GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 ;  options passed:  -fpreprocessed serial.ii -mmcu=attiny24 -Os
 ;  -fverbose-asm -fno-rtti -fno-enforce-eh-specs -fno-exceptions
 ;  options enabled:  -falign-loops -fargument-alias -fauto-inc-dec
 ;  -fbranch-count-reg -fcaller-saves -fcommon -fcprop-registers
 ;  -fcrossjumping -fcse-follow-jumps -fdefer-pop -fearly-inlining
 ;  -feliminate-unused-debug-types -fexpensive-optimizations
 ;  -fforward-propagate -ffunction-cse -fgcse -fgcse-lm
 ;  -fguess-branch-probability -fident -fif-conversion -fif-conversion2
 ;  -finline-functions -finline-functions-called-once
 ;  -finline-small-functions -fipa-pure-const -fipa-reference -fivopts
 ;  -fkeep-static-consts -fleading-underscore -fmath-errno
 ;  -fmerge-constants -fmerge-debug-strings -fmove-loop-invariants
 ;  -fomit-frame-pointer -foptimize-register-move -foptimize-sibling-calls
 ;  -fpeephole -fpeephole2 -freg-struct-return -fregmove
 ;  -freorder-functions -frerun-cse-after-loop -fsched-interblock
 ;  -fsched-spec -fsched-stalled-insns-dep -fsigned-zeros
 ;  -fsplit-ivs-in-unroller -fsplit-wide-types -fstrict-aliasing
 ;  -fstrict-overflow -fthread-jumps -ftoplevel-reorder -ftrapping-math
 ;  -ftree-ccp -ftree-copy-prop -ftree-copyrename -ftree-dce
 ;  -ftree-dominator-opts -ftree-dse -ftree-fre -ftree-loop-im
 ;  -ftree-loop-ivcanon -ftree-loop-optimize -ftree-parallelize-loops=
 ;  -ftree-reassoc -ftree-salias -ftree-scev-cprop -ftree-sink -ftree-sra
 ;  -ftree-store-ccp -ftree-ter -ftree-vect-loop-version -ftree-vrp
 ;  -funit-at-a-time -fverbose-asm -fzero-initialized-in-bss

 ;  Compiler executable checksum: a5eaa4751655043614dbbfd845ac39f8

	.text
.global	_Z9ClockInitv
	.type	_Z9ClockInitv, @function
_Z9ClockInitv:
/* prologue: function */
/* frame size = 0 */
	ldi r24,lo8(-128)	 ;  tmp42,
	out 70-32,r24	 ; ,, tmp42
	out 70-32,__zero_reg__	 ; ,,
/* epilogue start */
	ret
	.size	_Z9ClockInitv, .-_Z9ClockInitv
.global	_Z9TimerInitv
	.type	_Z9TimerInitv, @function
_Z9TimerInitv:
/* prologue: function */
/* frame size = 0 */
	out 79-32,__zero_reg__	 ; ,,
	out 66-32,__zero_reg__	 ; ,,
	out 44-32,__zero_reg__	 ; ,,
/* epilogue start */
	ret
	.size	_Z9TimerInitv, .-_Z9TimerInitv
.global	_Z10SerialInitv
	.type	_Z10SerialInitv, @function
_Z10SerialInitv:
/* prologue: function */
/* frame size = 0 */
	sbi 58-32,2	 ; ,,
	cbi 58-32,1	 ; ,,
	sbi 59-32,2	 ; ,,
/* epilogue start */
	ret
	.size	_Z10SerialInitv, .-_Z10SerialInitv
.global	_Z10SerialSendh
	.type	_Z10SerialSendh, @function
_Z10SerialSendh:
/* prologue: function */
/* frame size = 0 */
	mov r20,r24	 ;  val, val
/* #APP */
 ;  175 "serial.cpp" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(1)	 ;  tmp69,
	out 78-32,r24	 ; ,, tmp69
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	cbi 59-32,2	 ; ,,
	ldi r30,lo8(TIMING__8M_TCCR1B_1_115200)	 ;  __addr16,
	ldi r31,hi8(TIMING__8M_TCCR1B_1_115200)	 ;  __addr16,
/* #APP */
 ;  180 "serial.cpp" 1
	lpm r18, Z+	 ;  __result
	lpm r19, Z	 ;  __result
	
 ;  0 "" 2
/* #NOAPP */
.L8:
	in r24,76-32	 ;  D.1913,,
	in r25,(76)+1-32	 ;  D.1913,,
	cp r24,r18	 ;  D.1913, __result
	cpc r25,r19	 ;  D.1913, __result
	brlo .L8	 ; ,
	ldi r18,lo8(TIMING__8M_TCCR1B_1_115200+2)	 ;  pTiming,
	ldi r19,hi8(TIMING__8M_TCCR1B_1_115200+2)	 ;  pTiming,
	ldi r22,lo8(1)	 ;  chkbit,
	mov r26,r20	 ;  val, val
	ldi r27,lo8(0)	 ;  val,
.L13:
	mov r24,r22	 ;  chkbit, chkbit
	ldi r25,lo8(0)	 ;  chkbit,
	and r24,r26	 ;  chkbit, val
	and r25,r27	 ;  chkbit, val
	or r24,r25	 ;  chkbit
	breq .L9	 ; ,
	sbi 59-32,2	 ; ,,
	rjmp .L10	 ; 
.L9:
	cbi 59-32,2	 ; ,,
.L10:
	movw r30,r18	 ;  __addr16, pTiming
	subi r18,lo8(-(2))	 ;  pTiming,
	sbci r19,hi8(-(2))	 ;  pTiming,
/* #APP */
 ;  184 "serial.cpp" 1
	lpm r20, Z+	 ;  __result
	lpm r21, Z	 ;  __result
	
 ;  0 "" 2
/* #NOAPP */
.L11:
	in r24,76-32	 ;  D.1929,,
	in r25,(76)+1-32	 ;  D.1929,,
	cp r24,r20	 ;  D.1929, __result
	cpc r25,r21	 ;  D.1929, __result
	brlo .L11	 ; ,
	ldi r24,hi8(TIMING__8M_TCCR1B_1_115200+18)	 ; ,
	cpi r18,lo8(TIMING__8M_TCCR1B_1_115200+18)	 ;  pTiming,
	cpc r19,r24	 ;  pTiming,
	breq .L12	 ; ,
	lsl r22	 ;  chkbit
	rjmp .L13	 ; 
.L12:
	sbi 59-32,2	 ; ,,
	movw r30,r18	 ;  __addr16, pTiming
/* #APP */
 ;  186 "serial.cpp" 1
	lpm r18, Z+	 ;  __result
	lpm r19, Z	 ;  __result
	
 ;  0 "" 2
/* #NOAPP */
.L14:
	in r24,76-32	 ;  D.1936,,
	in r25,(76)+1-32	 ;  D.1936,,
	cp r24,r18	 ;  D.1936, __result
	cpc r25,r19	 ;  D.1936, __result
	brlo .L14	 ; ,
/* #APP */
 ;  187 "serial.cpp" 1
	sei
 ;  0 "" 2
/* epilogue start */
/* #NOAPP */
	ret
	.size	_Z10SerialSendh, .-_Z10SerialSendh
.global	_Z7SendIntm
	.type	_Z7SendIntm, @function
_Z7SendIntm:
	push r14	 ; 
	push r15	 ; 
	push r16	 ; 
	push r17	 ; 
	push r28	 ; 
	push r29	 ; 
/* prologue: function */
/* frame size = 0 */
	ldi r28,lo8(num10s)	 ;  ivtmp.73,
	ldi r29,hi8(num10s)	 ;  ivtmp.73,
	movw r20,r24	 ; ,
	movw r24,r22	 ; , tmp18
	movw r26,r20	 ; ,
	movw r14,r24	 ;  num, num
	movw r16,r26	 ;  num, num
.L22:
	movw r30,r28	 ;  __addr16, ivtmp.73
/* #APP */
 ;  156 "serial.cpp" 1
	lpm r18, Z+	 ;  __result
	lpm r19, Z+	 ;  __result
	lpm r20, Z+	 ;  __result
	lpm r21, Z	 ;  __result
	
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(0)	 ;  outNum,
.L21:
	cp r14,r18	 ;  num, __result
	cpc r15,r19	 ;  num, __result
	cpc r16,r20	 ;  num, __result
	cpc r17,r21	 ;  num, __result
	brlo .L20	 ; ,
	sub r14,r18	 ;  num, __result
	sbc r15,r19	 ;  num, __result
	sbc r16,r20	 ;  num, __result
	sbc r17,r21	 ;  num, __result
	subi r24,lo8(-(1))	 ;  outNum,
	cpi r24,lo8(10)	 ;  outNum,
	brne .L21	 ; ,
.L20:
	subi r24,lo8(-(48))	 ; ,
	rcall _Z10SerialSendh	 ; 
	adiw r28,4	 ;  ivtmp.73,
	ldi r24,hi8(num10s+40)	 ; ,
	cpi r28,lo8(num10s+40)	 ;  ivtmp.73,
	cpc r29,r24	 ;  ivtmp.73,
	brne .L22	 ; ,
/* epilogue start */
	pop r29	 ; 
	pop r28	 ; 
	pop r17	 ; 
	pop r16	 ; 
	pop r15	 ; 
	pop r14	 ; 
	ret
	.size	_Z7SendIntm, .-_Z7SendIntm
.global	_Z10SerialReadjPh
	.type	_Z10SerialReadjPh, @function
_Z10SerialReadjPh:
/* prologue: function */
/* frame size = 0 */
	movw r18,r24	 ;  timeout, timeout
	movw r30,r22	 ;  timoutParam, timoutParam
/* #APP */
 ;  223 "serial.cpp" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	ld r24,Z	 ;  D.1977,* timoutParam
	out 78-32,r24	 ; ,, D.1977
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	sbi 43-32,0	 ; ,,
	rjmp .L27	 ; 
.L30:
	sbic 43-32,0	 ; ,,
	rjmp .L28	 ; 
	in r24,76-32	 ;  iftmp.9,,
	in r25,(76)+1-32	 ;  iftmp.9,,
	cp r24,r18	 ;  iftmp.9, timeout
	cpc r25,r19	 ;  iftmp.9, timeout
	brlo .L27	 ; ,
.L28:
	st Z,__zero_reg__	 ; * timoutParam,
	ldi r22,lo8(0)	 ;  val,
	rjmp .L29	 ; 
.L27:
	sbic 57-32,1	 ; ,,
	rjmp .L30	 ; 
	ldi r24,lo8(1)	 ;  tmp73,
	out 78-32,r24	 ; ,, tmp73
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	ldi r22,lo8(0)	 ;  val,
	ldi r18,lo8(TIMING__8M_TCCR1B_1_115200)	 ;  pTiming,
	ldi r19,hi8(TIMING__8M_TCCR1B_1_115200)	 ;  pTiming,
.L34:
	movw r30,r18	 ;  __addr16, pTiming
	subi r18,lo8(-(2))	 ;  pTiming,
	sbci r19,hi8(-(2))	 ;  pTiming,
/* #APP */
 ;  241 "serial.cpp" 1
	lpm r20, Z+	 ;  __result
	lpm r21, Z	 ;  __result
	
 ;  0 "" 2
/* #NOAPP */
.L31:
	in r24,76-32	 ;  D.2005,,
	in r25,(76)+1-32	 ;  D.2005,,
	cp r24,r20	 ;  D.2005, __result
	cpc r25,r21	 ;  D.2005, __result
	brlo .L31	 ; ,
	sbic 57-32,1	 ; ,,
	rjmp .L32	 ; 
	ldi r24,lo8(0)	 ;  iftmp.12,
	rjmp .L33	 ; 
.L32:
	ldi r24,lo8(-128)	 ;  iftmp.12,
.L33:
	lsr r22	 ;  val
	or r22,r24	 ;  val, iftmp.12
	ldi r24,hi8(TIMING__8M_TCCR1B_1_115200+16)	 ; ,
	cpi r18,lo8(TIMING__8M_TCCR1B_1_115200+16)	 ;  pTiming,
	cpc r19,r24	 ;  pTiming,
	brne .L34	 ; ,
/* #APP */
 ;  244 "serial.cpp" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.L29:
	mov r24,r22	 ; , val
/* epilogue start */
	ret
	.size	_Z10SerialReadjPh, .-_Z10SerialReadjPh
.global	_Z10SerialReadv
	.type	_Z10SerialReadv, @function
_Z10SerialReadv:
	push r29	 ; 
	push r28	 ; 
	push __tmp_reg__
	in r28,__SP_L__	 ; 
	in r29,__SP_H__	 ; 
/* prologue: function */
/* frame size = 1 */
	std Y+1,__zero_reg__	 ;  timoutParam,
	ldi r24,lo8(-1)	 ; ,
	ldi r25,hi8(-1)	 ; ,
	movw r22,r28	 ; ,
	subi r22,lo8(-(1))	 ; ,
	sbci r23,hi8(-(1))	 ; ,
	rcall _Z10SerialReadjPh	 ; 
/* epilogue start */
	pop __tmp_reg__
	pop r28	 ; 
	pop r29	 ; 
	ret
	.size	_Z10SerialReadv, .-_Z10SerialReadv
.global	_Z7ReadIntv
	.type	_Z7ReadIntv, @function
_Z7ReadIntv:
	push r12	 ; 
	push r13	 ; 
	push r15	 ; 
	push r16	 ; 
	push r17	 ; 
	push r29	 ; 
	push r28	 ; 
	in r28,__SP_L__	 ; 
	in r29,__SP_H__	 ; 
	sbiw r28,10	 ; ,
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29	 ; 
	out __SREG__,__tmp_reg__
	out __SP_L__,r28	 ; 
/* prologue: function */
/* frame size = 10 */
	movw r12,r28	 ;  ivtmp.116,
	sec
	adc r12,__zero_reg__	 ;  ivtmp.116
	adc r13,__zero_reg__	 ;  ivtmp.116
	movw r16,r12	 ;  ivtmp.118, ivtmp.116
	clr r15	 ;  i
.L44:
	rcall _Z10SerialReadv	 ; 
	mov r25,r24	 ;  data,
	subi r24,lo8(-(-48))	 ;  tmp89,
	cpi r24,lo8(10)	 ;  tmp89,
	brlo .L42	 ; ,
.L45:
	movw r16,r12	 ;  ivtmp.119, ivtmp.116
	ldi r18,lo8(0)	 ;  num,
	ldi r19,hi8(0)	 ;  num,
	ldi r20,hlo8(0)	 ;  num,
	ldi r21,hhi8(0)	 ;  num,
	rjmp .L43	 ; 
.L42:
	movw r30,r16	 ; , ivtmp.118
	st Z+,r25	 ;  buff, data
	movw r16,r30	 ;  ivtmp.118,
	inc r15	 ;  i
	ldi r31,lo8(10)	 ; ,
	cp r15,r31	 ;  i,
	brne .L44	 ; ,
	rjmp .L45	 ; 
.L46:
	movw r24,r20	 ; , num
	movw r22,r18	 ; , num
	ldi r18,lo8(10)	 ; ,
	ldi r19,hi8(10)	 ; ,
	ldi r20,hlo8(10)	 ; ,
	ldi r21,hhi8(10)	 ; ,
	rcall __mulsi3	 ; 
	movw r18,r22	 ;  num,
	movw r20,r24	 ;  num,
	movw r30,r16	 ; , ivtmp.119
	ld r24,Z+	 ;  tmp92, buff
	movw r16,r30	 ;  ivtmp.119,
	ldi r25,lo8(0)	 ; ,
	sbiw r24,48	 ;  tmp91,
	clr r26	 ;  tmp94
	sbrc r25,7	 ;  tmp94
	com r26	 ;  tmp94
	mov r27,r26	 ;  tmp94
	add r18,r24	 ;  num, tmp94
	adc r19,r25	 ;  num, tmp94
	adc r20,r26	 ;  num, tmp94
	adc r21,r27	 ;  num, tmp94
.L43:
	mov r24,r16	 ;  tmp95,
	sub r24,r12	 ;  tmp95, ivtmp.116
	cp r24,r15	 ;  tmp95, i
	brlo .L46	 ; ,
	movw r22,r18	 ; ,
	movw r24,r20	 ; ,
/* epilogue start */
	adiw r28,10	 ; ,
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29	 ; 
	out __SREG__,__tmp_reg__
	out __SP_L__,r28	 ; 
	pop r28	 ; 
	pop r29	 ; 
	pop r17	 ; 
	pop r16	 ; 
	pop r15	 ; 
	pop r13	 ; 
	pop r12	 ; 
	ret
	.size	_Z7ReadIntv, .-_Z7ReadIntv
.global	_Z4loopv
	.type	_Z4loopv, @function
_Z4loopv:
	push r5	 ; 
	push r6	 ; 
	push r7	 ; 
	push r8	 ; 
	push r9	 ; 
	push r10	 ; 
	push r11	 ; 
	push r12	 ; 
	push r13	 ; 
	push r14	 ; 
	push r15	 ; 
	push r16	 ; 
	push r17	 ; 
	push r28	 ; 
	push r29	 ; 
/* prologue: function */
/* frame size = 0 */
	ldi r21,lo8(2)	 ; ,
	mov r5,r21	 ;  tmp103,
	mov r6,__zero_reg__	 ;  tmp104
	mov r7,__zero_reg__	 ;  tmp104
	ldi r20,hlo8(65536)	 ; ,
	mov r8,r20	 ;  tmp104,
	mov r9,__zero_reg__	 ;  tmp104
.L54:
	ldi r24,lo8(97)	 ; ,
	rcall _Z10SerialSendh	 ; 
	ldi r24,lo8(13)	 ; ,
	rcall _Z10SerialSendh	 ; 
	ldi r24,lo8(10)	 ; ,
	rcall _Z10SerialSendh	 ; 
	rcall _Z7ReadIntv	 ; 
	movw r14,r22	 ;  val1,
	movw r16,r24	 ;  val1,
	ldi r24,lo8(98)	 ; ,
	rcall _Z10SerialSendh	 ; 
	ldi r24,lo8(13)	 ; ,
	rcall _Z10SerialSendh	 ; 
	ldi r24,lo8(10)	 ; ,
	rcall _Z10SerialSendh	 ; 
	rcall _Z7ReadIntv	 ; 
	movw r10,r22	 ;  val2,
	movw r12,r24	 ;  val2,
	movw r24,r16	 ; , val1
	movw r22,r14	 ; , val1
	movw r20,r12	 ; , val2
	movw r18,r10	 ; , val2
	rcall __udivmodsi4
	movw r24,r20	 ; , tmp64
	movw r22,r18	 ; , tmp64
	rcall _Z7SendIntm	 ; 
	ldi r24,lo8(44)	 ; ,
	rcall _Z10SerialSendh	 ; 
	movw r24,r16	 ; , val1
	movw r22,r14	 ; , val1
	movw r20,r12	 ; , val2
	movw r18,r10	 ; , val2
	rcall __udivmodsi4
	rcall _Z7SendIntm	 ; 
	ldi r24,lo8(13)	 ; ,
	rcall _Z10SerialSendh	 ; 
	ldi r24,lo8(10)	 ; ,
	rcall _Z10SerialSendh	 ; 
	out 79-32,__zero_reg__	 ; ,,
	out 78-32,r5	 ; ,, tmp103
	out 66-32,__zero_reg__	 ; ,,
	out 44-32,__zero_reg__	 ; ,,
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	sbi 43-32,0	 ; ,,
	ldi r16,lo8(0)	 ;  temp,
	ldi r17,hi8(0)	 ;  temp,
	movw r16,r14	 ;  temp.125, temp
	clr r15	 ;  temp.125
	clr r14	 ;  temp.125
	movw r24,r16	 ; , temp.125
	movw r22,r14	 ; , temp.125
	ldi r18,lo8(12345)	 ; ,
	ldi r19,hi8(12345)	 ; ,
	ldi r20,hlo8(12345)	 ; ,
	ldi r21,hhi8(12345)	 ; ,
	rcall __udivmodsi4
	movw r24,r18	 ; ,
	movw r26,r20	 ; ,
	movw r20,r8	 ; , tmp104
	movw r18,r6	 ; , tmp104
	sub r18,r24	 ; , temp.126
	sbc r19,r25	 ; , temp.126
	sbc r20,r26	 ; , temp.126
	sbc r21,r27	 ; , temp.126
	movw r24,r20	 ;  temp.127,
	movw r22,r18	 ;  temp.127,
	ldi r18,lo8(12345)	 ; ,
	ldi r19,hi8(12345)	 ; ,
	ldi r20,hlo8(12345)	 ; ,
	ldi r21,hhi8(12345)	 ; ,
	rcall __mulsi3	 ; 
	ldi r18,10	 ; ,
1:	lsr r25	 ;  temp.129
	ror r24	 ;  temp.129
	ror r23	 ;  temp.129
	ror r22	 ;  temp.129
	dec r18	 ; 
	brne 1b
	cpi r22,lo8(12346)	 ;  temp.129,
	ldi r19,hi8(12346)	 ; ,
	cpc r23,r19	 ;  temp.129,
	ldi r19,hlo8(12346)	 ; ,
	cpc r24,r19	 ;  temp.129,
	ldi r19,hhi8(12346)	 ; ,
	cpc r25,r19	 ;  temp.129,
	brlo .L51	 ; ,
	ldi r22,lo8(12345)	 ;  Power,
	ldi r23,hi8(12345)	 ;  Power,
.L51:
	sbis 43-32,0	 ; ,,
	rjmp .L52	 ; 
	ldi r28,lo8(-1)	 ;  Tick,
	ldi r29,hi8(-1)	 ;  Tick,
	rjmp .L53	 ; 
.L52:
	in r28,76-32	 ;  Tick,,
	in r29,(76)+1-32	 ;  Tick,,
.L53:
	ldi r24,lo8(0)	 ;  Power,
	ldi r25,hi8(0)	 ;  Power,
	rcall _Z7SendIntm	 ; 
	ldi r24,lo8(44)	 ; ,
	rcall _Z10SerialSendh	 ; 
	movw r22,r28	 ;  Tick, Tick
	ldi r24,lo8(0)	 ;  Tick,
	ldi r25,hi8(0)	 ;  Tick,
	rcall _Z7SendIntm	 ; 
	ldi r24,lo8(13)	 ; ,
	rcall _Z10SerialSendh	 ; 
	ldi r24,lo8(10)	 ; ,
	rcall _Z10SerialSendh	 ; 
	rjmp .L54	 ; 
	.size	_Z4loopv, .-_Z4loopv
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
	ldi r24,lo8(-128)	 ;  tmp49,
	out 70-32,r24	 ; ,, tmp49
	out 70-32,__zero_reg__	 ; ,,
	sbi 58-32,2	 ; ,,
	cbi 58-32,1	 ; ,,
	sbi 59-32,2	 ; ,,
	out 79-32,__zero_reg__	 ; ,,
	out 66-32,__zero_reg__	 ; ,,
	out 44-32,__zero_reg__	 ; ,,
	rcall _Z4loopv	 ; 
	ldi r24,lo8(0)	 ; ,
	ldi r25,hi8(0)	 ; ,
/* epilogue start */
	ret
	.size	main, .-main
.global	TIMING__8M_TCCR1B_3____300
	.section	.progmem.data,"a",@progbits
	.type	TIMING__8M_TCCR1B_3____300, @object
	.size	TIMING__8M_TCCR1B_3____300, 20
TIMING__8M_TCCR1B_3____300:
	.word	416
	.word	833
	.word	1250
	.word	1666
	.word	2083
	.word	2500
	.word	2916
	.word	3333
	.word	3750
	.word	4166
.global	TIMING__8M_TCCR1B_2___1200
	.type	TIMING__8M_TCCR1B_2___1200, @object
	.size	TIMING__8M_TCCR1B_2___1200, 20
TIMING__8M_TCCR1B_2___1200:
	.word	833
	.word	1666
	.word	2500
	.word	3333
	.word	4166
	.word	5000
	.word	5833
	.word	6666
	.word	7500
	.word	8333
.global	TIMING__8M_TCCR1B_2___2400
	.type	TIMING__8M_TCCR1B_2___2400, @object
	.size	TIMING__8M_TCCR1B_2___2400, 20
TIMING__8M_TCCR1B_2___2400:
	.word	416
	.word	833
	.word	1250
	.word	1666
	.word	2083
	.word	2500
	.word	2916
	.word	3333
	.word	3750
	.word	4166
.global	TIMING__8M_TCCR1B_2___4800
	.type	TIMING__8M_TCCR1B_2___4800, @object
	.size	TIMING__8M_TCCR1B_2___4800, 20
TIMING__8M_TCCR1B_2___4800:
	.word	208
	.word	416
	.word	625
	.word	833
	.word	1041
	.word	1250
	.word	1458
	.word	1666
	.word	1875
	.word	2083
.global	TIMING__8M_TCCR1B_1___9600
	.type	TIMING__8M_TCCR1B_1___9600, @object
	.size	TIMING__8M_TCCR1B_1___9600, 20
TIMING__8M_TCCR1B_1___9600:
	.word	833
	.word	1666
	.word	2500
	.word	3333
	.word	4166
	.word	5000
	.word	5833
	.word	6666
	.word	7500
	.word	8333
.global	TIMING__8M_TCCR1B_1__14400
	.type	TIMING__8M_TCCR1B_1__14400, @object
	.size	TIMING__8M_TCCR1B_1__14400, 20
TIMING__8M_TCCR1B_1__14400:
	.word	555
	.word	1111
	.word	1666
	.word	2222
	.word	2777
	.word	3333
	.word	3888
	.word	4444
	.word	5000
	.word	5555
.global	TIMING__8M_TCCR1B_1__19200
	.type	TIMING__8M_TCCR1B_1__19200, @object
	.size	TIMING__8M_TCCR1B_1__19200, 20
TIMING__8M_TCCR1B_1__19200:
	.word	416
	.word	833
	.word	1250
	.word	1666
	.word	2083
	.word	2500
	.word	2916
	.word	3333
	.word	3750
	.word	4166
.global	TIMING__8M_TCCR1B_1__28800
	.type	TIMING__8M_TCCR1B_1__28800, @object
	.size	TIMING__8M_TCCR1B_1__28800, 20
TIMING__8M_TCCR1B_1__28800:
	.word	277
	.word	555
	.word	833
	.word	1111
	.word	1388
	.word	1666
	.word	1944
	.word	2222
	.word	2500
	.word	2777
.global	TIMING__8M_TCCR1B_1__38400
	.type	TIMING__8M_TCCR1B_1__38400, @object
	.size	TIMING__8M_TCCR1B_1__38400, 20
TIMING__8M_TCCR1B_1__38400:
	.word	208
	.word	416
	.word	625
	.word	833
	.word	1041
	.word	1250
	.word	1458
	.word	1666
	.word	1875
	.word	2083
.global	TIMING__8M_TCCR1B_1__57600
	.type	TIMING__8M_TCCR1B_1__57600, @object
	.size	TIMING__8M_TCCR1B_1__57600, 20
TIMING__8M_TCCR1B_1__57600:
	.word	138
	.word	277
	.word	416
	.word	555
	.word	694
	.word	833
	.word	972
	.word	1111
	.word	1250
	.word	1388
.global	TIMING__8M_TCCR1B_1_115200
	.type	TIMING__8M_TCCR1B_1_115200, @object
	.size	TIMING__8M_TCCR1B_1_115200, 20
TIMING__8M_TCCR1B_1_115200:
	.word	69
	.word	138
	.word	208
	.word	277
	.word	347
	.word	416
	.word	486
	.word	555
	.word	625
	.word	694
.global	buff
.global	buff
	.section .bss
	.type	buff, @object
	.size	buff, 8
buff:
	.skip 8,0
.global	num10s
	.section	.progmem.data
	.type	num10s, @object
	.size	num10s, 40
num10s:
	.byte	0
	.byte	-54
	.byte	-102
	.byte	59
	.byte	0
	.byte	-31
	.byte	-11
	.byte	5
	.byte	-128
	.byte	-106
	.byte	-104
	.byte	0
	.byte	64
	.byte	66
	.byte	15
	.byte	0
	.byte	-96
	.byte	-122
	.byte	1
	.byte	0
	.byte	16
	.byte	39
	.byte	0
	.byte	0
	.byte	-24
	.byte	3
	.byte	0
	.byte	0
	.byte	100
	.byte	0
	.byte	0
	.byte	0
	.byte	10
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	0
