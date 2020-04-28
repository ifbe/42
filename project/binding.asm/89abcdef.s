[bits 64]
extern pwrclkcreate
extern workercreate
extern supplycreate
extern say

section .text
global main
main:
	mov r8,'main'
	mov r9,0
	mov r10,0
	mov r11,0
	call pwrclkcreate

	mov r8, fmt
	mov r9, arg1
	mov r10, arg2
	mov r11, arg3
	mov r12, arg4
	mov r13, arg5
	mov r14, arg6
	mov r15, arg7
	call say

	mov r8,'wnd'
	mov r9,0
	mov r10,0
	mov r11,0
	call supplycreate

	mov r8,0
	mov r9,0
	mov r10,0
	mov r11,0
	call workercreate

	ret

section .data
fmt:
	db "r8, %s, %s, %s, %s, %s, %s, %s", 0xa, 0
arg1:
	db "r9", 0
arg2:
	db "ra", 0
arg3:
	db "rb", 0
arg4:
	db "rc", 0
arg5:
	db "rd", 0
arg6:
	db "re", 0
arg7:
	db "rf", 0
