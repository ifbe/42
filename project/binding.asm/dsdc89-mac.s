[bits 64]
extern _pwrclkcreate
extern _workercreate
extern _supplycreate
extern _say

section .text
global _main
_main:
	push rbp
	mov rbp,rsp
	sub rsp,0x20

	mov rdi,'main'
	mov rsi,0
	mov rdx,0
	mov rcx,0
	call _pwrclkcreate

	mov rdi, fmt
	mov rsi, arg1
	mov rdx, arg2
	mov rcx, arg3
	mov r8, arg4
	mov r9, arg5
	call _say

	mov rdi,'wnd'
	mov rsi,0
	mov rdx,0
	mov rcx,0
	call _supplycreate

	mov rdi,0
	mov rsi,0
	mov rdx,0
	mov rcx,0
	call _workercreate

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
