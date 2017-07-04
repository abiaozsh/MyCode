;The terms “less” and “greater” are used for comparisons of signed integers 
;the terms “above” and “below” are used for unsigned integers.
;c d 8 9 / RBX、RBP、RDI、RSI、R12、R13、R14 和 R15 / RAX、RCX、RDX、R8、R9、R10 和 R11

;Intel386函数传参是通过ebp寄存器和栈实现的，而AMD64稍有不同。函数参数是通过寄存器传递的，
;从第一个参数开始依次为rdi, rsi, rdx, rcx, r8, r9。可以理解为这六个寄存器归被调函数所有。
;rbp, rbx, r12, r13, r14, r15属于调用函数，如果被调函数要用这六个寄存器，要在栈中进行push/pop操作。
;如果明白了这一点，再看汇编程序，大部分操作都是在准备rdi, rsi, rdx, rcx, r8, r9这六个寄存器，
;也就是函数参数。


EXTRN	__security_check_cookie:PROC
PUBLIC	addA
_TEXT	SEGMENT
;rcx* Num_Array, rdx* num_Num_Array, r8 cnt, r9 disExp
;PSE$ = 8
addA	PROC

	push rdi
	push rsi
	
	;叠合部分加法
	;被加数
	mov r10,rcx					;r10 : Num_Array
	lea rdi,[r10+r9*8-8]							;mov edi, &Num_Array[disExp - 1];

	;加数
	lea rsi,[rdx-8]									;mov esi, &num_Num_Array[-1];

	;位数
	mov rcx,r8					;r8 : cnt			;mov ecx,cnt
	clc
	lpa:
		mov rax,[rdi+rcx*8];载入被加数
		adc rax,[rsi+rcx*8];被加数+加数+进位标志
		mov [rdi+rcx*8],rax;保存结果
	dec rcx
	jnz lpa
	mov rdx,0;传出进位标志
	adc rdx,0;mov carry,edx


	;继续向上进位
	;if (disExp>0)
	cmp r9,0
	jle jp2
	
		lea rdi,[r10-8]									;mov edi,&Num_Array[-1];
		mov rcx,r9										;mov ecx,disExp
		
		;载入进位标志
		rcr rdx,1
		lpb:
			;载入被加数
			mov rax,[rdi+rcx*8]
			;加进位标志
			adc rax,0
			;保存结果
			mov [rdi+rcx*8],rax
			jnc ext
		dec rcx
		jnz lpb
		ext:
		mov rdx,0
		adc rdx,0
		;mov carry,edx
	jp2:
	pop rsi
	pop rdi

	;return carry;
	mov rax,rdx
	ret
addA	ENDP
_TEXT	ENDS



PUBLIC	addB
_TEXT	SEGMENT
;rcx* Num_Array, rdx* num_Num_Array, r8 cnt, r9 disExp
;PSE$ = 8
addB	PROC
	push rbx
	push rdi
	push rsi

	;被加数高部
	;&Num_Array[-1];
	lea rdi,[rcx-8]

	;叠合部分加法
	mov r10,rdx   ;r10 : num_Num_Array

	;加数
	;&num_Num_Array[disExp - 1];
	lea rsi,[r10+r9*8-8]
		
	;被加数低部，原数据被废弃
	;&Num_Array[disExp - 1];
	lea rbx,[rcx+r9*8-8]
				
	;mov ecx,cnt
	mov rcx,r8

	;清除进位标志
	clc
	lpa:
		;载入被加数高部
		mov rax,[rdi+rcx*8]
		;被加数+加数+进位标志
		adc rax,[rsi+rcx*8]
		;保存到被加数低部
		mov [rbx+rcx*8],rax
	dec rcx
	jnz lpa
	;传出进位标志
	mov rdx,0
	adc rdx,0
	;mov carry,edx
	
	;继续向上进位
		
	;mov edi, &Num_Array[-1]; not touched

	;mov esi, &num_Num_Array[-1];
	lea rsi,[r10-8]

	;mov ecx,disExp
	mov rcx,r9
		
	rcr rdx,1;载入进位标志
	lpb:
		mov rax,[rsi+rcx*8]
		adc rax,0
		mov [rdi+rcx*8],rax
	dec rcx
	jnz lpb
	mov rdx,0
	adc rdx,0

	pop rsi
	pop rdi
	pop rbx
	;return carry;
	mov rax,rdx
	ret
addB	ENDP
_TEXT	ENDS





;高幂减低幂 100 - 10
PUBLIC	subA
_TEXT	SEGMENT
;rcx* Num_Array, rdx* num_Num_Array, r8 cnt, r9 disExp
;PSE$ = 8
subA	PROC
	push rdi
	push rsi

	mov r10,rcx
	;mov edi,  &Num_Array[disExp - 1];
	lea rdi,[r10+r9*8-8]
	
	;mov esi, &num_Num_Array[-1];
	lea rsi,[rdx-8]

	;mov ecx,cnt
	mov rcx,r8
		
	clc
	lpa:
		mov rax,[rdi+rcx*8]
		sbb rax,[rsi+rcx*8]
		mov [rdi+rcx*8],rax
	dec rcx
	jnz lpa
	mov rdx,0
	adc rdx,0;mov carry,edx

	;mov edi, &Num_Array[-1];
	lea rdi,[r10-8]

	mov rcx,r9 ;mov ecx,disExp

	rcr rdx,1;载入进位标志
	lpb:
		mov rax,[rdi+rcx*8]
		sbb rax,0
		mov [rdi+rcx*8],rax
		jnc ext
	dec rcx
	jnz lpb
	ext:

	pop rsi
	pop rdi
	ret
subA	ENDP
_TEXT	ENDS



;低幂减高幂 10 - 100
; rdi           .rbx            .
;-               rsi            .
PUBLIC	subB
_TEXT	SEGMENT
;rcx* Num_Array, rdx* num_Num_Array, r8 cnt, r9 disExp
;PSE$ = 8
subB	PROC
	push rbx
	push rdi
	push rsi

	;mov edi, &Num_Array[-1];
	lea rdi,[rcx-8]
		
	;&num_Num_Array[disExp - 1];
	lea rsi,[rdx+r9*8-8]
		
	;&Num_Array[disExp - 1];
	lea rbx,[rcx+r9*8-8]

	mov r10,rdx  ;num_Num_Array

	;mov ecx,cnt
	mov rcx,r8
	clc
	lpa:
		mov rax,[rsi+rcx*8]
		sbb rax,[rdi+rcx*8]
		mov [rbx+rcx*8],rax
	dec rcx
	jnz lpa
	mov rdx,0
	adc rdx,0


	;mov edi, &Num_Array[-1];
	;lea rdi,[rcx-8] ;// 已经赋过值了，rcx已被破坏

	;mov esi, &num_Num_Array[-1];
	lea rsi,[r10-8] ;//前面有 mov r10,rdx  ;num_Num_Array  rdx被用作进位
		
	mov rcx,r9 ;disExp

	rcr rdx,1;载入进位标志
	lpb:
		mov rax,[rsi+rcx*8]
		sbb rax,0
		mov [rdi+rcx*8],rax
	dec rcx
	jnz lpb

	pop rsi 
	pop rdi
	pop rbx
	ret
subB	ENDP
_TEXT	ENDS



PUBLIC	subC
_TEXT	SEGMENT
;rcx* Num_Array, rdx* num_Num_Array, r8 cnt
;PSE$ = 8
subC	PROC
	push rdi
	push rsi
	lea rdi,[rcx-8] ;Num_Array;
	lea rsi,[rdx-8] ;num_Num_Array;
	mov rcx,r8
	clc
	lp:
		mov rax,[rdi+rcx*8]
		sbb rax,[rsi+rcx*8]
		mov [rdi+rcx*8],rax
	dec rcx
	jnz lp
	pop rsi
	pop rdi
	ret
subC	ENDP
_TEXT	ENDS




PUBLIC	subD
_TEXT	SEGMENT
;rcx* Num_Array, rdx* num_Num_Array, r8 cnt
;PSE$ = 8
subD	PROC
	push rdi
	push rsi
	lea rdi,[rcx-8]  ;Num_Array;
	lea rsi,[rdx-8]  ;num_Num_Array;
	mov rcx,r8
	clc
	lp:
		mov rax,[rsi+rcx*8]
		sbb rax,[rdi+rcx*8]
		mov [rdi+rcx*8],rax
	dec rcx
	jnz lp
	pop rsi
	pop rdi
	ret
subD	ENDP
_TEXT	ENDS




PUBLIC	incr
_TEXT	SEGMENT
;rcx Num_Array,rdx pExponent
;PSE$ = 8
incr	PROC
	push rbx
	push rdi
	push rsi
	mov r10,rcx	;Num_Array
	mov rbx,rdx	;pExponent
	mov rcx,[rbx]
	cmp rcx,0  ;if (cnt>0)
	jle jp1
		mov rdi,r10  ;&Num_Array[-1];
		sub rdi,8
		stc
		lpa:
			mov rax,[rdi+rcx*8]
			adc rax,0
			mov [rdi+rcx*8],rax
			jnc ext
		dec rcx
		jnz lpa
		ext:
		mov rdx,0
		adc rdx,0
		;mov carry,edx
	jp1:
	cmp rdx,1 ;if (carry == 1)
	jne jp2
		inc QWORD PTR [rbx]   ;*pExponent++;
		mov rcx,[rbx]
		dec rcx
		;mov edi,Num_Array
		mov rsi,rdi
		add rdi,8  ;Num_Array[i] = Num_Array[i - 1];
		lpb:
			mov rax,[rsi+rcx*8]
			mov [rdi+rcx*8],rax
		dec rcx
		jnz lpb
		;Num_Array[0] = 1;
		mov QWORD PTR [rdi],1
	jp2:
	pop rsi
	pop rdi
	pop rbx
	ret	0
incr	ENDP
_TEXT	ENDS


END
