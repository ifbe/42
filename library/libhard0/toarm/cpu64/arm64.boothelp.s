

.section ".text"
.global trampoline64
trampoline64:

	// set stack
10:	ldr     x1, =0x4000
	mov     sp, x1

	//if(el3)goto el3_to_el2
30:	mrs     x1, CurrentEL
	and     x1, x1, #12		// x1 &= 0xc
	cmp     x1, #12			// 12 means EL3
	b.ne    50f

	// at el3, to el2
40:	mov     x1, #0x5b1
	msr     scr_el3, x1
	mov     x1, #0x3c9
	msr     spsr_el3, x1
	adr     x1, 50f
	msr     elr_el3, x1
	eret

	//if(el2)goto el2_to_el1
50:	mrs     x1, CurrentEL
	and     x1, x1, #12		// x1 &= 0xc
	cmp     x1, #8			// 8 means EL2
	b.ne    70f

	// at el2, to el1
60:	/* Initialize Generic Timers */
	mrs x1, cnthctl_el2
	orr x1, x1, #0x3 /* Enable EL2 access to timers */
	msr cnthctl_el2, x1

	mrs x1, cntkctl_el1
	orr x1, x1, #0x3 /* Enable EL1 access to timers */
	msr cntkctl_el1, x1

	/* Initilize MPID/MPIDR registers */
	mrs x1, midr_el1
	msr vpidr_el2, x1
	mrs x1, mpidr_el1
	msr vmpidr_el2, x1

	/* Disable coprocessor traps */
	mov x1, #0x33ff
	msr cptr_el2, x1 /* Disable coprocessor traps to EL2 */
	msr hstr_el2, xzr /* Disable Hyp System Trap */

	mrs x1, cpacr_el1
	mov x1, #3 << 20
	msr cpacr_el1, x1 /* Enable FP/SIMD at EL1 */

	/* Initialize HCR_EL2 */
	mov x1, #(1 << 31) /* 64bit EL1 */
	orr x1, x1, #(1 << 29) /* Disable HVC */
	msr hcr_el2, x1

	/* SCTLR_EL1 initialization */
	mov x1, #0x0800
	movk x1, #0x30d0, lsl #16
	msr sctlr_el1, x1

	/* Return to the EL1_SP1 mode from EL2 */
	mov x1, sp
	msr sp_el1, x1 /* Migrate SP */
	mrs x1, vbar_el2
	msr vbar_el1, x1 /* Migrate VBAR */
	mov x1, #0x3c5
	msr spsr_el2, x1 /* EL1_SP1 | D | A | I | F */
	adr x1, 70f
	msr elr_el2, x1
	eret

	// jump to C code, should not return
70:	bl initcpu_other

	// sleep forever
80:	wfe
	b 80b




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
