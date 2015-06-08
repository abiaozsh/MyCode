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
	push r28	 ; 
	push r29	 ; 
/* prologue: function */
/* frame size = 0 */
	sbis 57-32,6	 ; ,,
	rjmp .L13	 ; 
	ldi r24,lo8(6)	 ;  tmp69,
	out 39-32,r24	 ; ,, tmp69
	ldi r24,lo8(16)	 ;  tmp71,
	out 35-32,r24	 ; ,, tmp71
	ldi r24,lo8(-57)	 ;  tmp73,
	out 38-32,r24	 ; ,, tmp73
.L14:
	in r24,38-32	 ;  D.1907,,
	andi r24,lo8(6)	 ;  D.1907,
	brne .L14	 ; ,
	in r22,37-32	 ;  aread,,
	ldi r18,lo8(7)	 ;  tmp79,
	out 39-32,r18	 ; ,, tmp79
	out 35-32,__zero_reg__	 ; ,,
	ldi r18,lo8(-57)	 ;  tmp82,
	out 38-32,r18	 ; ,, tmp82
	lds r28,TargetRPM	 ;  TargetRPM.10, TargetRPM
	lds r29,(TargetRPM)+1	 ;  TargetRPM.10, TargetRPM
	ldi r23,lo8(0)	 ;  aread,
	ldi r24,lo8(0)	 ;  aread,
	ldi r25,hi8(0)	 ;  aread,
	movw r18,r28	 ;  TargetRPM.10, TargetRPM.10
	ldi r20,lo8(0)	 ;  TargetRPM.10,
	ldi r21,hi8(0)	 ;  TargetRPM.10,
	rcall __mulsi3	 ; 
	mov r22,r23	 ;  tmp86, tmp85
	mov r23,r24	 ;  tmp86, tmp85
	mov r24,r25	 ;  tmp86, tmp85
	clr r25	 ;  tmp86
	lds r24,Step	 ;  Step, Step
	mov r30,r24	 ;  tmp124, Step
	ldi r31,lo8(0)	 ; ,
	subi r30,lo8(-(PWR_OFF))	 ;  tmp124,
	sbci r31,hi8(-(PWR_OFF))	 ;  tmp124,
.L18:
	sbic 43-32,0	 ; ,,
	rjmp .L15	 ; 
	in r24,76-32	 ;  iftmp.12,,
	in r25,(76)+1-32	 ;  iftmp.12,,
	cp r24,r22	 ;  iftmp.12, temp
	cpc r25,r23	 ;  iftmp.12, temp
	brlo .L16	 ; ,
.L15:
	ld r24,Z	 ;  D.1930, PWR_OFF
	out 59-32,r24	 ; ,, D.1930
.L16:
	sbic 43-32,0	 ; ,,
	rjmp .L25	 ; 
	in r24,76-32	 ;  iftmp.15,,
	in r25,(76)+1-32	 ;  iftmp.15,,
	cp r24,r28	 ;  iftmp.15, TargetRPM.10
	cpc r25,r29	 ;  iftmp.15, TargetRPM.10
	brlo .L18	 ; ,
	rjmp .L25	 ; 
.L13:
	lds r24,Step	 ;  Step, Step
	mov r26,r24	 ;  temp.59, Step
	ldi r27,lo8(0)	 ;  temp.59,
	movw r30,r26	 ;  tmp101, temp.59
	subi r30,lo8(-(DigitReadBaseVal))	 ;  tmp101,
	sbci r31,hi8(-(DigitReadBaseVal))	 ;  tmp101,
	ld r17,Z	 ;  valbase, DigitReadBaseVal
	lds r20,rpm	 ;  temp, rpm
	lds r21,(rpm)+1	 ;  temp, rpm
	lsr r21	 ;  temp
	ror r20	 ;  temp
	lds r22,Power	 ;  Power.20, Power
	lds r23,(Power)+1	 ;  Power.20, Power
	movw r30,r26	 ;  tmp126, temp.59
	subi r30,lo8(-(PWR_OFF))	 ;  tmp126,
	sbci r31,hi8(-(PWR_OFF))	 ;  tmp126,
	movw r28,r26	 ;  tmp127, temp.59
	subi r28,lo8(-(DigitRead))	 ;  tmp127,
	sbci r29,hi8(-(DigitRead))	 ;  tmp127,
.L29:
	sbis 57-32,6	 ; ,,
	rjmp .L19	 ; 
	subi r26,lo8(-(PWR_OFF))	 ;  temp.59,
	sbci r27,hi8(-(PWR_OFF))	 ;  temp.59,
	ld r24,X	 ;  temp.60, PWR_OFF
	out 59-32,r24	 ; ,, temp.60
	rjmp .L25	 ; 
.L19:
	sbis 43-32,0	 ; ,,
	rjmp .L20	 ; 
	ldi r24,lo8(-1)	 ;  iftmp.19,
	ldi r25,hi8(-1)	 ;  iftmp.19,
	rjmp .L21	 ; 
.L20:
	in r24,76-32	 ;  iftmp.19,,
	in r25,(76)+1-32	 ;  iftmp.19,,
.L21:
	cp r22,r24	 ;  Power.20, iftmp.19
	cpc r23,r25	 ;  Power.20, iftmp.19
	brsh .L22	 ; ,
	ld r24,Z	 ;  temp.61, PWR_OFF
	out 59-32,r24	 ; ,, temp.61
.L22:
	in r19,54-32	 ;  D.1953,,
	ld r18,Y	 ;  D.1954, DigitRead
	sbic 43-32,0	 ; ,,
	rjmp .L23	 ; 
	in r24,76-32	 ;  iftmp.23,,
	in r25,(76)+1-32	 ;  iftmp.23,,
	cp r24,r20	 ;  iftmp.23, temp
	cpc r25,r21	 ;  iftmp.23, temp
	brlo .L29	 ; ,
.L23:
	and r18,r19	 ;  D.1954, D.1953
	cp r18,r17	 ;  D.1954, valbase
	breq .L29	 ; ,
.L25:
/* epilogue start */
	pop r29	 ; 
	pop r28	 ; 
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
	sts (Power)+1,__zero_reg__	 ;  Power,
	sts Power,__zero_reg__	 ;  Power,
	in r24,36-32	 ;  aread,,
	in r25,(36)+1-32	 ;  aread,,
	sbi 38-32,6	 ; ,,
	ldi r18,hi8(512)	 ; ,
	cpi r24,lo8(512)	 ;  aread,
	cpc r25,r18	 ;  aread,
	brlo .L31	 ; ,
	ldi r30,lo8(512)	 ;  __addr16,
	ldi r31,hi8(512)	 ;  __addr16,
	sub r30,r24	 ;  __addr16, aread
	sbc r31,r25	 ;  __addr16, aread
	lsr r31	 ;  __addr16
	ror r30	 ;  __addr16
	subi r30,lo8(-(PWR_Table2))	 ;  __addr16,
	sbci r31,hi8(-(PWR_Table2))	 ;  __addr16,
/* #APP */
 ;  206 "motor6.cpp" 1
	lpm r24, Z+	 ;  __result
	lpm r25, Z	 ;  __result
	
 ;  0 "" 2
/* #NOAPP */
	rjmp .L43	 ; 
.L31:
	movw r30,r24	 ;  __addr16, aread
	andi r30,lo8(-2)	 ;  __addr16,
	subi r30,lo8(-(PWR_Table1))	 ;  __addr16,
	sbci r31,hi8(-(PWR_Table1))	 ;  __addr16,
/* #APP */
 ;  210 "motor6.cpp" 1
	lpm r24, Z+	 ;  __result
	lpm r25, Z	 ;  __result
	
 ;  0 "" 2
/* #NOAPP */
.L43:
	sts (TargetRPM)+1,r25	 ;  TargetRPM, __result
	sts TargetRPM,r24	 ;  TargetRPM, __result
	lds r24,Status	 ;  Status, Status
	lds r18,rpm	 ;  rpm.72, rpm
	lds r19,(rpm)+1	 ;  rpm.72, rpm
	cpi r24,lo8(1)	 ;  Status,
	breq .+2	 ; 
	rjmp .L33	 ; 
	ldi r20,hi8(11001)	 ; ,
	cpi r18,lo8(11001)	 ;  rpm.72,
	cpc r19,r20	 ;  rpm.72,
	brlo .L34	 ; ,
	sts StartUpCount1,__zero_reg__	 ;  StartUpCount1,
	sts StartUpCount2,__zero_reg__	 ;  StartUpCount2,
	sts Status,__zero_reg__	 ;  Status,
	rjmp .L42	 ; 
.L34:
	lds r22,TargetRPM	 ;  TargetRPM.26, TargetRPM
	lds r23,(TargetRPM)+1	 ;  TargetRPM.26, TargetRPM
	cp r22,r18	 ;  TargetRPM.26, rpm.72
	cpc r23,r19	 ;  TargetRPM.26, rpm.72
	brlo .+2	 ; 
	rjmp .L42	 ; 
	cpi r18,201	 ;  rpm.72,
	cpc r19,__zero_reg__	 ;  rpm.72
	brlo .L36	 ; ,
	ldi r24,lo8(0)	 ;  temp,
	ldi r25,hi8(0)	 ;  temp,
	movw r24,r22	 ;  temp.67, temp
	clr r23	 ;  temp.67
	clr r22	 ;  temp.67
	movw r10,r18	 ;  D.1985, rpm.72
	clr r12	 ;  D.1985
	clr r13	 ;  D.1985
	movw r20,r12	 ; , D.1985
	movw r18,r10	 ; , D.1985
	rcall __udivmodsi4
	movw r14,r18	 ; ,
	movw r16,r20	 ; ,
	ldi r22,lo8(65536)	 ;  tmp70,
	ldi r23,hi8(65536)	 ;  tmp70,
	ldi r24,hlo8(65536)	 ;  tmp70,
	ldi r25,hhi8(65536)	 ;  tmp70,
	sub r22,r14	 ;  tmp70, temp.68
	sbc r23,r15	 ;  tmp70, temp.68
	sbc r24,r16	 ;  tmp70, temp.68
	sbc r25,r17	 ;  tmp70, temp.68
	movw r20,r12	 ; , D.1985
	movw r18,r10	 ; , D.1985
	rcall __mulsi3	 ; 
	ldi r21,15	 ; ,
1:	lsr r25	 ;  tmp71
	ror r24	 ;  tmp71
	ror r23	 ;  tmp71
	ror r22	 ;  tmp71
	dec r21	 ; 
	brne 1b
	sts (Power)+1,r23	 ;  Power, tmp71
	sts Power,r22	 ;  Power, tmp71
	rjmp .L42	 ; 
.L36:
	ldi r24,lo8(1000)	 ;  tmp73,
	ldi r25,hi8(1000)	 ;  tmp73,
	sts (Power)+1,r25	 ;  Power, tmp73
	sts Power,r24	 ;  Power, tmp73
	rjmp .L42	 ; 
.L33:
	movw r24,r18	 ;  tmp74, rpm.72
	subi r24,lo8(-(-1501))	 ;  tmp74,
	sbci r25,hi8(-(-1501))	 ;  tmp74,
	subi r24,lo8(10499)	 ;  tmp74,
	sbci r25,hi8(10499)	 ;  tmp74,
	brsh .L37	 ; ,
	lds r24,StartUpCount1	 ;  StartUpCount1, StartUpCount1
	subi r24,lo8(-(1))	 ;  StartUpCount1,
	sts StartUpCount1,r24	 ;  StartUpCount1, StartUpCount1
	rjmp .L38	 ; 
.L37:
	sts StartUpCount1,__zero_reg__	 ;  StartUpCount1,
	sts StartUpCount2,__zero_reg__	 ;  StartUpCount2,
.L38:
	lds r24,StartUpCount1	 ;  StartUpCount1, StartUpCount1
	cpi r24,lo8(21)	 ;  StartUpCount1,
	brlo .L39	 ; ,
	movw r24,r18	 ;  tmp78, rpm.72
	lsr r25	 ;  tmp78
	ror r24	 ;  tmp78
	lsr r25	 ;  tmp78
	ror r24	 ;  tmp78
	sts (Power)+1,r25	 ;  Power, tmp78
	sts Power,r24	 ;  Power, tmp78
.L39:
	subi r18,lo8(-(-1376))	 ;  rpm.72,
	sbci r19,hi8(-(-1376))	 ;  rpm.72,
	subi r18,lo8(9624)	 ;  rpm.72,
	sbci r19,hi8(9624)	 ;  rpm.72,
	brsh .L40	 ; ,
	lds r24,StartUpCount2	 ;  StartUpCount2, StartUpCount2
	subi r24,lo8(-(1))	 ;  StartUpCount2,
	sts StartUpCount2,r24	 ;  StartUpCount2, StartUpCount2
	rjmp .L41	 ; 
.L40:
	sts StartUpCount2,__zero_reg__	 ;  StartUpCount2,
.L41:
	lds r24,StartUpCount2	 ;  StartUpCount2, StartUpCount2
	cpi r24,lo8(21)	 ;  StartUpCount2,
	brlo .L42	 ; ,
	ldi r24,lo8(1)	 ;  tmp83,
	sts Status,r24	 ;  Status, tmp83
.L42:
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
	lds r24,Power	 ;  Power, Power
	lds r25,(Power)+1	 ;  Power, Power
	lds r30,Step	 ;  Step, Step
	or r24,r25	 ;  Power
	breq .L45	 ; ,
	ldi r31,lo8(0)	 ;  Step,
	subi r30,lo8(-(PWR_ON))	 ;  Step,
	sbci r31,hi8(-(PWR_ON))	 ;  Step,
	rjmp .L51	 ; 
.L45:
	ldi r31,lo8(0)	 ;  Step,
	subi r30,lo8(-(PWR_OFF))	 ;  Step,
	sbci r31,hi8(-(PWR_OFF))	 ;  Step,
.L51:
	ld r24,Z	 ;  D.1881,* Step
	out 59-32,r24	 ; ,, D.1881
	rcall _Z3adjv	 ; 
	rcall _Z5waitav	 ; 
	lds r30,Step	 ;  Step, Step
	ldi r31,lo8(0)	 ;  temp.83,
	movw r26,r30	 ;  tmp67, temp.83
	subi r26,lo8(-(PWR_OFF))	 ;  tmp67,
	sbci r27,hi8(-(PWR_OFF))	 ;  tmp67,
	ld r24,X	 ;  temp.84, PWR_OFF
	out 59-32,r24	 ; ,, temp.84
	subi r30,lo8(-(NextStep))	 ;  temp.83,
	sbci r31,hi8(-(NextStep))	 ;  temp.83,
	ld r24,Z	 ;  tmp71, NextStep
	sts Step,r24	 ;  Step, tmp71
	sbis 43-32,0	 ; ,,
	rjmp .L47	 ; 
	ldi r24,lo8(-1)	 ;  iftmp.7,
	ldi r25,hi8(-1)	 ;  iftmp.7,
	rjmp .L48	 ; 
.L47:
	in r24,76-32	 ;  iftmp.7,,
	in r25,(76)+1-32	 ;  iftmp.7,,
.L48:
	sts (rpm)+1,r25	 ;  rpm, iftmp.7
	sts rpm,r24	 ;  rpm, iftmp.7
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
.L56:
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	sbi 43-32,0	 ; ,,
.L54:
	sbic 43-32,0	 ; ,,
	rjmp .L53	 ; 
	in r24,76-32	 ;  iftmp.3,,
	in r25,(76)+1-32	 ;  iftmp.3,,
	subi r24,lo8(1000)	 ;  iftmp.3,
	sbci r25,hi8(1000)	 ;  iftmp.3,
	brlo .L54	 ; ,
.L53:
	sbic 57-32,6	 ; ,,
	rjmp .L55	 ; 
	ldi r18,lo8(0)	 ;  cnt,
	ldi r19,hi8(0)	 ;  cnt,
.L55:
	subi r18,lo8(-(1))	 ;  cnt,
	sbci r19,hi8(-(1))	 ;  cnt,
	ldi r24,hi8(1000)	 ; ,
	cpi r18,lo8(1000)	 ;  cnt,
	cpc r19,r24	 ;  cnt,
	brlt .L56	 ; ,
.L59:
	sbic 57-32,6	 ; ,,
	rjmp .L59	 ; 
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
	.word	-1434
	.word	32413
	.word	21690
	.word	16298
	.word	13053
	.word	10886
	.word	9336
	.word	8172
	.word	7266
	.word	6541
	.word	5948
	.word	5453
	.word	5034
	.word	4675
	.word	4364
	.word	4092
	.word	3851
	.word	3637
	.word	3446
	.word	3274
	.word	3118
	.word	2977
	.word	2847
	.word	2729
	.word	2620
	.word	2519
	.word	2426
	.word	2339
	.word	2259
	.word	2183
	.word	2113
	.word	2047
	.word	1985
	.word	1927
	.word	1872
	.word	1820
	.word	1770
	.word	1724
	.word	1680
	.word	1638
	.word	1598
	.word	1560
	.word	1524
	.word	1489
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
	.byte	63
	.byte	60
	.byte	50
	.byte	40
	.byte	30
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
.global	StartUpCount2
.global	StartUpCount2
	.type	StartUpCount2, @object
	.size	StartUpCount2, 1
StartUpCount2:
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
