[bits 64]
extern birth
extern death
extern say

section .text
global main
main:
	call birth

	mov rcx, fmt
	mov rdx, arg1
	mov r8, arg2
	mov r9, arg3
	call say

	call death
	ret

section .data
fmt:
	db "rcx, %s, %s, %s", 0xa, 0
arg1:
	db "rdx", 0
arg2:
	db "r8", 0
arg3:
	db "r9", 0
