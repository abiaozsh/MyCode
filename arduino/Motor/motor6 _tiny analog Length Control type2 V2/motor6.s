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
.global	_Z4waitj
	.type	_Z4waitj, @function
_Z4waitj:
/* prologue: function */
/* frame size = 0 */
	movw r18,r24	 ;  ticks, ticks
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	sbi 43-32,0	 ; ,,
.L3:
	sbic 43-32,0	 ; ,,
	rjmp .L4	 ; 
	in r24,76-32	 ;  iftmp.3,,
	in r25,(76)+1-32	 ;  iftmp.3,,
	cp r24,r18	 ;  iftmp.3, ticks
	cpc r25,r19	 ;  iftmp.3, ticks
	brlo .L3	 ; ,
.L4:
	ret
	.size	_Z4waitj, .-_Z4waitj
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
	ldi r24,lo8(2)	 ;  tmp43,
	out 78-32,r24	 ; ,, tmp43
	out 66-32,__zero_reg__	 ; ,,
	out 44-32,__zero_reg__	 ; ,,
/* epilogue start */
	ret
	.size	_Z9TimerInitv, .-_Z9TimerInitv
.global	_Z10AnalogInitv
	.type	_Z10AnalogInitv, @function
_Z10AnalogInitv:
/* prologue: function */
/* frame size = 0 */
	ldi r24,lo8(7)	 ;  tmp42,
	out 39-32,r24	 ; ,, tmp42
	out 35-32,__zero_reg__	 ; ,,
	ldi r24,lo8(-57)	 ;  tmp45,
	out 38-32,r24	 ; ,, tmp45
/* epilogue start */
	ret
	.size	_Z10AnalogInitv, .-_Z10AnalogInitv
.global	_Z5waitav
	.type	_Z5waitav, @function
_Z5waitav:
	push r17	 ; 
/* prologue: function */
/* frame size = 0 */
	lds r24,Step	 ;  Step, Step
	ldi r25,lo8(0)	 ;  D.1921,
	movw r30,r24	 ;  tmp57, D.1921
	subi r30,lo8(-(DigitReadBaseVal))	 ;  tmp57,
	sbci r31,hi8(-(DigitReadBaseVal))	 ;  tmp57,
	ld r17,Z	 ;  valbase, DigitReadBaseVal
	lds r20,rpm	 ;  temp, rpm
	lds r21,(rpm)+1	 ;  temp, rpm
	lsr r21	 ;  temp
	ror r20	 ;  temp
	lds r22,Power	 ;  Power.21, Power
	lds r23,(Power)+1	 ;  Power.21, Power
	movw r30,r24	 ;  tmp82, D.1921
	subi r30,lo8(-(PWR_OFF))	 ;  tmp82,
	sbci r31,hi8(-(PWR_OFF))	 ;  tmp82,
	movw r26,r24	 ;  tmp83, D.1921
	subi r26,lo8(-(DigitRead))	 ;  tmp83,
	sbci r27,hi8(-(DigitRead))	 ;  tmp83,
.L24:
	sbis 43-32,0	 ; ,,
	rjmp .L13	 ; 
	ldi r24,lo8(-1)	 ;  iftmp.20,
	ldi r25,hi8(-1)	 ;  iftmp.20,
	rjmp .L14	 ; 
.L13:
	in r24,76-32	 ;  iftmp.20,,
	in r25,(76)+1-32	 ;  iftmp.20,,
.L14:
	cp r22,r24	 ;  Power.21, iftmp.20
	cpc r23,r25	 ;  Power.21, iftmp.20
	brsh .L15	 ; ,
	ld r24,Z	 ;  D.1936, PWR_OFF
	out 59-32,r24	 ; ,, D.1936
.L15:
	sbis 57-32,6	 ; ,,
	rjmp .L16	 ; 
	sbic 43-32,0	 ; ,,
	rjmp .L21	 ; 
	in r24,76-32	 ;  D.1949,,
	in r25,(76)+1-32	 ;  D.1949,,
	subi r24,lo8(-1)	 ;  D.1949,
	sbci r25,hi8(-1)	 ;  D.1949,
	brne .L16	 ; ,
	rjmp .L21	 ; 
.L26:
	in r24,76-32	 ;  iftmp.26,,
	in r25,(76)+1-32	 ;  iftmp.26,,
	cp r24,r20	 ;  iftmp.26, temp
	cpc r25,r21	 ;  iftmp.26, temp
	brlo .L24	 ; ,
.L19:
	and r18,r19	 ;  D.1952, D.1951
	cp r18,r17	 ;  D.1952, valbase
	brne .L21	 ; ,
	rjmp .L24	 ; 
.L16:
	in r19,54-32	 ;  D.1951,,
	ld r18,X	 ;  D.1952, DigitRead
	sbic 43-32,0	 ; ,,
	rjmp .L19	 ; 
	rjmp .L26	 ; 
.L21:
/* epilogue start */
	pop r17	 ; 
	ret
	.size	_Z5waitav, .-_Z5waitav
.global	_Z3adjv
	.type	_Z3adjv, @function
_Z3adjv:
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
	sts (NextPower)+1,__zero_reg__	 ;  NextPower,
	sts NextPower,__zero_reg__	 ;  NextPower,
	in r30,36-32	 ;  aread,,
	in r31,(36)+1-32	 ;  aread,,
	sbi 38-32,6	 ; ,,
	ldi r24,hi8(512)	 ; ,
	cpi r30,lo8(512)	 ;  aread,
	cpc r31,r24	 ;  aread,
	brlo .L28	 ; ,
	andi r31,hi8(511)	 ;  aread,
	lsr r31	 ;  aread
	ror r30	 ;  aread
	subi r30,lo8(-(PWR_Table2))	 ;  aread,
	sbci r31,hi8(-(PWR_Table2))	 ;  aread,
/* #APP */
 ;  214 "motor6.cpp" 1
	lpm r24, Z	 ;  __result
	
 ;  0 "" 2
/* #NOAPP */
	sts TargetRPM,r24	 ;  TargetRPM, __result
	sts TargetRPM+1,__zero_reg__	 ;  TargetRPM,
	rjmp .L29	 ; 
.L28:
	andi r30,lo8(-2)	 ;  __addr16,
	subi r30,lo8(-(PWR_Table1))	 ;  __addr16,
	sbci r31,hi8(-(PWR_Table1))	 ;  __addr16,
/* #APP */
 ;  218 "motor6.cpp" 1
	lpm r24, Z+	 ;  __result
	lpm r25, Z	 ;  __result
	
 ;  0 "" 2
/* #NOAPP */
	sts (TargetRPM)+1,r25	 ;  TargetRPM, __result
	sts TargetRPM,r24	 ;  TargetRPM, __result
.L29:
	lds r24,Status	 ;  Status, Status
	lds r18,rpm	 ;  rpm, rpm
	lds r19,(rpm)+1	 ;  rpm, rpm
	tst r24	 ;  Status
	brne .+2	 ; 
	rjmp .L30	 ; 
	ldi r24,hi8(11001)	 ; ,
	cpi r18,lo8(11001)	 ;  rpm,
	cpc r19,r24	 ;  rpm,
	brlo .L31	 ; ,
	sts StartUpCount1,__zero_reg__	 ;  StartUpCount1,
	sts Status,__zero_reg__	 ;  Status,
	rjmp .L36	 ; 
.L31:
	lds r22,TargetRPM	 ;  TargetRPM.31, TargetRPM
	lds r23,(TargetRPM)+1	 ;  TargetRPM.31, TargetRPM
	cp r22,r18	 ;  TargetRPM.31, rpm
	cpc r23,r19	 ;  TargetRPM.31, rpm
	brlo .+2	 ; 
	rjmp .L36	 ; 
	cpi r18,201	 ;  rpm,
	cpc r19,__zero_reg__	 ;  rpm
	brlo .L33	 ; ,
	ldi r24,lo8(0)	 ;  temp,
	ldi r25,hi8(0)	 ;  temp,
	movw r24,r22	 ;  temp.65, temp
	clr r23	 ;  temp.65
	clr r22	 ;  temp.65
	movw r10,r18	 ;  D.1985, rpm
	clr r12	 ;  D.1985
	clr r13	 ;  D.1985
	movw r20,r12	 ; , D.1985
	movw r18,r10	 ; , D.1985
	rcall __udivmodsi4
	movw r14,r18	 ; ,
	movw r16,r20	 ; ,
	ldi r22,lo8(65536)	 ;  tmp69,
	ldi r23,hi8(65536)	 ;  tmp69,
	ldi r24,hlo8(65536)	 ;  tmp69,
	ldi r25,hhi8(65536)	 ;  tmp69,
	sub r22,r14	 ;  tmp69, temp.66
	sbc r23,r15	 ;  tmp69, temp.66
	sbc r24,r16	 ;  tmp69, temp.66
	sbc r25,r17	 ;  tmp69, temp.66
	movw r20,r12	 ; , D.1985
	movw r18,r10	 ; , D.1985
	rcall __mulsi3	 ; 
	ldi r19,15	 ; ,
1:	lsr r25	 ;  tmp70
	ror r24	 ;  tmp70
	ror r23	 ;  tmp70
	ror r22	 ;  tmp70
	dec r19	 ; 
	brne 1b
	sts (NextPower)+1,r23	 ;  NextPower, tmp70
	sts NextPower,r22	 ;  NextPower, tmp70
	rjmp .L36	 ; 
.L33:
	ldi r24,lo8(1000)	 ;  tmp72,
	ldi r25,hi8(1000)	 ;  tmp72,
	sts (NextPower)+1,r25	 ;  NextPower, tmp72
	sts NextPower,r24	 ;  NextPower, tmp72
	rjmp .L36	 ; 
.L30:
	subi r18,lo8(-(-1501))	 ;  rpm,
	sbci r19,hi8(-(-1501))	 ;  rpm,
	subi r18,lo8(10499)	 ;  rpm,
	sbci r19,hi8(10499)	 ;  rpm,
	brsh .L34	 ; ,
	lds r24,StartUpCount1	 ;  StartUpCount1, StartUpCount1
	subi r24,lo8(-(1))	 ;  StartUpCount1,
	sts StartUpCount1,r24	 ;  StartUpCount1, StartUpCount1
	rjmp .L35	 ; 
.L34:
	sts StartUpCount1,__zero_reg__	 ;  StartUpCount1,
.L35:
	lds r24,StartUpCount1	 ;  StartUpCount1, StartUpCount1
	cpi r24,lo8(51)	 ;  StartUpCount1,
	brlo .L36	 ; ,
	ldi r24,lo8(1)	 ;  tmp78,
	sts Status,r24	 ;  Status, tmp78
.L36:
/* epilogue start */
	pop r17	 ; 
	pop r16	 ; 
	pop r15	 ; 
	pop r14	 ; 
	pop r13	 ; 
	pop r12	 ; 
	pop r11	 ; 
	pop r10	 ; 
	ret
	.size	_Z3adjv, .-_Z3adjv
.global	_Z4loopv
	.type	_Z4loopv, @function
_Z4loopv:
/* prologue: function */
/* frame size = 0 */
.L49:
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	sbi 43-32,0	 ; ,,
	sbis 57-32,6	 ; ,,
	rjmp .L38	 ; 
	lds r18,TargetRPM	 ;  TargetRPM.5, TargetRPM
	lds r19,(TargetRPM)+1	 ;  TargetRPM.5, TargetRPM
	movw r20,r18	 ;  temp, TargetRPM.5
	lsr r21	 ;  temp
	ror r20	 ;  temp
	lds r24,Step	 ;  Step, Step
	mov r30,r24	 ;  tmp106, Step
	ldi r31,lo8(0)	 ; ,
	subi r30,lo8(-(PWR_OFF))	 ;  tmp106,
	sbci r31,hi8(-(PWR_OFF))	 ;  tmp106,
.L42:
	sbic 43-32,0	 ; ,,
	rjmp .L39	 ; 
	in r24,76-32	 ;  iftmp.7,,
	in r25,(76)+1-32	 ;  iftmp.7,,
	cp r24,r20	 ;  iftmp.7, temp
	cpc r25,r21	 ;  iftmp.7, temp
	brlo .L40	 ; ,
.L39:
	ld r24,Z	 ;  D.1895, PWR_OFF
	out 59-32,r24	 ; ,, D.1895
.L40:
	sbic 43-32,0	 ; ,,
	rjmp .L53	 ; 
	in r24,76-32	 ;  iftmp.10,,
	in r25,(76)+1-32	 ;  iftmp.10,,
	cp r24,r18	 ;  iftmp.10, TargetRPM.5
	cpc r25,r19	 ;  iftmp.10, TargetRPM.5
	brlo .L42	 ; ,
	rjmp .L53	 ; 
.L38:
	lds r18,NextPower	 ;  NextPower.11, NextPower
	lds r19,(NextPower)+1	 ;  NextPower.11, NextPower
	sts (Power)+1,r19	 ;  Power, NextPower.11
	sts Power,r18	 ;  Power, NextPower.11
	lds r24,Status	 ;  Status, Status
	lds r30,Step	 ;  Step, Step
	tst r24	 ;  Status
	breq .L44	 ; ,
	ldi r31,lo8(0)	 ;  Step,
	subi r30,lo8(-(PWR_ON))	 ;  Step,
	sbci r31,hi8(-(PWR_ON))	 ;  Step,
	rjmp .L52	 ; 
.L44:
	ldi r31,lo8(0)	 ;  Step,
	subi r30,lo8(-(PWR_OFF))	 ;  Step,
	sbci r31,hi8(-(PWR_OFF))	 ;  Step,
.L52:
	ld r24,Z	 ;  temp.81,* Step
	out 59-32,r24	 ; ,, temp.81
	cpi r18,200	 ;  NextPower.11,
	cpc r19,__zero_reg__	 ;  NextPower.11
	brsh .L46	 ; ,
	lds r24,rpm	 ;  rpm, rpm
	lds r25,(rpm)+1	 ;  rpm, rpm
	subi r24,lo8(501)	 ;  rpm,
	sbci r25,hi8(501)	 ;  rpm,
	brlo .L46	 ; ,
	rcall _Z5waitav	 ; 
.L53:
	rcall _Z3adjv	 ; 
	rjmp .L43	 ; 
.L46:
	rcall _Z3adjv	 ; 
	rcall _Z5waitav	 ; 
.L43:
	lds r30,Step	 ;  Step, Step
	ldi r31,lo8(0)	 ;  temp.83,
	movw r26,r30	 ;  tmp96, temp.83
	subi r26,lo8(-(PWR_OFF))	 ;  tmp96,
	sbci r27,hi8(-(PWR_OFF))	 ;  tmp96,
	ld r24,X	 ;  temp.84, PWR_OFF
	out 59-32,r24	 ; ,, temp.84
	subi r30,lo8(-(NextStep))	 ;  temp.83,
	sbci r31,hi8(-(NextStep))	 ;  temp.83,
	ld r24,Z	 ;  tmp100, NextStep
	sts Step,r24	 ;  Step, tmp100
	sbis 43-32,0	 ; ,,
	rjmp .L47	 ; 
	ldi r24,lo8(-1)	 ;  iftmp.16,
	ldi r25,hi8(-1)	 ;  iftmp.16,
	rjmp .L48	 ; 
.L47:
	in r24,76-32	 ;  iftmp.16,,
	in r25,(76)+1-32	 ;  iftmp.16,,
.L48:
	sts (rpm)+1,r25	 ;  rpm, iftmp.16
	sts rpm,r24	 ;  rpm, iftmp.16
	rjmp .L49	 ; 
	.size	_Z4loopv, .-_Z4loopv
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
	ldi r24,lo8(-128)	 ;  tmp50,
	out 70-32,r24	 ; ,, tmp50
	out 70-32,__zero_reg__	 ; ,,
	out 79-32,__zero_reg__	 ; ,,
	ldi r24,lo8(2)	 ;  tmp54,
	out 78-32,r24	 ; ,, tmp54
	out 66-32,__zero_reg__	 ; ,,
	out 44-32,__zero_reg__	 ; ,,
	ldi r24,lo8(7)	 ;  tmp58,
	out 39-32,r24	 ; ,, tmp58
	out 35-32,__zero_reg__	 ; ,,
	ldi r24,lo8(-57)	 ;  tmp61,
	out 38-32,r24	 ; ,, tmp61
	out 58-32,__zero_reg__	 ; ,,
	out 59-32,__zero_reg__	 ; ,,
	out 55-32,__zero_reg__	 ; ,,
	out 56-32,__zero_reg__	 ; ,,
	ldi r18,lo8(0)	 ;  cnt,
	ldi r19,hi8(0)	 ;  cnt,
.L58:
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	sbi 43-32,0	 ; ,,
.L56:
	sbic 43-32,0	 ; ,,
	rjmp .L55	 ; 
	in r24,76-32	 ;  iftmp.3,,
	in r25,(76)+1-32	 ;  iftmp.3,,
	subi r24,lo8(1000)	 ;  iftmp.3,
	sbci r25,hi8(1000)	 ;  iftmp.3,
	brlo .L56	 ; ,
.L55:
	sbic 57-32,6	 ; ,,
	rjmp .L57	 ; 
	ldi r18,lo8(0)	 ;  cnt,
	ldi r19,hi8(0)	 ;  cnt,
.L57:
	subi r18,lo8(-(1))	 ;  cnt,
	sbci r19,hi8(-(1))	 ;  cnt,
	ldi r24,hi8(1000)	 ; ,
	cpi r18,lo8(1000)	 ;  cnt,
	cpc r19,r24	 ;  cnt,
	brlt .L58	 ; ,
.L61:
	sbic 57-32,6	 ; ,,
	rjmp .L61	 ; 
	ldi r24,lo8(63)	 ;  tmp80,
	out 58-32,r24	 ; ,, tmp80
	ldi r24,lo8(42)	 ;  tmp82,
	out 59-32,r24	 ; ,, tmp82
	out 55-32,__zero_reg__	 ; ,,
	out 56-32,__zero_reg__	 ; ,,
	rcall _Z4loopv	 ; 
	ldi r24,lo8(0)	 ; ,
	ldi r25,hi8(0)	 ; ,
/* epilogue start */
	ret
	.size	main, .-main
.global	NextStep
	.data
	.type	NextStep, @object
	.size	NextStep, 6
NextStep:
	.byte	1
	.byte	2
	.byte	3
	.byte	4
	.byte	5
	.byte	0
.global	PWR_ON
	.type	PWR_ON, @object
	.size	PWR_ON, 6
PWR_ON:
	.byte	50
	.byte	56
	.byte	44
	.byte	14
	.byte	11
	.byte	35
.global	PWR_OFF
	.type	PWR_OFF, @object
	.size	PWR_OFF, 6
PWR_OFF:
	.byte	34
	.byte	40
	.byte	40
	.byte	10
	.byte	10
	.byte	34
.global	DigitRead
	.type	DigitRead, @object
	.size	DigitRead, 6
DigitRead:
	.byte	4
	.byte	2
	.byte	1
	.byte	4
	.byte	2
	.byte	1
.global	DigitReadBaseVal
	.type	DigitReadBaseVal, @object
	.size	DigitReadBaseVal, 6
DigitReadBaseVal:
	.byte	4
	.byte	0
	.byte	1
	.byte	0
	.byte	2
	.byte	0
.global	PWR_Table1
	.section	.progmem.data,"a",@progbits
	.type	PWR_Table1, @object
	.size	PWR_Table1, 512
PWR_Table1:
	.word	-15536
	.word	-17536
	.word	-19536
	.word	-21536
	.word	-23536
	.word	-25536
	.word	-27536
	.word	-29536
	.word	-31536
	.word	32000
	.word	30000
	.word	29000
	.word	28000
	.word	27000
	.word	26000
	.word	25000
	.word	24000
	.word	23000
	.word	22000
	.word	21000
	.word	20000
	.word	19000
	.word	18000
	.word	17000
	.word	16000
	.word	15000
	.word	14500
	.word	14000
	.word	13500
	.word	13000
	.word	12500
	.word	12000
	.word	11500
	.word	11000
	.word	10500
	.word	10000
	.word	9000
	.word	8000
	.word	7000
	.word	6000
	.word	5000
	.word	4000
	.word	3000
	.word	2000
	.word	1456
	.word	1424
	.word	1394
	.word	1365
	.word	1337
	.word	1310
	.word	1285
	.word	1260
	.word	1236
	.word	1213
	.word	1191
	.word	1170
	.word	1149
	.word	1130
	.word	1110
	.word	1092
	.word	1074
	.word	1057
	.word	1040
	.word	1024
	.word	1008
	.word	993
	.word	978
	.word	963
	.word	949
	.word	936
	.word	923
	.word	910
	.word	897
	.word	885
	.word	873
	.word	862
	.word	851
	.word	840
	.word	829
	.word	819
	.word	809
	.word	799
	.word	789
	.word	780
	.word	771
	.word	762
	.word	753
	.word	744
	.word	736
	.word	728
	.word	720
	.word	712
	.word	704
	.word	697
	.word	690
	.word	682
	.word	675
	.word	668
	.word	662
	.word	655
	.word	649
	.word	642
	.word	636
	.word	630
	.word	624
	.word	618
	.word	612
	.word	606
	.word	601
	.word	595
	.word	590
	.word	585
	.word	580
	.word	575
	.word	570
	.word	565
	.word	560
	.word	555
	.word	550
	.word	546
	.word	541
	.word	537
	.word	532
	.word	528
	.word	524
	.word	520
	.word	516
	.word	512
	.word	508
	.word	504
	.word	500
	.word	496
	.word	492
	.word	489
	.word	485
	.word	482
	.word	478
	.word	475
	.word	471
	.word	468
	.word	464
	.word	461
	.word	458
	.word	455
	.word	452
	.word	449
	.word	445
	.word	442
	.word	439
	.word	437
	.word	434
	.word	431
	.word	428
	.word	425
	.word	422
	.word	420
	.word	417
	.word	414
	.word	412
	.word	409
	.word	407
	.word	404
	.word	402
	.word	399
	.word	397
	.word	394
	.word	392
	.word	390
	.word	387
	.word	385
	.word	383
	.word	381
	.word	378
	.word	376
	.word	374
	.word	372
	.word	370
	.word	368
	.word	366
	.word	364
	.word	362
	.word	360
	.word	358
	.word	356
	.word	354
	.word	352
	.word	350
	.word	348
	.word	346
	.word	345
	.word	343
	.word	341
	.word	339
	.word	337
	.word	336
	.word	334
	.word	332
	.word	331
	.word	329
	.word	327
	.word	326
	.word	324
	.word	322
	.word	321
	.word	319
	.word	318
	.word	316
	.word	315
	.word	313
	.word	312
	.word	310
	.word	309
	.word	307
	.word	306
	.word	304
	.word	303
	.word	302
	.word	300
	.word	299
	.word	298
	.word	296
	.word	295
	.word	293
	.word	292
	.word	291
	.word	290
	.word	288
	.word	287
	.word	286
	.word	285
	.word	283
	.word	282
	.word	281
	.word	280
	.word	278
	.word	277
	.word	276
	.word	275
	.word	274
	.word	273
	.word	272
	.word	270
	.word	269
	.word	268
	.word	267
	.word	266
	.word	265
	.word	264
	.word	263
	.word	262
	.word	261
	.word	260
	.word	259
	.word	258
	.word	257
	.word	256
.global	PWR_Table2
	.type	PWR_Table2, @object
	.size	PWR_Table2, 256
PWR_Table2:
	.byte	-1
	.byte	-4
	.byte	-7
	.byte	-10
	.byte	-13
	.byte	-16
	.byte	-19
	.byte	-21
	.byte	-24
	.byte	-26
	.byte	-29
	.byte	-31
	.byte	-34
	.byte	-36
	.byte	-38
	.byte	-40
	.byte	-43
	.byte	-45
	.byte	-47
	.byte	-49
	.byte	-51
	.byte	-53
	.byte	-55
	.byte	-57
	.byte	-59
	.byte	-60
	.byte	-62
	.byte	-64
	.byte	-66
	.byte	-67
	.byte	-69
	.byte	-71
	.byte	-72
	.byte	-74
	.byte	-75
	.byte	-77
	.byte	-79
	.byte	-80
	.byte	-81
	.byte	-83
	.byte	-84
	.byte	-86
	.byte	-87
	.byte	-88
	.byte	-90
	.byte	-91
	.byte	-92
	.byte	-94
	.byte	-95
	.byte	-96
	.byte	-97
	.byte	-98
	.byte	-100
	.byte	-101
	.byte	-102
	.byte	-103
	.byte	-104
	.byte	-105
	.byte	-106
	.byte	-107
	.byte	-108
	.byte	-109
	.byte	-110
	.byte	-111
	.byte	-112
	.byte	-113
	.byte	-114
	.byte	-115
	.byte	-116
	.byte	-117
	.byte	-118
	.byte	-119
	.byte	-120
	.byte	-121
	.byte	-122
	.byte	-122
	.byte	-123
	.byte	-124
	.byte	-125
	.byte	-126
	.byte	-127
	.byte	-127
	.byte	-128
	.byte	127
	.byte	126
	.byte	126
	.byte	125
	.byte	124
	.byte	123
	.byte	123
	.byte	122
	.byte	121
	.byte	121
	.byte	120
	.byte	119
	.byte	119
	.byte	118
	.byte	117
	.byte	117
	.byte	116
	.byte	115
	.byte	115
	.byte	114
	.byte	113
	.byte	113
	.byte	112
	.byte	112
	.byte	111
	.byte	110
	.byte	110
	.byte	109
	.byte	109
	.byte	108
	.byte	108
	.byte	107
	.byte	106
	.byte	106
	.byte	105
	.byte	105
	.byte	104
	.byte	104
	.byte	103
	.byte	103
	.byte	102
	.byte	102
	.byte	101
	.byte	101
	.byte	100
	.byte	100
	.byte	99
	.byte	99
	.byte	98
	.byte	98
	.byte	98
	.byte	97
	.byte	97
	.byte	96
	.byte	96
	.byte	95
	.byte	95
	.byte	95
	.byte	94
	.byte	94
	.byte	93
	.byte	93
	.byte	92
	.byte	92
	.byte	92
	.byte	91
	.byte	91
	.byte	90
	.byte	90
	.byte	90
	.byte	89
	.byte	89
	.byte	89
	.byte	88
	.byte	88
	.byte	87
	.byte	87
	.byte	87
	.byte	86
	.byte	86
	.byte	86
	.byte	85
	.byte	85
	.byte	85
	.byte	84
	.byte	84
	.byte	84
	.byte	83
	.byte	83
	.byte	83
	.byte	82
	.byte	82
	.byte	82
	.byte	81
	.byte	81
	.byte	81
	.byte	80
	.byte	80
	.byte	80
	.byte	79
	.byte	79
	.byte	79
	.byte	79
	.byte	78
	.byte	78
	.byte	78
	.byte	77
	.byte	77
	.byte	77
	.byte	77
	.byte	76
	.byte	76
	.byte	76
	.byte	75
	.byte	75
	.byte	75
	.byte	75
	.byte	74
	.byte	74
	.byte	74
	.byte	74
	.byte	73
	.byte	73
	.byte	73
	.byte	73
	.byte	72
	.byte	72
	.byte	72
	.byte	72
	.byte	71
	.byte	71
	.byte	71
	.byte	71
	.byte	70
	.byte	70
	.byte	70
	.byte	70
	.byte	69
	.byte	69
	.byte	69
	.byte	69
	.byte	69
	.byte	68
	.byte	68
	.byte	68
	.byte	68
	.byte	67
	.byte	67
	.byte	67
	.byte	67
	.byte	67
	.byte	66
	.byte	66
	.byte	66
	.byte	66
	.byte	66
	.byte	65
	.byte	65
	.byte	65
	.byte	65
	.byte	64
	.byte	64
	.byte	64
	.byte	64
	.byte	64
	.byte	64
	.byte	63
	.byte	63
	.byte	50
	.byte	40
	.byte	30
	.byte	20
	.byte	10
.global	Step
.global	Step
	.section .bss
	.type	Step, @object
	.size	Step, 1
Step:
	.skip 1,0
.global	Status
.global	Status
	.type	Status, @object
	.size	Status, 1
Status:
	.skip 1,0
.global	StartUpCount1
.global	StartUpCount1
	.type	StartUpCount1, @object
	.size	StartUpCount1, 1
StartUpCount1:
	.skip 1,0
.global	TargetRPM
.global	TargetRPM
	.type	TargetRPM, @object
	.size	TargetRPM, 2
TargetRPM:
	.skip 2,0
.global	rpm
.global	rpm
	.type	rpm, @object
	.size	rpm, 2
rpm:
	.skip 2,0
.global	startupCurrent
.global	startupCurrent
	.type	startupCurrent, @object
	.size	startupCurrent, 2
startupCurrent:
	.skip 2,0
.global	Power
.global	Power
	.type	Power, @object
	.size	Power, 2
Power:
	.skip 2,0
.global	NextPower
.global	NextPower
	.type	NextPower, @object
	.size	NextPower, 2
NextPower:
	.skip 2,0
