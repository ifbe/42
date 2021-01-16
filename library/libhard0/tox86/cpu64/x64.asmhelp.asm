[bits 64]
section .text


global enable_fpu
enable_fpu:
.okFPU:
	mov rax, cr0
	and al, 0xfb
	or rax, 0x22
	mov cr0, rax
	clts
	;finit
	mov eax, 1			;return 1
.noFPU:
	ret


global enable_sse
enable_sse:
	mov eax, 0x1
	cpuid
	xor eax, eax		;return 0
	test edx, 1<<25
	jz .noSSE
.okSSE:
	mov rax, cr0
	and ax, 0xFFFB		;clear coprocessor emulation CR0.EM
	or ax, 0x2			;set coprocessor monitoring  CR0.MP
	mov cr0, rax

	mov rax, cr4
	or ax, 3 << 9		;set CR4.OSFXSR(bit9) and CR4.OSXMMEXCPT(bit10)
	mov cr4, rax

	mov eax, 1			;return 1
.noSSE:
	ret


global enable_avx
enable_avx:				;must after enable_sse and enable_xsave
	push rax
	push rcx
	push rdx
 
	xor rcx, rcx
	xgetbv		;Load XCR0 register
	or eax,7	;Set AVX, SSE, X87 bits
	xsetbv		;Save back to XCR0
 
	pop rdx
	pop rcx
	pop rax
	ret



global enable_fxsave
enable_fxsave:
	ret


global enable_xsave
enable_xsave:
	mov eax, 0x1
	cpuid
	xor eax, eax		;return 0
	test ecx, 1<<26
	jz .noXSAVE
.okXSAVE:
	mov rax, cr4
	or rax, 1<<18		;CR4.OSXSAVE
	mov cr4, rax

	mov eax, 1			;return 1
.noXSAVE
	ret