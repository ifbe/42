

.equ       X0_OFFS, 0
.equ       X1_OFFS, 8
.equ      X30_OFFS, (8*30)
.equ      X31_OFFS, (8*31)

.equ  ELR_EL1_OFFS, (256+0)
.equ SPSR_EL1_OFFS, (256+8)
.equ   SP_EL0_OFFS, (256+16)
.equ   SP_EL1_OFFS, (256+24)


	.align 11
.global vectors
vectors:
	.align 7			//0
	b exception_spsel0_sync


	.align 7			//1
	b exception_spsel0_irq


	.align 7			//2
	b exception_spsel0_fiq


	.align 7			//3
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

	mrs x2, elr_el1
	mrs x3, spsr_el1
	stp x2, x3, [sp,#ELR_EL1_OFFS]	//save elr,spsr

	mrs x0, sp_el0
	mov x1, sp
	stp x0, x1, [sp, #SP_EL0_OFFS]	//save sp

	mov x0, sp						//set arg0
	bl exception_spsel1_sync		//b=jump, bl=call

	b 98f


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

	mrs x2, elr_el1
	mrs x3, spsr_el1
	stp x2, x3, [sp,#ELR_EL1_OFFS]	//save elr,spsr

	mrs x0, sp_el0
	mov x1, sp
	stp x0, x1, [sp, #SP_EL0_OFFS]	//save sp

	mov x0, sp						//arg0=sp_el1
	bl exception_spsel1_irq			//b=jump, bl=call

	b 98f


	.align 7			//6
	b exception_spsel1_fiq


	.align 7			//7
	b exception_spsel1_serr


	.align 7			//8
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

	mrs x2, elr_el1
	mrs x3, spsr_el1
	stp x2, x3, [sp,#ELR_EL1_OFFS]	//save elr,spsr

	mrs x0, sp_el0
	mov x1, sp
	stp x0, x1, [sp, #SP_EL0_OFFS]	//save sp

	mov x0, sp						//arg0=sp_el1
	bl exception_lowel64_sync		//b=jump, bl=call

	b 98f


	.align 7			//9
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

	mrs x2, elr_el1
	mrs x3, spsr_el1
	stp x2, x3, [sp,#ELR_EL1_OFFS]	//save elr,spsr

	mrs x0, sp_el0
	mov x1, sp
	stp x0, x1, [sp, #SP_EL0_OFFS]	//save sp

	mov x0, sp						//arg0=sp_el1
	bl exception_lowel64_irq		//b=jump, bl=call

	b 98f


	.align 7			//10
	b exception_lowel64_fiq


	.align 7			//11
	b exception_lowel64_serr


	.align 7			//12
	b exception_lowel32_sync


	.align 7			//13
	b exception_lowel32_irq


	.align 7			//14
	b exception_lowel32_fiq


	.align 7			//15
	b exception_lowel32_serr


	.align 7			//align 128b
98:	ldp x0, x1, [sp, #SP_EL0_OFFS]	//switch stack (old stack)
	msr sp_el0, x0
	mov sp, x1

	ldp x2, x3, [sp,#ELR_EL1_OFFS]	//load elr,spsr (new stack)
	msr elr_el1, x2
	msr spsr_el1, x3

99:	ldp x0, x1, [sp], #16
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
