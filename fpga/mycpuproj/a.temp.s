jmp _main
.file "a.c"
.intel_syntax noprefix
.text
.globl ___main
.def ___main; .scl 2; .type 32; .endef
___main:
push ebp
mov ebp, esp
pop ebp
ret
.def ___main; .scl 2; .type 32; .endef
.globl _main
.def _main; .scl 2; .type 32; .endef
_main:
push ebp
mov ebp, esp
mov ra, -16
and esp, ra
mov ra, +32
sub esp, ra
call ___main
mov ra, +0
mov DWORD PTR [esp+12], ra
mov ra, +0
mov DWORD PTR [esp+16], ra
mov ra, +33558528
mov DWORD PTR [esp+20], ra
mov ra, +33558544
mov DWORD PTR [esp+24], ra
jmp L5
L7:
nop
L5:
mov eax, DWORD PTR [esp+20]
mov eax, DWORD PTR [eax]
mov DWORD PTR [esp+28], eax
mov eax, DWORD PTR [esp+28]
mov ra, DWORD PTR [esp+12]
cmp eax, ra
jne L7
mov eax, DWORD PTR [esp+28]
mov DWORD PTR [esp+12], eax
mov rb, DWORD PTR [esp+16]
mov ra, +1
add rb, ra
mov DWORD PTR [esp+16], rb
mov eax, DWORD PTR [esp+24]
mov edx, DWORD PTR [esp+16]
mov DWORD PTR [eax], edx
jmp L5

