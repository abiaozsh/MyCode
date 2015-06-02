;The terms “less” and “greater” are used for comparisons of signed integers
;the terms “above” and “below” are used for unsigned integers.

;c d 8 9 入参
; RBX、RBP、RDI、RSI、R12、R13、R14 和 R15 需要保护
; RAX、RCX、RDX、R8、R9、R10 和 R11 不需保护

PUBLIC	divcore
_TEXT	SEGMENT
;rcx r, rdx b, r8 idx_main, r9 len
;PSE$ = 8
divcore	PROC
	push rbx
	push rsi
	push rdi

	mov r11,rdx					;b
	lea rdi,[rcx+r8*8]			;mov edi,&r[idx_main]
	mov rdx,[rcx+r8*8]			;rh = r[idx_main];
	mov rax,[rcx+r8*8+8]		;rl = r[idx_main + 1];
	mov rcx,[r11]				;mov ecx,b[0];

	cmp rdx,rcx
	jz else1
		div rcx
		jmp endif1
	else1:
		mov rax,0
		not rax
	endif1:

	;r8:商
	mov r8,rax					;mov q,eax

	;乘减 [rdi] = [rsi]*r8 - [rdi];
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;借位
	lea rsi,[r11-8]				;mov esi,&b[-1];
	mov rcx,r9					;mov ecx,len
	
	mov rbx,0					;mov ebx,0 
	lp1:
		mov rax,r8
		cqo
		mul QWORD PTR [rsi+rcx*8]
		add rax,rbx
		adc rdx,0
		mov rbx,rdx
		mov rdx,rax
		not rdx
		sub rax,[rdi+rcx*8]
		neg rax
		mov [rdi+rcx*8],rax
		cmp rdx,rax
		adc rbx,0
	dec rcx
	jnz lp1
	sub [rdi],rbx
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	;加回
	;0,1,2 times
	;while (r[idx_main]>0x7FFFFFFFFFFFFFFF) (r[idx_main]<0)
	jp1:
	mov rax,[rdi]
	cmp rax,0
	jge jp2	 ;r[idx_main]<0
		mov rcx,r9				;mov ecx,len
		clc
		lp2:
			mov rax,[rdi+rcx*8]
			adc rax,[rsi+rcx*8]
			mov [rdi+rcx*8],rax
		dec rcx
		jnz lp2
		mov rax,[rdi]
		adc rax,0
		mov [rdi],rax
		dec r8				;q--;
	jmp jp1
	jp2:

	pop rdi
	pop rsi
	pop rbx

	mov rax,r8

	ret
divcore	ENDP
_TEXT	ENDS


PUBLIC	divn
_TEXT	SEGMENT
;rcx Precision, rdx Num_Array, r8 pExponent, r9 n
;extern "C" void divn(int Precision, unsigned int* Num_Array, int* pExponent, unsigned int n)
;PSE$ = 8
divn	PROC
	push rbx
	push rsi
	push rdi

	mov r11,rcx
	mov rsi,rdx
	mov rdi,rdx	;写结果
	mov rax,[rsi]
	mov rdx,0
	div r9
	;mov rdx,rdx 余数变高部(不用做)

	cmp rax,0
	mov rbx,r8
	jz jp0
		;结果不是0时
		mov [rdi],rax	;保存结果
		add rdi,8	;移动指针
		inc QWORD PTR [rbx]	;抵消(幂-1)
	jp0:
	;结果是0时,不保存结果,不移动指针
	dec QWORD PTR [rbx]		;幂-1

	add rsi,8
	mov rcx,r11	;Precision
	dec rcx
	cmp rcx,0
	jz ext
	lp:
		mov rax,[rsi]
		div r9
		mov [rdi],rax
		add rsi,8
		add rdi,8
	dec rcx
	jnz lp
	ext:

	pop rdi
	pop rsi
	pop rbx
	ret
divn	ENDP
_TEXT	ENDS

END