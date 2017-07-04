;The terms ��less�� and ��greater�� are used for comparisons of signed integers 
;the terms ��above�� and ��below�� are used for unsigned integers.
;c d 8 9 / RBX��RBP��RDI��RSI��R12��R13��R14 �� R15 / RAX��RCX��RDX��R8��R9��R10 �� R11

;Intel386����������ͨ��ebp�Ĵ�����ջʵ�ֵģ���AMD64���в�ͬ������������ͨ���Ĵ������ݵģ�
;�ӵ�һ��������ʼ����Ϊrdi, rsi, rdx, rcx, r8, r9���������Ϊ�������Ĵ����鱻���������С�
;rbp, rbx, r12, r13, r14, r15���ڵ��ú����������������Ҫ���������Ĵ�����Ҫ��ջ�н���push/pop������
;�����������һ�㣬�ٿ������򣬴󲿷ֲ���������׼��rdi, rsi, rdx, rcx, r8, r9�������Ĵ�����
;Ҳ���Ǻ���������


EXTRN	__security_check_cookie:PROC
PUBLIC	addA
_TEXT	SEGMENT
;rcx* Num_Array, rdx* num_Num_Array, r8 cnt, r9 disExp
;PSE$ = 8
addA	PROC

	push rdi
	push rsi
	
	;���ϲ��ּӷ�
	;������
	mov r10,rcx					;r10 : Num_Array
	lea rdi,[r10+r9*8-8]							;mov edi, &Num_Array[disExp - 1];

	;����
	lea rsi,[rdx-8]									;mov esi, &num_Num_Array[-1];

	;λ��
	mov rcx,r8					;r8 : cnt			;mov ecx,cnt
	clc
	lpa:
		mov rax,[rdi+rcx*8];���뱻����
		adc rax,[rsi+rcx*8];������+����+��λ��־
		mov [rdi+rcx*8],rax;������
	dec rcx
	jnz lpa
	mov rdx,0;������λ��־
	adc rdx,0;mov carry,edx


	;�������Ͻ�λ
	;if (disExp>0)
	cmp r9,0
	jle jp2
	
		lea rdi,[r10-8]									;mov edi,&Num_Array[-1];
		mov rcx,r9										;mov ecx,disExp
		
		;�����λ��־
		rcr rdx,1
		lpb:
			;���뱻����
			mov rax,[rdi+rcx*8]
			;�ӽ�λ��־
			adc rax,0
			;������
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

	;�������߲�
	;&Num_Array[-1];
	lea rdi,[rcx-8]

	;���ϲ��ּӷ�
	mov r10,rdx   ;r10 : num_Num_Array

	;����
	;&num_Num_Array[disExp - 1];
	lea rsi,[r10+r9*8-8]
		
	;�������Ͳ���ԭ���ݱ�����
	;&Num_Array[disExp - 1];
	lea rbx,[rcx+r9*8-8]
				
	;mov ecx,cnt
	mov rcx,r8

	;�����λ��־
	clc
	lpa:
		;���뱻�����߲�
		mov rax,[rdi+rcx*8]
		;������+����+��λ��־
		adc rax,[rsi+rcx*8]
		;���浽�������Ͳ�
		mov [rbx+rcx*8],rax
	dec rcx
	jnz lpa
	;������λ��־
	mov rdx,0
	adc rdx,0
	;mov carry,edx
	
	;�������Ͻ�λ
		
	;mov edi, &Num_Array[-1]; not touched

	;mov esi, &num_Num_Array[-1];
	lea rsi,[r10-8]

	;mov ecx,disExp
	mov rcx,r9
		
	rcr rdx,1;�����λ��־
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





;���ݼ����� 100 - 10
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

	rcr rdx,1;�����λ��־
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



;���ݼ����� 10 - 100
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
	;lea rdi,[rcx-8] ;// �Ѿ�����ֵ�ˣ�rcx�ѱ��ƻ�

	;mov esi, &num_Num_Array[-1];
	lea rsi,[r10-8] ;//ǰ���� mov r10,rdx  ;num_Num_Array  rdx��������λ
		
	mov rcx,r9 ;disExp

	rcr rdx,1;�����λ��־
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
