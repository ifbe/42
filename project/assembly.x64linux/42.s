[bits 64]
extern birth
extern death

section .text
global main
main:
	call birth
	call death
	ret
