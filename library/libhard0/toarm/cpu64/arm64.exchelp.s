
	.align 11
.global vectors
vectors:
	.align 7			//0
	stp x6, x7, [sp,#-16]!
	stp x4, x5, [sp,#-16]!
	stp x2, x3, [sp,#-16]!
	stp x0, x1, [sp,#-16]!
	mrs x0, esr_el1
	mrs x1, elr_el1
	mrs x2, spsr_el1
	mrs x3, far_el1
	b exception_spsel0_sync
	ldp x0, x1, [sp], #16
	ldp x2, x3, [sp], #16
	ldp x4, x5, [sp], #16
	ldp x6, x7, [sp], #16
	eret

	.align 7			//1
	mrs x0, esr_el1
	mrs x1, elr_el1
	mrs x2, spsr_el1
	mrs x3, far_el1
	b exception_spsel0_irq

	.align 7			//2
	mrs x0, esr_el1
	mrs x1, elr_el1
	mrs x2, spsr_el1
	mrs x3, far_el1
	b exception_spsel0_fiq

	.align 7			//3
	mrs x0, esr_el1
	mrs x1, elr_el1
	mrs x2, spsr_el1
	mrs x3, far_el1
	b exception_spsel0_serr

	.align 7			//4
	sub sp, sp, #128
	stp x30, xzr, [sp,#-16]!
	stp x28, x29, [sp,#-16]!
	stp x26, x27, [sp,#-16]!
	stp x24, x25, [sp,#-16]!
	stp x22, x23, [sp,#-16]!
	stp x20, x21, [sp,#-16]!
	stp x18, x19, [sp,#-16]!
	stp x16, x17, [sp,#-16]!
	stp x14, x15, [sp,#-16]!
	stp x12, x13, [sp,#-16]!
	stp x10, x11, [sp,#-16]!
	stp x8, x9, [sp,#-16]!
	stp x6, x7, [sp,#-16]!
	stp x4, x5, [sp,#-16]!
	stp x2, x3, [sp,#-16]!
	stp x0, x1, [sp,#-16]!
	mrs x0, esr_el1
	mrs x1, elr_el1
	mrs x2, spsr_el1
	mrs x3, far_el1
	bl exception_spsel1_sync		//use bl, not b
	b 1f

	.align 7			//5
	sub sp, sp, #128
	stp x30, xzr, [sp,#-16]!
	stp x28, x29, [sp,#-16]!
	stp x26, x27, [sp,#-16]!
	stp x24, x25, [sp,#-16]!
	stp x22, x23, [sp,#-16]!
	stp x20, x21, [sp,#-16]!
	stp x18, x19, [sp,#-16]!
	stp x16, x17, [sp,#-16]!
	stp x14, x15, [sp,#-16]!
	stp x12, x13, [sp,#-16]!
	stp x10, x11, [sp,#-16]!
	stp x8, x9, [sp,#-16]!
	stp x6, x7, [sp,#-16]!
	stp x4, x5, [sp,#-16]!
	stp x2, x3, [sp,#-16]!
	stp x0, x1, [sp,#-16]!
	mov x0, sp
	bl exception_spsel1_irq		//use bl, not b
	b 1f

	.align 7			//6
	mrs x0, esr_el1
	mrs x1, elr_el1
	mrs x2, spsr_el1
	mrs x3, far_el1
	b exception_spsel1_fiq

	.align 7			//7
	mrs x0, esr_el1
	mrs x1, elr_el1
	mrs x2, spsr_el1
	mrs x3, far_el1
	b exception_spsel1_serr

	.align 7			//8
	mrs x0, esr_el1
	mrs x1, elr_el1
	mrs x2, spsr_el1
	mrs x3, far_el1
	b exception_lowel64_sync

	.align 7			//9
	mrs x0, esr_el1
	mrs x1, elr_el1
	mrs x2, spsr_el1
	mrs x3, far_el1
	b exception_lowel64_irq

	.align 7			//10
	mrs x0, esr_el1
	mrs x1, elr_el1
	mrs x2, spsr_el1
	mrs x3, far_el1
	b exception_lowel64_fiq

	.align 7			//11
	mrs x0, esr_el1
	mrs x1, elr_el1
	mrs x2, spsr_el1
	mrs x3, far_el1
	b exception_lowel64_serr

	.align 7			//12
	mrs x0, esr_el1
	mrs x1, elr_el1
	mrs x2, spsr_el1
	mrs x3, far_el1
	b exception_lowel32_sync

	.align 7			//13
	mrs x0, esr_el1
	mrs x1, elr_el1
	mrs x2, spsr_el1
	mrs x3, far_el1
	b exception_lowel32_irq

	.align 7			//14
	mrs x0, esr_el1
	mrs x1, elr_el1
	mrs x2, spsr_el1
	mrs x3, far_el1
	b exception_lowel32_fiq

	.align 7			//15
	mrs x0, esr_el1
	mrs x1, elr_el1
	mrs x2, spsr_el1
	mrs x3, far_el1
	b exception_lowel32_serr

	.align 7			//padding 128b
1:	ldp x0, x1, [sp], #16
	ldp x2, x3, [sp], #16
	ldp x4, x5, [sp], #16
	ldp x6, x7, [sp], #16
	ldp x8, x9, [sp], #16
	ldp x10, x11, [sp], #16
	ldp x12, x13, [sp], #16
	ldp x14, x15, [sp], #16
	ldp x16, x17, [sp], #16
	ldp x18, x19, [sp], #16
	ldp x20, x21, [sp], #16
	ldp x22, x23, [sp], #16
	ldp x24, x25, [sp], #16
	ldp x26, x27, [sp], #16
	ldp x28, x29, [sp], #16
	ldp x30, xzr, [sp], #16
	add sp, sp, #128
	eret
