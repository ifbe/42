[bits 64]
extern origincreate
extern workercreate
extern supplycreate
extern say

section .text
global main
main:
	push rbp
	mov rbp,rsp
	sub rsp,0x20

	mov rcx,'main'
	mov rdx,0
	mov r8,0
	mov r9,0
	call origincreate

	mov rcx, fmt
	mov rdx, arg1
	mov r8, arg2
	mov r9, arg3
	call say

	mov rcx,'wnd'
	mov rdx,0
	mov r8,0
	mov r9,0
	call supplycreate

	mov rcx,0
	mov rdx,0
	mov r8,0
	mov r9,0
	call workercreate

	leave
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
