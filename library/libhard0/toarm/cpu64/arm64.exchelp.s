/*	// save q0-q31 onto stack
	stp	q30, q31, [sp, #-32]!
	stp	q28, q29, [sp, #-32]!
	stp	q26, q27, [sp, #-32]!
	stp	q24, q25, [sp, #-32]!
	stp	q22, q23, [sp, #-32]!
	stp	q20, q21, [sp, #-32]!
	stp	q18, q19, [sp, #-32]!
	stp	q16, q17, [sp, #-32]!
	stp	q14, q15, [sp, #-32]!
	stp	q12, q13, [sp, #-32]!
	stp	q10, q11, [sp, #-32]!
	stp	q8, q9, [sp, #-32]!
	stp	q6, q7, [sp, #-32]!
	stp	q4, q5, [sp, #-32]!
	stp	q2, q3, [sp, #-32]!
	stp	q0, q1, [sp, #-32]!

	//restore q0-q31 from stack
	ldp	q0, q1, [sp], #32
	ldp	q2, q3, [sp], #32
	ldp	q4, q5, [sp], #32
	ldp	q6, q7, [sp], #32
	ldp	q8, q9, [sp], #32
	ldp	q10, q11, [sp], #32
	ldp	q12, q13, [sp], #32
	ldp	q14, q15, [sp], #32
	ldp	q16, q17, [sp], #32
	ldp	q18, q19, [sp], #32
	ldp	q20, q21, [sp], #32
	ldp	q22, q23, [sp], #32
	ldp	q24, q25, [sp], #32
	ldp	q26, q27, [sp], #32
	ldp	q28, q29, [sp], #32
	ldp	q30, q31, [sp], #32
*/

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
	stp x30, x0, [sp,#-16]!
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
	bl exception_spsel1_sync
	b 1f

	.align 7			//5
	sub sp, sp, #128
	stp x30, x0, [sp,#-16]!
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
	bl exception_spsel1_irq
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
	ldp x30, x0, [sp], #16
	add sp, sp, #128
	eret
