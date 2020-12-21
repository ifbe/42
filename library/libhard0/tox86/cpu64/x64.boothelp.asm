%define TRAMPOLINE16 0x8000
%define FromBsp_rip 0xffe0
%define FromBsp_rsp 0xffe8
%define ApToBsp_message 0xfff0
%define BspToAp_command 0xfff8

%define GDTBUF 0x30000
	%define KERNCODE 0x10
	%define KERNDATA 0x18
%define PML4ADDR 0x7f000

%define TEMPGDTR 0x800
%define TEMPIDTR 0x900
%define TEMPJUMP 0xa00


section .text


[bits 16]
entry_ap_start:
	;0.tell bsp, ap started in 16bit
	mov ax, 0
	mov ds, ax
	mov es, ax

setflag16:
	mov di, ApToBsp_message
	mov al, 16
	mov [es:di], al

wait4cmd:
	cmp word [es:BspToAp_command],"go"
	jne wait4cmd

at16to64:
	;inc word [es:0]		;test code
	mov ax, 0xe0ff
	mov word [TEMPJUMP], ax

	mov word [TEMPIDTR + 0], 0
	mov word [TEMPIDTR + 2], 0
	mov word [TEMPIDTR + 4], 0

	mov word [TEMPGDTR + 0], 0x30-1
	mov word [TEMPGDTR + 2], GDTBUF & 0xffff
	mov word [TEMPGDTR + 4], GDTBUF >> 16

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

setflag64:
	mov di, ApToBsp_message
	mov al, 64
	mov [es:di], al

lastjump:
	mov eax, [FromBsp_rsp]
	mov esp, eax

	mov eax, [FromBsp_rip]
	jmp dword KERNCODE:TEMPJUMP

failloop:
	cli
	hlt
	jmp failloop
entry_ap_end:


[bits 64]
global enterring3
enterring3:
	pop             rax		;user ip
	mov [rsp-0x28], rax
	mov       rax, 0x23		;user cs
	mov [rsp-0x20], rax
	mov      rax, 0x202		;rflag
	mov [rsp-0x18], rax
	mov        rax, rsp		;user sp
	mov [rsp-0x10], rax
	mov       rax, 0x2b		;user ss
	mov [rsp-0x08], rax

	sub rsp, 0x28
	iretq

global get_entry_ap_start
get_entry_ap_start:
	lea rax,[rel entry_ap_start]
	ret

global get_entry_ap_end
get_entry_ap_end:
	lea rax,[rel entry_ap_end]
	ret