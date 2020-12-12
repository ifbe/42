[bits 64]
extern isr_rtc
extern isr_825x
extern endofirq
section .text


global getisr03
getisr03:
	lea rax,[rel isr03_debug]
	ret

global getisr20
getisr20:
	lea rax,[rel isr20_825x]
	ret

global getisr80
getisr80:
	lea rax,[rel isr80_ring3to0]
	ret


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


isr03_debug:
	iretq


;[sp,sp+0x27]: ip,cs,rf,sp,ss
isr20_825x:
	push rdi
	push rsi
	push rdx
	push rcx
	push rbx
	push rax
	mov rdi, rsp	;for abi: di,si,dx,cx,r8,r9
	mov rcx, rdi	;for abi: cx,dx,r8,r9
	call isr_825x
	mov rdi, 0x20	;for abi: di,si,dx,cx,r8,r9
	mov rcx, rdi	;for abi: cx,dx,r8,r9
	call endofirq
	pop rax
	pop rbx
	pop rcx
	pop rdx
	pop rsi
	pop rdi
	iretq 


isr80_ring3to0:
	push rax

	mov       rax, 0x10		;kernel cs @ 10
	mov [rsp+0x10], rax
	mov       rax, 0x18		;kernel ss @ 18
	mov [rsp+0x28], rax

	pop rax
	iretq