[bits 64]

section .text
global loadgdtandtss
loadgdtandtss:
	mov word [0x800], 0x17
	mov qword [0x802], 0x10000
	lgdt [0x800]

	;fs,gs
	mov rax, 0x10
	mov ss, ax		;must must must
	mov ds, ax		;no harm
	mov es, ax		;no harm
	mov fs, ax
	mov gs, ax

	;cs
	pop rax
	push 8
	push rax
	retf		;yasm:retf		;nasm:retfq