	.file	"a.c"
	.intel_syntax noprefix
	.text
.globl ___main
	.def	___main;	.scl	2;	.type	32;	.endef
___main:
	push	ebp
	mov	ebp, esp
	pop	ebp
	ret
	.def	___main;	.scl	2;	.type	32;	.endef
.globl _main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
	push	ebp
	mov	ebp, esp
	and	esp, -16
	sub	esp, 32
	call	___main
	mov	DWORD PTR [esp+12], 0
	mov	DWORD PTR [esp+16], 0
	mov	DWORD PTR [esp+20], 33558528
	mov	DWORD PTR [esp+24], 33558544
	jmp	L5
L7:
	nop
L5:
	mov	eax, DWORD PTR [esp+20]
	mov	eax, DWORD PTR [eax]
	mov	DWORD PTR [esp+28], eax
	mov	eax, DWORD PTR [esp+28]
	cmp	eax, DWORD PTR [esp+12]
	jne	L7
	mov	eax, DWORD PTR [esp+28]
	mov	DWORD PTR [esp+12], eax
	add	DWORD PTR [esp+16], 1
	mov	eax, DWORD PTR [esp+24]
	mov	edx, DWORD PTR [esp+16]
	mov	DWORD PTR [eax], edx
	jmp	L5
