%define PERCPU 0x30000
%define GDTBUF (PERCPU+0x000)
%define TSSBUF (PERCPU+0x1000)
%define IDTBUF (PERCPU+0x2000)
%define STKBUF (PERCPU+0x10000)

%define KERNCODE 0x10
%define KERNDATA 0x18


section .text
[bits 64]


global loadgdt
loadgdt:
	;gdt
	mov word [0x800], 8*8-1
	mov qword [0x802], GDTBUF
	lgdt [0x800]

	;fs,gs
	mov rax, KERNDATA	;kernel data @ 18
	mov ss, ax		;must must for retf
	mov ds, ax		;no harm
	mov es, ax		;no harm
	mov fs, ax
	mov gs, ax

	;cs
	pop rax		;return address
	push KERNCODE	;kernel code @ 10
	push rax
	retf		;yasm:retf		;nasm:retfq


global loadtss
loadtss:
	mov ax, 0x30	;task state segment @ 30
	ltr ax
	ret