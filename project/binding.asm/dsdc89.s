[bits 64]
extern pwrclkcreate
extern workercreate
extern supplycreate
extern say

section .text
global main
main:
	push rbp
	mov rbp,rsp
	sub rsp,0x20

	mov rdi,'main'
	mov rsi,0
	mov rdx,0
	mov rcx,0
	call pwrclkcreate

	mov rdi, fmt
	mov rsi, arg1
	mov rdx, arg2
	mov rcx, arg3
	mov r8, arg4
	mov r9, arg5
	call say

	mov rdi,'wnd'
	mov rsi,0
	mov rdx,0
	mov rcx,0
	call supplycreate

	mov rdi,0
	mov rsi,0
	mov rdx,0
	mov rcx,0
	call workercreate

	leave
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
