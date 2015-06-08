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
	out 79-32,__zero_reg__	 ; ,,
	ldi r24,lo8(2)	 ;  tmp48,
	out 78-32,r24	 ; ,, tmp48
	out 66-32,__zero_reg__	 ; ,,
	out 44-32,__zero_reg__	 ; ,,
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	sbi 43-32,0	 ; ,,
.L3:
	sbic 43-32,0	 ; ,,
	rjmp .L4	 ; 
	in r24,76-32	 ;  iftmp.1,,
	in r25,(76)+1-32	 ;  iftmp.1,,
	cp r24,r18	 ;  iftmp.1, ticks
	cpc r25,r19	 ;  iftmp.1, ticks
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
.global	_Z12AnalogReadOnh
	.type	_Z12AnalogReadOnh, @function
_Z12AnalogReadOnh:
/* prologue: function */
/* frame size = 0 */
	mov r18,r24	 ;  pin, pin
	sbi 56-32,1	 ; ,,
	out 79-32,__zero_reg__	 ; ,,
	ldi r24,lo8(2)	 ;  tmp59,
	out 78-32,r24	 ; ,, tmp59
	out 66-32,__zero_reg__	 ; ,,
	out 44-32,__zero_reg__	 ; ,,
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	sbi 43-32,0	 ; ,,
.L12:
	sbic 43-32,0	 ; ,,
	rjmp .L11	 ; 
	in r24,76-32	 ;  iftmp.1,,
	in r25,(76)+1-32	 ;  iftmp.1,,
	cpi r24,100	 ;  iftmp.1,
	cpc r25,__zero_reg__	 ;  iftmp.1
	brlo .L12	 ; ,
.L11:
	out 39-32,r18	 ; ,, pin
	ldi r24,lo8(16)	 ;  tmp71,
	out 35-32,r24	 ; ,, tmp71
	ldi r24,lo8(-121)	 ;  tmp73,
	out 38-32,r24	 ; ,, tmp73
	sbi 38-32,6	 ; ,,
.L13:
	sbic 38-32,6	 ; ,,
	rjmp .L13	 ; 
	in r24,37-32	 ;  aread,,
	out 38-32,__zero_reg__	 ; ,,
	cbi 56-32,1	 ; ,,
/* epilogue start */
	ret
	.size	_Z12AnalogReadOnh, .-_Z12AnalogReadOnh
.global	_Z13AnalogReadOffh
	.type	_Z13AnalogReadOffh, @function
_Z13AnalogReadOffh:
/* prologue: function */
/* frame size = 0 */
	mov r18,r24	 ;  pin, pin
	out 79-32,__zero_reg__	 ; ,,
	ldi r24,lo8(2)	 ;  tmp53,
	out 78-32,r24	 ; ,, tmp53
	out 66-32,__zero_reg__	 ; ,,
	out 44-32,__zero_reg__	 ; ,,
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	sbi 43-32,0	 ; ,,
.L19:
	sbic 43-32,0	 ; ,,
	rjmp .L18	 ; 
	in r24,76-32	 ;  iftmp.1,,
	in r25,(76)+1-32	 ;  iftmp.1,,
	cpi r24,100	 ;  iftmp.1,
	cpc r25,__zero_reg__	 ;  iftmp.1
	brlo .L19	 ; ,
.L18:
	out 39-32,r18	 ; ,, pin
	ldi r24,lo8(16)	 ;  tmp65,
	out 35-32,r24	 ; ,, tmp65
	ldi r24,lo8(-121)	 ;  tmp67,
	out 38-32,r24	 ; ,, tmp67
	sbi 38-32,6	 ; ,,
.L20:
	sbic 38-32,6	 ; ,,
	rjmp .L20	 ; 
	in r24,37-32	 ;  aread,,
	out 38-32,__zero_reg__	 ; ,,
/* epilogue start */
	ret
	.size	_Z13AnalogReadOffh, .-_Z13AnalogReadOffh
.global	_Z4loopv
	.type	_Z4loopv, @function
_Z4loopv:
	push r14	 ; 
	push r15	 ; 
	push r16	 ; 
	push r17	 ; 
/* prologue: function */
/* frame size = 0 */
	ldi r16,lo8(2)	 ;  tmp191,
	clr r15	 ;  tmp192
	inc r15	 ;  tmp192
	ldi r24,lo8(3)	 ; ,
	mov r14,r24	 ;  tmp193,
.L65:
	lds r24,Status	 ;  Status, Status
	tst r24	 ;  Status
	brne .L25	 ; ,
	ldi r24,lo8(7)	 ; ,
	rcall _Z13AnalogReadOffh	 ; 
	cpi r24,lo8(64)	 ; ,
	brsh .L26	 ; ,
	sts Status,r15	 ;  Status, tmp192
.L26:
	out 79-32,__zero_reg__	 ; ,,
	out 78-32,r16	 ; ,, tmp191
	out 66-32,__zero_reg__	 ; ,,
	out 44-32,__zero_reg__	 ; ,,
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	sbi 43-32,0	 ; ,,
.L27:
	sbic 43-32,0	 ; ,,
	rjmp .L25	 ; 
	in r24,76-32	 ;  iftmp.1,,
	in r25,(76)+1-32	 ;  iftmp.1,,
	subi r24,lo8(1000)	 ;  iftmp.1,
	sbci r25,hi8(1000)	 ;  iftmp.1,
	brlo .L27	 ; ,
.L25:
	lds r24,Status	 ;  Status, Status
	cpi r24,lo8(1)	 ;  Status,
	breq .+2	 ; 
	rjmp .L28	 ; 
	ldi r24,lo8(7)	 ; ,
	rcall _Z13AnalogReadOffh	 ; 
	cpi r24,lo8(65)	 ; ,
	brlo .L29	 ; ,
	sts Status,__zero_reg__	 ;  Status,
.L29:
	ldi r24,lo8(0)	 ; ,
	rcall _Z13AnalogReadOffh	 ; 
	mov r17,r24	 ;  valBase,
	ldi r24,lo8(0)	 ; ,
	rcall _Z12AnalogReadOnh	 ; 
	cp r17,r24	 ;  valBase, val.54
	brsh .L30	 ; ,
	ldi r25,lo8(0)	 ;  val.54,
	sub r24,r17	 ;  val.54, valBase
	sbc r25,__zero_reg__	 ;  val.54
	sbiw r24,41	 ;  val.54,
	brge .L31	 ; ,
.L30:
	sts Status,r16	 ;  Status, tmp191
	sbi 56-32,0	 ; ,,
.L64:
	ldi r24,lo8(0)	 ; ,
	rcall _Z13AnalogReadOffh	 ; 
	mov r17,r24	 ;  valBase.65,
	ldi r24,lo8(0)	 ; ,
	rcall _Z12AnalogReadOnh	 ; 
	cp r17,r24	 ;  valBase.65, val.66
	brsh .L64	 ; ,
	ldi r25,lo8(0)	 ;  val.66,
	sub r24,r17	 ;  val.66, valBase.65
	sbc r25,__zero_reg__	 ;  val.66
	sbiw r24,41	 ;  val.66,
	brlt .L64	 ; ,
	rjmp .L65	 ; 
.L31:
	ldi r24,lo8(2)	 ; ,
	rcall _Z13AnalogReadOffh	 ; 
	mov r17,r24	 ;  valBase.55,
	ldi r24,lo8(2)	 ; ,
	rcall _Z12AnalogReadOnh	 ; 
	cp r17,r24	 ;  valBase.55, val.56
	brsh .L34	 ; ,
	ldi r25,lo8(0)	 ;  val.56,
	sub r24,r17	 ;  val.56, valBase.55
	sbc r25,__zero_reg__	 ;  val.56
	sbiw r24,41	 ;  val.56,
	brge .L35	 ; ,
.L34:
	sts Status,r14	 ;  Status, tmp193
	sbi 56-32,0	 ; ,,
.L66:
	ldi r24,lo8(2)	 ; ,
	rcall _Z13AnalogReadOffh	 ; 
	mov r17,r24	 ;  valBase.60,
	ldi r24,lo8(2)	 ; ,
	rcall _Z12AnalogReadOnh	 ; 
	cp r17,r24	 ;  valBase.60, val.61
	brsh .L66	 ; ,
	ldi r25,lo8(0)	 ;  val.61,
	sub r24,r17	 ;  val.61, valBase.60
	sbc r25,__zero_reg__	 ;  val.61
	sbiw r24,41	 ;  val.61,
	brlt .L66	 ; ,
	rjmp .L65	 ; 
.L35:
	out 79-32,__zero_reg__	 ; ,,
	out 78-32,r16	 ; ,, tmp191
	out 66-32,__zero_reg__	 ; ,,
	out 44-32,__zero_reg__	 ; ,,
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	sbi 43-32,0	 ; ,,
.L37:
	sbic 43-32,0	 ; ,,
	rjmp .L28	 ; 
	in r24,76-32	 ;  iftmp.1,,
	in r25,(76)+1-32	 ;  iftmp.1,,
	subi r24,lo8(1000)	 ;  iftmp.1,
	sbci r25,hi8(1000)	 ;  iftmp.1,
	brlo .L37	 ; ,
.L28:
	lds r24,Status	 ;  Status, Status
	cpi r24,lo8(2)	 ;  Status,
	brne .L38	 ; ,
	rcall _Z13AnalogReadOffh	 ; 
	mov r17,r24	 ;  valBase,
	ldi r24,lo8(2)	 ; ,
	rcall _Z12AnalogReadOnh	 ; 
	cp r17,r24	 ;  valBase, val
	brsh .L39	 ; ,
	ldi r25,lo8(0)	 ;  val,
	sub r24,r17	 ;  val, valBase
	sbc r25,__zero_reg__	 ;  val
	sbiw r24,41	 ;  val,
	brge .L38	 ; ,
.L39:
	sts Status,r15	 ;  Status, tmp192
	cbi 56-32,0	 ; ,,
	out 79-32,__zero_reg__	 ; ,,
	out 78-32,r16	 ; ,, tmp191
	out 66-32,__zero_reg__	 ; ,,
	out 44-32,__zero_reg__	 ; ,,
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	sbi 43-32,0	 ; ,,
.L41:
	sbic 43-32,0	 ; ,,
	rjmp .L63	 ; 
	in r24,76-32	 ;  iftmp.1,,
	in r25,(76)+1-32	 ;  iftmp.1,,
	subi r24,lo8(-5536)	 ;  iftmp.1,
	sbci r25,hi8(-5536)	 ;  iftmp.1,
	brlo .L41	 ; ,
.L63:
	ldi r24,lo8(2)	 ; ,
	rcall _Z13AnalogReadOffh	 ; 
	mov r17,r24	 ;  valBase.71,
	ldi r24,lo8(2)	 ; ,
	rcall _Z12AnalogReadOnh	 ; 
	cp r17,r24	 ;  valBase.71, val.72
	brsh .L63	 ; ,
	ldi r25,lo8(0)	 ;  val.72,
	sub r24,r17	 ;  val.72, valBase.71
	sbc r25,__zero_reg__	 ;  val.72
	sbiw r24,41	 ;  val.72,
	brlt .L63	 ; ,
.L38:
	lds r24,Status	 ;  Status, Status
	cpi r24,lo8(3)	 ;  Status,
	breq .+2	 ; 
	rjmp .L65	 ; 
	ldi r24,lo8(0)	 ; ,
	rcall _Z13AnalogReadOffh	 ; 
	mov r17,r24	 ;  valBase,
	ldi r24,lo8(0)	 ; ,
	rcall _Z12AnalogReadOnh	 ; 
	cp r17,r24	 ;  valBase, val
	brsh .L42	 ; ,
	ldi r25,lo8(0)	 ;  val,
	sub r24,r17	 ;  val, valBase
	sbc r25,__zero_reg__	 ;  val
	sbiw r24,41	 ;  val,
	brlt .+2	 ; 
	rjmp .L65	 ; 
.L42:
	sts Status,r15	 ;  Status, tmp192
	cbi 56-32,0	 ; ,,
	out 79-32,__zero_reg__	 ; ,,
	out 78-32,r16	 ; ,, tmp191
	out 66-32,__zero_reg__	 ; ,,
	out 44-32,__zero_reg__	 ; ,,
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	sbi 43-32,0	 ; ,,
.L44:
	sbic 43-32,0	 ; ,,
	rjmp .L62	 ; 
	in r24,76-32	 ;  iftmp.1,,
	in r25,(76)+1-32	 ;  iftmp.1,,
	subi r24,lo8(-5536)	 ;  iftmp.1,
	sbci r25,hi8(-5536)	 ;  iftmp.1,
	brlo .L44	 ; ,
.L62:
	ldi r24,lo8(0)	 ; ,
	rcall _Z13AnalogReadOffh	 ; 
	mov r17,r24	 ;  valBase.77,
	ldi r24,lo8(0)	 ; ,
	rcall _Z12AnalogReadOnh	 ; 
	cp r17,r24	 ;  valBase.77, val.78
	brsh .L62	 ; ,
	ldi r25,lo8(0)	 ;  val.78,
	sub r24,r17	 ;  val.78, valBase.77
	sbc r25,__zero_reg__	 ;  val.78
	sbiw r24,41	 ;  val.78,
	brlt .L62	 ; ,
	rjmp .L65	 ; 
	.size	_Z4loopv, .-_Z4loopv
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
	ldi r24,lo8(-128)	 ;  tmp47,
	out 70-32,r24	 ; ,, tmp47
	out 70-32,__zero_reg__	 ; ,,
	out 79-32,__zero_reg__	 ; ,,
	ldi r24,lo8(2)	 ;  tmp51,
	out 78-32,r24	 ; ,, tmp51
	out 66-32,__zero_reg__	 ; ,,
	out 44-32,__zero_reg__	 ; ,,
	out 58-32,__zero_reg__	 ; ,,
	out 59-32,__zero_reg__	 ; ,,
	out 55-32,__zero_reg__	 ; ,,
	out 56-32,__zero_reg__	 ; ,,
	out 79-32,__zero_reg__	 ; ,,
	out 78-32,r24	 ; ,, tmp51
	out 66-32,__zero_reg__	 ; ,,
	out 44-32,__zero_reg__	 ; ,,
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	sbi 43-32,0	 ; ,,
.L69:
	sbic 43-32,0	 ; ,,
	rjmp .L68	 ; 
	in r24,76-32	 ;  iftmp.1,,
	in r25,(76)+1-32	 ;  iftmp.1,,
	subi r24,lo8(1000)	 ;  iftmp.1,
	sbci r25,hi8(1000)	 ;  iftmp.1,
	brlo .L69	 ; ,
.L68:
	out 58-32,__zero_reg__	 ; ,,
	out 59-32,__zero_reg__	 ; ,,
	ldi r24,lo8(3)	 ;  tmp73,
	out 55-32,r24	 ; ,, tmp73
	out 56-32,__zero_reg__	 ; ,,
	rcall _Z4loopv	 ; 
	ldi r24,lo8(0)	 ; ,
	ldi r25,hi8(0)	 ; ,
/* epilogue start */
	ret
	.size	main, .-main
.global	Status
.global	Status
	.section .bss
	.type	Status, @object
	.size	Status, 1
Status:
	.skip 1,0
