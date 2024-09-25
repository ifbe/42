.text
.align 4
.globl _start
_start:
	subq $8, %rsp

	movq %rcx, %rdi
	movq %rdx, %rsi
	call efi_main

	addq $8, %rsp
	ret




.data
dummy:	.long	0




.section .reloc, "a"
label1:
	.long	dummy-label1				// Page RVA
	.long	10					// Block Size (2*4+2)
	.word	0		// reloc for dummy

