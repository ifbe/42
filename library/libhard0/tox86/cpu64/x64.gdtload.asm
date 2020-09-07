[bits 64]

section .text
global loadgdtandtss
loadgdtandtss:
	mov word [0x800], 4*8-1
	mov qword [0x802], 0x10000
	lgdt [0x800]

	;fs,gs
	mov rax, 0x18	;kernel data @ 18
	mov ss, ax		;must must must
	mov ds, ax		;no harm
	mov es, ax		;no harm
	mov fs, ax
	mov gs, ax

	;cs
	pop rax
	push 0x10	;kernel code @ 10
	push rax
	retf		;yasm:retf		;nasm:retfq