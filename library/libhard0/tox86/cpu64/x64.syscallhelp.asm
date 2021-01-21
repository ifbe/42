extern syscall_handler


section .text
[bits 64]


global getisr80
getisr80:
	lea rax,[rel int80_syscall]
	ret


global getsyscall
getsyscall:
	lea rax,[rel int80_syscall]
	ret


int80_syscall:
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
    call syscall_handler

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