	.file	"motor6.cpp"
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
 ;  options passed:  -fpreprocessed motor6.ii -mmcu=attiny24 -Os
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
.global	_Z10AnalogInitv
	.type	_Z10AnalogInitv, @function
_Z10AnalogInitv:
/* prologue: function */
/* frame size = 0 */
	ldi r24,lo8(6)	 ;  tmp42,
	out 39-32,r24	 ; ,, tmp42
	out 35-32,__zero_reg__	 ; ,,
	ldi r24,lo8(-57)	 ;  tmp45,
	out 38-32,r24	 ; ,, tmp45
/* epilogue start */
	ret
	.size	_Z10AnalogInitv, .-_Z10AnalogInitv
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
 ;  150 "motor6.cpp" 1
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
 ;  155 "motor6.cpp" 1
	lpm r18, Z+	 ;  __result
	lpm r19, Z	 ;  __result
	
 ;  0 "" 2
/* #NOAPP */
.L10:
	in r24,76-32	 ;  D.1914,,
	in r25,(76)+1-32	 ;  D.1914,,
	cp r24,r18	 ;  D.1914, __result
	cpc r25,r19	 ;  D.1914, __result
	brlo .L10	 ; ,
	ldi r18,lo8(TIMING__8M_TCCR1B_1_115200+2)	 ;  pTiming,
	ldi r19,hi8(TIMING__8M_TCCR1B_1_115200+2)	 ;  pTiming,
	ldi r22,lo8(1)	 ;  chkbit,
	mov r26,r20	 ;  val, val
	ldi r27,lo8(0)	 ;  val,
.L15:
	mov r24,r22	 ;  chkbit, chkbit
	ldi r25,lo8(0)	 ;  chkbit,
	and r24,r26	 ;  chkbit, val
	and r25,r27	 ;  chkbit, val
	or r24,r25	 ;  chkbit
	breq .L11	 ; ,
	sbi 59-32,2	 ; ,,
	rjmp .L12	 ; 
.L11:
	cbi 59-32,2	 ; ,,
.L12:
	movw r30,r18	 ;  __addr16, pTiming
	subi r18,lo8(-(2))	 ;  pTiming,
	sbci r19,hi8(-(2))	 ;  pTiming,
/* #APP */
 ;  159 "motor6.cpp" 1
	lpm r20, Z+	 ;  __result
	lpm r21, Z	 ;  __result
	
 ;  0 "" 2
/* #NOAPP */
.L13:
	in r24,76-32	 ;  D.1930,,
	in r25,(76)+1-32	 ;  D.1930,,
	cp r24,r20	 ;  D.1930, __result
	cpc r25,r21	 ;  D.1930, __result
	brlo .L13	 ; ,
	ldi r24,hi8(TIMING__8M_TCCR1B_1_115200+18)	 ; ,
	cpi r18,lo8(TIMING__8M_TCCR1B_1_115200+18)	 ;  pTiming,
	cpc r19,r24	 ;  pTiming,
	breq .L14	 ; ,
	lsl r22	 ;  chkbit
	rjmp .L15	 ; 
.L14:
	sbi 59-32,2	 ; ,,
	movw r30,r18	 ;  __addr16, pTiming
/* #APP */
 ;  161 "motor6.cpp" 1
	lpm r18, Z+	 ;  __result
	lpm r19, Z	 ;  __result
	
 ;  0 "" 2
/* #NOAPP */
.L16:
	in r24,76-32	 ;  D.1937,,
	in r25,(76)+1-32	 ;  D.1937,,
	cp r24,r18	 ;  D.1937, __result
	cpc r25,r19	 ;  D.1937, __result
	brlo .L16	 ; ,
/* #APP */
 ;  162 "motor6.cpp" 1
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
	ldi r28,lo8(num10s)	 ;  ivtmp.78,
	ldi r29,hi8(num10s)	 ;  ivtmp.78,
	movw r20,r24	 ; ,
	movw r24,r22	 ; , tmp18
	movw r26,r20	 ; ,
	movw r14,r24	 ;  num, num
	movw r16,r26	 ;  num, num
.L24:
	movw r30,r28	 ;  __addr16, ivtmp.78
/* #APP */
 ;  131 "motor6.cpp" 1
	lpm r18, Z+	 ;  __result
	lpm r19, Z+	 ;  __result
	lpm r20, Z+	 ;  __result
	lpm r21, Z	 ;  __result
	
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(0)	 ;  outNum,
.L23:
	cp r14,r18	 ;  num, __result
	cpc r15,r19	 ;  num, __result
	cpc r16,r20	 ;  num, __result
	cpc r17,r21	 ;  num, __result
	brlo .L22	 ; ,
	sub r14,r18	 ;  num, __result
	sbc r15,r19	 ;  num, __result
	sbc r16,r20	 ;  num, __result
	sbc r17,r21	 ;  num, __result
	subi r24,lo8(-(1))	 ;  outNum,
	cpi r24,lo8(10)	 ;  outNum,
	brne .L23	 ; ,
.L22:
	subi r24,lo8(-(48))	 ; ,
	rcall _Z10SerialSendh	 ; 
	adiw r28,4	 ;  ivtmp.78,
	ldi r24,hi8(num10s+40)	 ; ,
	cpi r28,lo8(num10s+40)	 ;  ivtmp.78,
	cpc r29,r24	 ;  ivtmp.78,
	brne .L24	 ; ,
/* epilogue start */
	pop r29	 ; 
	pop r28	 ; 
	pop r17	 ; 
	pop r16	 ; 
	pop r15	 ; 
	pop r14	 ; 
	ret
	.size	_Z7SendIntm, .-_Z7SendIntm
.global	_Z4loopv
	.type	_Z4loopv, @function
_Z4loopv:
	push r9	 ; 
	push r10	 ; 
	push r11	 ; 
	push r12	 ; 
	push r13	 ; 
	push r14	 ; 
	push r15	 ; 
	push r16	 ; 
	push r17	 ; 
/* prologue: function */
/* frame size = 0 */
	ldi r23,lo8(7)	 ; ,
	mov r9,r23	 ;  tmp89,
	ldi r22,lo8(16)	 ; ,
	mov r10,r22	 ;  tmp90,
	ldi r21,lo8(-60)	 ; ,
	mov r11,r21	 ;  tmp91,
	ldi r20,lo8(6)	 ; ,
	mov r12,r20	 ;  tmp92,
	ldi r19,lo8(-57)	 ; ,
	mov r13,r19	 ;  tmp93,
.L31:
	in r22,36-32	 ;  aread1,,
	in r23,(36)+1-32	 ;  aread1,,
	sbi 38-32,6	 ; ,,
	ldi r24,lo8(0)	 ;  aread1,
	ldi r25,hi8(0)	 ;  aread1,
	rcall _Z7SendIntm	 ; 
	ldi r24,lo8(13)	 ; ,
	rcall _Z10SerialSendh	 ; 
	ldi r24,lo8(10)	 ; ,
	rcall _Z10SerialSendh	 ; 
	ldi r18,lo8(0)	 ;  i,
	ldi r19,hi8(0)	 ;  i,
	ldi r20,hlo8(0)	 ;  i,
	ldi r21,hhi8(0)	 ;  i,
.L29:
	lds r24,temp	 ;  temp.0, temp
	subi r24,lo8(-(1))	 ;  temp.0,
	sts temp,r24	 ;  temp, temp.0
	subi r18,lo8(-(1))	 ;  i,
	sbci r19,hi8(-(1))	 ;  i,
	sbci r20,hlo8(-(1))	 ;  i,
	sbci r21,hhi8(-(1))	 ;  i,
	cpi r18,lo8(500000)	 ;  i,
	ldi r24,hi8(500000)	 ; ,
	cpc r19,r24	 ;  i,
	ldi r24,hlo8(500000)	 ; ,
	cpc r20,r24	 ;  i,
	ldi r24,hhi8(500000)	 ; ,
	cpc r21,r24	 ;  i,
	brne .L29	 ; ,
	out 38-32,__zero_reg__	 ; ,,
	out 39-32,r9	 ; ,, tmp89
	out 35-32,r10	 ; ,, tmp90
	out 38-32,r11	 ; ,, tmp91
.L30:
	in r24,38-32	 ;  D.1859,,
	andi r24,lo8(6)	 ;  D.1859,
	brne .L30	 ; ,
	in r14,37-32	 ;  aread2,,
	mov r22,r14	 ;  aread2, aread2
	ldi r23,lo8(0)	 ;  aread2,
	ldi r24,lo8(0)	 ;  aread2,
	ldi r25,hi8(0)	 ;  aread2,
	rcall _Z7SendIntm	 ; 
	ldi r24,lo8(13)	 ; ,
	rcall _Z10SerialSendh	 ; 
	ldi r24,lo8(10)	 ; ,
	rcall _Z10SerialSendh	 ; 
	out 39-32,r12	 ; ,, tmp92
	out 35-32,__zero_reg__	 ; ,,
	out 38-32,r13	 ; ,, tmp93
	ldi r24,lo8(127)	 ; ,
	and r14,r24	 ;  aread2,
	clr r15	 ; 
	ldi r16,lo8(0)	 ; ,
	ldi r17,hi8(0)	 ; ,
	movw r24,r16	 ; , tmp78
	movw r22,r14	 ; , tmp78
	ldi r18,lo8(12300)	 ; ,
	ldi r19,hi8(12300)	 ; ,
	ldi r20,hlo8(12300)	 ; ,
	ldi r21,hhi8(12300)	 ; ,
	rcall __mulsi3	 ; 
	ldi r18,7	 ; ,
1:	lsr r25	 ;  tmp79
	ror r24	 ;  tmp79
	ror r23	 ;  tmp79
	ror r22	 ;  tmp79
	dec r18	 ; 
	brne 1b
	ldi r24,lo8(0)	 ; ,
	ldi r25,hi8(0)	 ; ,
	rcall _Z7SendIntm	 ; 
	ldi r24,lo8(13)	 ; ,
	rcall _Z10SerialSendh	 ; 
	ldi r24,lo8(10)	 ; ,
	rcall _Z10SerialSendh	 ; 
	ldi r22,lo8(101)	 ; ,
	ldi r23,hi8(101)	 ; ,
	ldi r24,hlo8(101)	 ; ,
	ldi r25,hhi8(101)	 ; ,
	rcall _Z7SendIntm	 ; 
	ldi r24,lo8(13)	 ; ,
	rcall _Z10SerialSendh	 ; 
	ldi r24,lo8(10)	 ; ,
	rcall _Z10SerialSendh	 ; 
	rjmp .L31	 ; 
	.size	_Z4loopv, .-_Z4loopv
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
	ldi r24,lo8(-128)	 ;  tmp49,
	out 70-32,r24	 ; ,, tmp49
	out 70-32,__zero_reg__	 ; ,,
	ldi r24,lo8(6)	 ;  tmp52,
	out 39-32,r24	 ; ,, tmp52
	out 35-32,__zero_reg__	 ; ,,
	ldi r24,lo8(-57)	 ;  tmp55,
	out 38-32,r24	 ; ,, tmp55
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
.global	_Z10SerialReadjPh
	.type	_Z10SerialReadjPh, @function
_Z10SerialReadjPh:
/* prologue: function */
/* frame size = 0 */
	movw r18,r24	 ;  timeout, timeout
	movw r30,r22	 ;  timoutParam, timoutParam
/* #APP */
 ;  198 "motor6.cpp" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	ld r24,Z	 ;  D.1978,* timoutParam
	out 78-32,r24	 ; ,, D.1978
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	sbi 43-32,0	 ; ,,
	rjmp .L38	 ; 
.L41:
	sbic 43-32,0	 ; ,,
	rjmp .L39	 ; 
	in r24,76-32	 ;  iftmp.9,,
	in r25,(76)+1-32	 ;  iftmp.9,,
	cp r24,r18	 ;  iftmp.9, timeout
	cpc r25,r19	 ;  iftmp.9, timeout
	brlo .L38	 ; ,
.L39:
	st Z,__zero_reg__	 ; * timoutParam,
	ldi r22,lo8(0)	 ;  val,
	rjmp .L40	 ; 
.L38:
	sbic 57-32,1	 ; ,,
	rjmp .L41	 ; 
	ldi r24,lo8(1)	 ;  tmp73,
	out 78-32,r24	 ; ,, tmp73
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	ldi r22,lo8(0)	 ;  val,
	ldi r18,lo8(TIMING__8M_TCCR1B_1_115200)	 ;  pTiming,
	ldi r19,hi8(TIMING__8M_TCCR1B_1_115200)	 ;  pTiming,
.L45:
	movw r30,r18	 ;  __addr16, pTiming
	subi r18,lo8(-(2))	 ;  pTiming,
	sbci r19,hi8(-(2))	 ;  pTiming,
/* #APP */
 ;  216 "motor6.cpp" 1
	lpm r20, Z+	 ;  __result
	lpm r21, Z	 ;  __result
	
 ;  0 "" 2
/* #NOAPP */
.L42:
	in r24,76-32	 ;  D.2006,,
	in r25,(76)+1-32	 ;  D.2006,,
	cp r24,r20	 ;  D.2006, __result
	cpc r25,r21	 ;  D.2006, __result
	brlo .L42	 ; ,
	sbic 57-32,1	 ; ,,
	rjmp .L43	 ; 
	ldi r24,lo8(0)	 ;  iftmp.12,
	rjmp .L44	 ; 
.L43:
	ldi r24,lo8(-128)	 ;  iftmp.12,
.L44:
	lsr r22	 ;  val
	or r22,r24	 ;  val, iftmp.12
	ldi r24,hi8(TIMING__8M_TCCR1B_1_115200+16)	 ; ,
	cpi r18,lo8(TIMING__8M_TCCR1B_1_115200+16)	 ;  pTiming,
	cpc r19,r24	 ;  pTiming,
	brne .L45	 ; ,
/* #APP */
 ;  219 "motor6.cpp" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.L40:
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
	movw r12,r28	 ;  ivtmp.139,
	sec
	adc r12,__zero_reg__	 ;  ivtmp.139
	adc r13,__zero_reg__	 ;  ivtmp.139
	movw r16,r12	 ;  ivtmp.141, ivtmp.139
	clr r15	 ;  i
.L55:
	rcall _Z10SerialReadv	 ; 
	mov r25,r24	 ;  data,
	subi r24,lo8(-(-48))	 ;  tmp89,
	cpi r24,lo8(10)	 ;  tmp89,
	brlo .L53	 ; ,
.L56:
	movw r16,r12	 ;  ivtmp.142, ivtmp.139
	ldi r18,lo8(0)	 ;  num,
	ldi r19,hi8(0)	 ;  num,
	ldi r20,hlo8(0)	 ;  num,
	ldi r21,hhi8(0)	 ;  num,
	rjmp .L54	 ; 
.L53:
	movw r30,r16	 ; , ivtmp.141
	st Z+,r25	 ;  buff, data
	movw r16,r30	 ;  ivtmp.141,
	inc r15	 ;  i
	ldi r31,lo8(10)	 ; ,
	cp r15,r31	 ;  i,
	brne .L55	 ; ,
	rjmp .L56	 ; 
.L57:
	movw r24,r20	 ; , num
	movw r22,r18	 ; , num
	ldi r18,lo8(10)	 ; ,
	ldi r19,hi8(10)	 ; ,
	ldi r20,hlo8(10)	 ; ,
	ldi r21,hhi8(10)	 ; ,
	rcall __mulsi3	 ; 
	movw r18,r22	 ;  num,
	movw r20,r24	 ;  num,
	movw r30,r16	 ; , ivtmp.142
	ld r24,Z+	 ;  tmp92, buff
	movw r16,r30	 ;  ivtmp.142,
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
.L54:
	mov r24,r16	 ;  tmp95,
	sub r24,r12	 ;  tmp95, ivtmp.139
	cp r24,r15	 ;  tmp95, i
	brlo .L57	 ; ,
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
.global	TIMING__8M_TCCR1B_1_115200
	.section	.progmem.data,"a",@progbits
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
.global	temp
.global	temp
	.section .bss
	.type	temp, @object
	.size	temp, 1
temp:
	.skip 1,0
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
