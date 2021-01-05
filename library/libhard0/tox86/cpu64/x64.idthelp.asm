[bits 64]
extern isr_rtc
extern isr_825x
extern dual8259_endofirq
extern localapic_endofirq
extern scheduleprocess
section .text


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

global getisr80
getisr80:
	lea rax,[rel isr80_ring3to0]
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
	sub rsp, 0x80
	movaps [rsp+0x00], xmm0
	movaps [rsp+0x10], xmm1
	movaps [rsp+0x20], xmm2
	movaps [rsp+0x30], xmm3
	movaps [rsp+0x40], xmm4
	movaps [rsp+0x50], xmm5
	movaps [rsp+0x60], xmm6
	movaps [rsp+0x70], xmm7

	mov rdi, rsp	;for abi: di,si,dx,cx,r8,r9
	mov rcx, rdi	;for abi: cx,dx,r8,r9
	call isr_825x

	mov rdi, 0x20	;for abi: di,si,dx,cx,r8,r9
	mov rcx, rdi	;for abi: cx,dx,r8,r9
	call dual8259_endofirq

	movaps xmm0, [rsp+0x00]
	movaps xmm1, [rsp+0x10]
	movaps xmm2, [rsp+0x20]
	movaps xmm3, [rsp+0x30]
	movaps xmm4, [rsp+0x40]
	movaps xmm5, [rsp+0x50]
	movaps xmm6, [rsp+0x60]
	movaps xmm7, [rsp+0x70]
	add rsp, 0x80
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
	sub rsp, 0x80
	movaps [rsp+0x00], xmm0
	movaps [rsp+0x10], xmm1
	movaps [rsp+0x20], xmm2
	movaps [rsp+0x30], xmm3
	movaps [rsp+0x40], xmm4
	movaps [rsp+0x50], xmm5
	movaps [rsp+0x60], xmm6
	movaps [rsp+0x70], xmm7

	;inc qword [0]	;debug

	mov rdi, rsp	;for abi: di,si,dx,cx,r8,r9
	mov rcx, rdi	;for abi: cx,dx,r8,r9
	call scheduleprocess

	call localapic_endofirq

	movaps xmm0, [rsp+0x00]
	movaps xmm1, [rsp+0x10]
	movaps xmm2, [rsp+0x20]
	movaps xmm3, [rsp+0x30]
	movaps xmm4, [rsp+0x40]
	movaps xmm5, [rsp+0x50]
	movaps xmm6, [rsp+0x60]
	movaps xmm7, [rsp+0x70]
	add rsp, 0x80
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


isr80_ring3to0:
	push rax

	mov       rax, 0x10		;kernel cs @ 10
	mov [rsp+0x10], rax
	mov       rax, 0x18		;kernel ss @ 18
	mov [rsp+0x28], rax

	pop rax
	iretq