[bits 64]
extern birth
extern death
extern say

section .text
global main
main:
	call birth

	mov rdi, fmt
	mov rsi, arg1
	mov rdx, arg2
	mov rcx, arg3
	mov r8, arg4
	mov r9, arg5
	call say

	call death
	ret

section .data
fmt:
	db "rdi, %s, %s, %s, %s, %s", 0xa, 0
arg1:
	db "rsi", 0
arg2:
	db "rdx", 0
arg3:
	db "rcx", 0
arg4:
	db "r8", 0
arg5:
	db "r9", 0
