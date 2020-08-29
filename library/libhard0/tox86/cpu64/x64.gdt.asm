[bits 64]

section .text
global initgdt
initgdt:
	;clear [0x10000, 0x20000)
	xor rax, rax
	mov rdi, 0x10000
	mov rcx, 0x10000
	cld
	rep stosb

	;gdt
	mov rax, 0
	mov [0x10000], rax
	mov rax, 0x002f9a0000000000
	mov [0x10008], rax
	mov rax, 0x002f920000000000
	mov [0x10010], rax

	mov word [0x800], 0x17
	mov qword [0x802], 0x10000
	lgdt [0x800]

	;tss

	;fs,gs
	mov rax, 0x10
	;mov ss, ax
	;mov ds, ax
	;mos es, ax
	mov fs, ax
	mov gs, ax

	;cs
	pop rax
	push 8
	push rax
	retf		;yasm:retf		;nasm:retfq