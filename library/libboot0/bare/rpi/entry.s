//AArch64, @0x80000
.section ".text.boot"

.globl _start
_start:
	//if(0 != id)goto sleep
00:	mrs     x1, mpidr_el1
	and     x1, x1, #3
	cbnz    x1, 80f

	// set stack before _start
10:	ldr     x1, =_start
	mov     sp, x1

	// clear bss
	ldr     x1, =__bss_start
	ldr     w2, =__bss_size
20:	cbz     w2, 30f
	str     xzr, [x1], #8
	sub     w2, w2, #1
	cbnz    w2, 20b

	//if(el3)
30:	mrs     x1, CurrentEL
	and     x1, x1, #12		// x1 &= 0xc
	cmp     x1, #12			// 12 means EL3
	b.ne    50f

	//el3_to_el2
40:	mov     x1, #0x5b1
	msr     scr_el3, x1
	mov     x1, #0x3c9
	msr     spsr_el3, x1
	adr     x1, 50f
	msr     elr_el3, x1
	eret

	//if(el2)
50:	mrs     x1, CurrentEL
	and     x1, x1, #12		// x1 &= 0xc
	cmp     x1, #8			// 8 means EL2
	b.ne    70f

	//el2_to_el1
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
70:	bl main

	// sleep forever
80:	wfe
	b 80b
