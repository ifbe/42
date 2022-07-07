%define XSAVESIZE 1024
extern isr_rtc
extern isr_825x
extern endofextirq
extern percpu_schedule


section .text
[bits 64]


global getisr03
getisr03:
	lea rax,[rel isr03_debug]
	ret

global getisr20
getisr20:
	lea rax,[rel isr20_825x]
	ret

global getisr40
getisr40:
	lea rax,[rel isr40_apictimer]
	ret


isr03_debug:
	iretq


;[sp,sp+0x27]: ip,cs,rf,sp,ss
isr20_825x:
	push rbp
	push rdi
	push rsi
	push rdx
	push rcx
	push rbx
	push rax
	push r15
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8

	mov rdi, rsp	;for abi: di,si,dx,cx,r8,r9
	mov rcx, rdi	;for abi: cx,dx,r8,r9
	sub rsp, 0x100	;mingw bug!!!: it breaks stack [rsp,rsp+?]
	call isr_825x

	mov rdi, 0x20	;for abi: di,si,dx,cx,r8,r9
	mov rcx, rdi	;for abi: cx,dx,r8,r9
	call endofextirq

	add rsp, 0x100
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15
	pop rax
	pop rbx
	pop rcx
	pop rdx
	pop rsi
	pop rdi
	pop rbp
	iretq


isr40_apictimer:
	push rbp
	push rdi
	push rsi
	push rdx
	push rcx
	push rbx
	push rax
	push r15
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8

	mov rdi, rsp	;for abi: di,si,dx,cx,r8,r9
	mov rcx, rdi	;for abi: cx,dx,r8,r9
	sub rsp, 0x100	;mingw bug!!!: it breaks stack [rsp,rsp+?]
	call percpu_schedule

	call endofextirq

	add rsp, 0x100
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15
	pop rax
	pop rbx
	pop rcx
	pop rdx
	pop rsi
	pop rdi
	pop rbp
	iretq