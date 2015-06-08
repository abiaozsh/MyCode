	.file	"timer.cpp"
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
 ;  options passed:  -fpreprocessed timer.ii -mmcu=attiny24 -Os
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
	ldi r24,lo8(2)	 ;  tmp43,
	out 78-32,r24	 ; ,, tmp43
	out 66-32,__zero_reg__	 ; ,,
	out 44-32,__zero_reg__	 ; ,,
/* epilogue start */
	ret
	.size	_Z9TimerInitv, .-_Z9TimerInitv
.global	_Z4waitj
	.type	_Z4waitj, @function
_Z4waitj:
/* prologue: function */
/* frame size = 0 */
	movw r18,r24	 ;  ticks, ticks
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	sbi 43-32,0	 ; ,,
.L7:
	sbic 43-32,0	 ; ,,
	rjmp .L8	 ; 
	in r24,76-32	 ;  iftmp.1,,
	in r25,(76)+1-32	 ;  iftmp.1,,
	cp r24,r18	 ;  iftmp.1, ticks
	cpc r25,r19	 ;  iftmp.1, ticks
	brlo .L7	 ; ,
.L8:
	ret
	.size	_Z4waitj, .-_Z4waitj
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
	ldi r24,lo8(-128)	 ;  tmp62,
	out 70-32,r24	 ; ,, tmp62
	out 70-32,__zero_reg__	 ; ,,
	out 79-32,__zero_reg__	 ; ,,
	ldi r24,lo8(2)	 ;  tmp66,
	out 78-32,r24	 ; ,, tmp66
	out 66-32,__zero_reg__	 ; ,,
	out 44-32,__zero_reg__	 ; ,,
	sbi 58-32,0	 ; ,,
	sbi 59-32,0	 ; ,,
.L17:
	sbi 57-32,0	 ; ,,
	ldi r18,lo8(0)	 ;  i,
	ldi r19,hi8(0)	 ;  i,
.L13:
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
	subi r18,lo8(-(1))	 ;  i,
	sbci r19,hi8(-(1))	 ;  i,
	ldi r24,hi8(1000)	 ; ,
	cpi r18,lo8(1000)	 ;  i,
	cpc r19,r24	 ;  i,
	brne .L13	 ; ,
	sbi 57-32,0	 ; ,,
	ldi r18,lo8(0)	 ;  i,
	ldi r19,hi8(0)	 ;  i,
.L16:
	out (76)+1-32,__zero_reg__	 ; ,,
	out 76-32,__zero_reg__	 ; ,,
	sbi 43-32,0	 ; ,,
.L15:
	sbic 43-32,0	 ; ,,
	rjmp .L14	 ; 
	in r24,76-32	 ;  iftmp.1,,
	in r25,(76)+1-32	 ;  iftmp.1,,
	cpi r24,100	 ;  iftmp.1,
	cpc r25,__zero_reg__	 ;  iftmp.1
	brlo .L15	 ; ,
.L14:
	subi r18,lo8(-(1))	 ;  i,
	sbci r19,hi8(-(1))	 ;  i,
	ldi r24,hi8(1000)	 ; ,
	cpi r18,lo8(1000)	 ;  i,
	cpc r19,r24	 ;  i,
	brne .L16	 ; ,
	rjmp .L17	 ; 
	.size	main, .-main
.global	temp
.global	temp
	.section .bss
	.type	temp, @object
	.size	temp, 2
temp:
	.skip 2,0
