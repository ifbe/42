%define GDTBUF 0x30000
[bits 64]
section .text


global loadgdt
loadgdt:
	;gdt
	mov word [0x800], 8*8-1
	mov qword [0x802], GDTBUF
	lgdt [0x800]

	;fs,gs
	mov rax, 0x18	;kernel data @ 18
	mov ss, ax		;must must for retf
	mov ds, ax		;no harm
	mov es, ax		;no harm
	mov fs, ax
	mov gs, ax

	;cs
	pop rax		;return address
	push 0x10	;kernel code @ 10
	push rax
	retf		;yasm:retf		;nasm:retfq


global loadtss
loadtss:
	mov ax, 0x30	;task state segment @ 30
	ltr ax
	ret