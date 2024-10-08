%define TRAMPOLINE16 0x8000
%define TRAMPOLINE64 0x9000
%define FromBsp_rsp 0xffe0
%define FromBsp_rip 0xffe8
%define ApToBsp_message 0xfff0
%define BspToAp_command 0xfff8

%define BSPCMD_16to64 64
%define BSPCMD_asmtoc 88

%define APMSG_16bit 16
%define APMSG_64bit 64

%define KERNCODE 0x10
%define KERNDATA 0x18

%define BSPCPU 0x30000
%define BSPCPU_GDT (BSPCPU+0x0000)
%define BSPCPU_TSS (BSPCPU+0x1000)
%define BSPCPU_IDT (BSPCPU+0x2000)

;%define PML1ADDR unused	;pt
%define PML2ADDR 0x42000	;pd
%define PML3ADDR 0x41000	;pdp
%define PML4ADDR 0x40000	;pml4

%define TEMPGDTR 0x800
%define TEMPIDTR 0x900
%define TEMPJUMP 0xa00


section .text


[bits 16]
trampoline16_start:
	;0.tell bsp, ap started in 16bit
	mov ax, 0
	mov ds, ax
	mov es, ax

setflag_16bit:
	mov di, ApToBsp_message
	mov al, APMSG_16bit
	mov [es:di], al

waitcmd_16to64:
	cmp word [es:BspToAp_command], BSPCMD_16to64
	jne waitcmd_16to64

at16to64:
	;inc word [es:0]		;test code
	mov ax, 0xe0ff
	mov word [TEMPJUMP], ax

	mov word [TEMPIDTR + 0], 0
	mov word [TEMPIDTR + 2], 0
	mov word [TEMPIDTR + 4], 0

	mov word [TEMPGDTR + 0], 0x40-1
	mov word [TEMPGDTR + 2], BSPCPU_GDT & 0xffff
	mov word [TEMPGDTR + 4], BSPCPU_GDT >> 16

	lidt [TEMPIDTR]

	mov eax, 10100000b
	mov cr4, eax		;Set the PAE and PGE bit.

	mov edx, PML4ADDR
	mov cr3, edx		;Point CR3 at the PML4.

	mov ecx,0xC0000080		;Read from the EFER MSR.
	rdmsr
	or eax,0x00000100		;Set the LME bit.
	wrmsr

	mov ebx, cr0
	or ebx,0x80000001		;enable paging and protection
	mov cr0, ebx

	a32 lgdt [TEMPGDTR]

lastjump:
	mov eax, TRAMPOLINE64
	jmp dword KERNCODE:TEMPJUMP

failloop:
	cli
	hlt
	jmp failloop
trampoline16_end:


[bits 64]
trampoline64_start:
	mov rax, KERNDATA	;kernel data @ 18
	mov ss, ax		;must must for retf
	mov ds, ax		;no harm
	mov es, ax		;no harm
	mov fs, ax
	mov gs, ax

setflag_64bit:
	mov edi, ApToBsp_message
	mov al, APMSG_64bit
	mov [edi], al

waitcmd_asmtoc:
	cmp word [BspToAp_command], BSPCMD_asmtoc
	jne waitcmd_asmtoc

asmtoc:
	mov rax, [FromBsp_rsp]
	mov rsp, rax

	mov rax, [FromBsp_rip]
	jmp rax
trampoline64_end:


global get_trampoline16_start
get_trampoline16_start:
	lea rax,[rel trampoline16_start]
	ret

global get_trampoline16_end
get_trampoline16_end:
	lea rax,[rel trampoline16_end]
	ret

global get_trampoline64_start
get_trampoline64_start:
	lea rax,[rel trampoline64_start]
	ret

global get_trampoline64_end
get_trampoline64_end:
	lea rax,[rel trampoline64_end]
	ret