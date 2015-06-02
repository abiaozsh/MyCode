;The terms “less” and “greater” are used for comparisons of signed integers
;the terms “above” and “below” are used for unsigned integers.

;c d 8 9 入参
; RBX、RBP、RDI、RSI、R12、R13、R14 和 R15 需要保护
; RAX、RCX、RDX、R8、R9、R10 和 R11 不需保护

EXTRN	__security_check_cookie:PROC
PUBLIC	mulcore
_TEXT	SEGMENT
;rcx* Num_Array, rdx* buf_Num_Array, r8 cnt, r9 shift N$,n
N$ = 32+8
mulcore	PROC
	mov r10,QWORD PTR N$[rsp] ;n
	push rsi
	push rdi

	;mov edi,&Num_Array[shift - 1];
	lea rdi,[rcx+r9*8-8]

	;int carry = 0;
	;if (cnt>0)
	cmp r8,0
	jle jp1
		;mov esi,&buf_Num_Array[-1];
		lea rsi,[rdx-8]

		;mov ecx,cnt
		mov rcx,r8

		mov r11,0
		lp1:
			mov rax,r10
			cqo
			mul QWORD PTR [rsi+rcx*8] ; low:rax,high:rdx
			add rax,[rdi+rcx*8]
			adc rdx,0
			add rax,r11
			adc rdx,0
			mov [rdi+rcx*8],rax
			mov r11,rdx
		;loop lp1
		dec rcx
		jnz lp1
		;mov carry,ebx
	jp1:

	;继续进位
	;if (shift>0)
	cmp r9,0
	jle jp2
		;mov edi&Num_Array[shift - 1];

		;mov ecx,shift
		mov rcx,r9
		
		;mov edx,carry
		lp2:
			mov rax,[rdi]
			add rax,rdx
			mov [rdi],rax
			mov rdx,0
			jnc ext
			mov rdx,1
			sub rdi,8
		;loop lp2
		dec rcx
		jnz lp2
		ext:
		;mov carry,edx
	jp2:

	pop rdi
	pop rsi

	mov rax,rdx
	ret
mulcore	ENDP
_TEXT	ENDS


PUBLIC	muln
_TEXT	SEGMENT
;rcx* Precision, rdx* Num_Array, r8 pExponent, r9 n
;extern "C" void muln(int Precision, unsigned int* Num_Array, int* pExponent, unsigned int n);
muln	PROC
	push rbx
	push rsi
	push rdi

	mov r11,0	;int temp_Exponent = *pExponent; (*pExponent) = 0;
	
	;mov rcx,rcx	;int cnt = Precision;
	
	mov r10,rcx
	
	;mov edi,&Num_Array[-1];
	lea rdi,[rdx-8]
	mov rbx,0
	lp:
		mov rax,r9  ;n
		cqo
		mul QWORD PTR [rdi+rcx*8]	;l:eax,h:edx
		add rax,rbx		;+ carry
		adc rdx,0
		mov [rdi+rcx*8],eax
		mov rbx,rdx
	dec rcx
	jnz lp
	;mov carry,ebx

	cmp rbx,0  ;if (carry!=0)
	jz jp1
		inc r11	 ;(*pExponent)++;
		mov rcx,r10	;Precision - 1
		dec rcx
		mov rsi,rdi
		add rdi,8
		;for (int i = Precision - 1; i > 0; i--)
		;{
		;	Num_Array[i] = Num_Array[i - 1];
		;}
		lp1:
			mov rax,[rsi+rcx*8]
			mov [rdi+rcx*8],rax
		dec rcx
		jnz lp1
		;Num_Array[0] = carry;
		mov [rdi],rbx
	jp1:
	;(*pExponent) += temp_Exponent;
	add [r8],r11
	pop rdi
	pop rsi
	pop rbx
	ret
muln	ENDP
_TEXT	ENDS



END