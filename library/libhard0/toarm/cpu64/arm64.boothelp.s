

.section ".text"
.global trampoline64
trampoline64:

	// set stack
10:	ldr     x1, =0x4000
	mov     sp, x1

	// jump to C code, should not return
70:	bl initcpu_other

	// sleep forever
80:	wfe
	b 80b




.global if_el3_go_el2
if_el3_go_el2:
	//if(el3)
	mrs     x0, CurrentEL
	and     x0, x0, #12		// x0 &= 0xc
	cmp     x0, #12			// 12 means EL3
	b.eq    22f
	ret						//just ret to lr

	//el3_to_el2
22:	mov     x0, #0x5b1
	msr     scr_el3, x0
	mov     x0, #0x3c9
	msr     spsr_el3, x0
	mov     x0, lr			//lr is ret addr
	msr     elr_el3, x0
	eret




.global if_el2_go_el1
if_el2_go_el1:
	//if(el2)
	mrs     x0, CurrentEL
	and     x0, x0, #12		// x0 &= 0xc
	cmp     x0, #8			// 8 means EL2
	b.eq    11f
	ret						//just ret to lr

	//el2_to_el1
	/* Initialize Generic Timers */
11:	mrs x0, cnthctl_el2
	orr x0, x0, #0x3 /* Enable EL2 access to timers */
	msr cnthctl_el2, x0

	mrs x0, cntkctl_el1
	orr x0, x0, #0x3 /* Enable EL1 access to timers */
	msr cntkctl_el1, x0

	/* Initilize MPID/MPIDR registers */
	mrs x0, midr_el1
	msr vpidr_el2, x0
	mrs x0, mpidr_el1
	msr vmpidr_el2, x0

	/* Disable coprocessor traps */
	mov x0, #0x33ff
	msr cptr_el2, x0 /* Disable coprocessor traps to EL2 */
	msr hstr_el2, xzr /* Disable Hyp System Trap */

	mrs x0, cpacr_el1
	mov x0, #3 << 20
	msr cpacr_el1, x0 /* Enable FP/SIMD at EL1 */

	/* Initialize HCR_EL2 */
	mov x0, #(1 << 31) /* 64bit EL1 */
	orr x0, x0, #(1 << 29) /* Disable HVC */
	msr hcr_el2, x0

	/* SCTLR_EL1 initialization */
	mov x0, #0x0800
	movk x0, #0x30d0, lsl #16
	msr sctlr_el1, x0

	/* Return to the EL1_SP1 mode from EL2 */
	mov x0, sp
	msr sp_el1, x0			//Migrate SP, its wrong
	mrs x0, vbar_el2
	msr vbar_el1, x0		//Migrate VBAR
	mov x0, #0x3c5
	msr spsr_el2, x0		//EL1_SP1 | D | A | I | F
	mov x0, lr
	msr elr_el2, x0			//lr is ret addr
	eret




.global percpu_savefpu
percpu_savefpu:
	stp	q0, q1, [x0], #32
	stp	q2, q3, [x0], #32
	stp	q4, q5, [x0], #32
	stp	q6, q7, [x0], #32
	stp	q8, q9, [x0], #32
	stp	q10, q11, [x0], #32
	stp	q12, q13, [x0], #32
	stp	q14, q15, [x0], #32
	stp	q16, q17, [x0], #32
	stp	q18, q19, [x0], #32
	stp	q20, q21, [x0], #32
	stp	q22, q23, [x0], #32
	stp	q24, q25, [x0], #32
	stp	q26, q27, [x0], #32
	stp	q28, q29, [x0], #32
	stp	q30, q31, [x0], #32
	ret

.global percpu_loadfpu
percpu_loadfpu:
	ldp	q0, q1, [x0], #32
	ldp	q2, q3, [x0], #32
	ldp	q4, q5, [x0], #32
	ldp	q6, q7, [x0], #32
	ldp	q8, q9, [x0], #32
	ldp	q10, q11, [x0], #32
	ldp	q12, q13, [x0], #32
	ldp	q14, q15, [x0], #32
	ldp	q16, q17, [x0], #32
	ldp	q18, q19, [x0], #32
	ldp	q20, q21, [x0], #32
	ldp	q22, q23, [x0], #32
	ldp	q24, q25, [x0], #32
	ldp	q26, q27, [x0], #32
	ldp	q28, q29, [x0], #32
	ldp	q30, q31, [x0], #32
	ret
