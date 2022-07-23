
%define APPCPU 0x20000
%define APPCPU_GDT (APPCPU+0x000)
%define APPCPU_TSS (APPCPU+0x1000)
%define APPCPU_IDT (APPCPU+0x2000)
%define APPCPU_RSP (APPCPU+0x10000)

%define BSPCPU 0x30000
%define BSPCPU_GDT (BSPCPU+0x000)
%define BSPCPU_TSS (BSPCPU+0x1000)
%define BSPCPU_IDT (BSPCPU+0x2000)
%define BSPCPU_RSP (BSPCPU+0x10000)

%define KERNCODE 0x10
%define KERNDATA 0x18


section .text
[bits 64]


global loadgdt
loadgdt:
	;gdt
	mov word [0x800], 8*8-1
	mov qword [0x802], BSPCPU_GDT
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


global loadgdt_ap
loadgdt_ap:
	;gdt
	mov word [0x800], 8*8-1
	mov qword [0x802], APPCPU_GDT
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


global loadtss_ap
loadtss_ap:
	mov ax, 0x30	;task state segment @ 30
	ltr ax
	ret