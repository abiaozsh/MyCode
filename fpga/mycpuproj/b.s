	.file	"b.c"
	.intel_syntax noprefix
	.comm	_temp, 16	 # 4
.globl _cnt
	.bss
	.align 4
_cnt:
	.space 4
	.text
.globl ___main
	.def	___main;	.scl	2;	.type	32;	.endef
___main:
	push	ebp
	mov	ebp, esp
	pop	ebp
	ret
.globl _test
	.def	_test;	.scl	2;	.type	32;	.endef
_test:
	push	ebp
	mov	ebp, esp
	sub	esp, 16
	mov	DWORD PTR [ebp-8], 0
	jmp	L5
L7:
	nop
L5:
	mov	eax, DWORD PTR [ebp+8]
	mov	eax, DWORD PTR [eax]
	mov	DWORD PTR [ebp-4], eax
	mov	eax, DWORD PTR [ebp-4]
	cmp	eax, DWORD PTR [ebp-8]
	je	L7
	mov	eax, DWORD PTR [ebp-4]
	mov	DWORD PTR [ebp-8], eax
	mov	eax, DWORD PTR _cnt
	add	eax, 1
	mov	DWORD PTR _cnt, eax
	mov	edx, DWORD PTR _cnt
	mov	eax, DWORD PTR [ebp+12]
	mov	DWORD PTR [eax], edx
	mov	eax, DWORD PTR _cnt
	mov	DWORD PTR _temp, eax
	jmp	L5
	.def	___main;	.scl	2;	.type	32;	.endef
.globl _main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
	push	ebp
	mov	ebp, esp
	and	esp, -16
	sub	esp, 32
	call	___main
	mov	DWORD PTR [esp+24], 33558528
	mov	DWORD PTR [esp+28], 33558544
	mov	eax, DWORD PTR [esp+28]
	mov	DWORD PTR [esp+4], eax
	mov	eax, DWORD PTR [esp+24]
	mov	DWORD PTR [esp], eax
	call	_test
	leave
	ret
