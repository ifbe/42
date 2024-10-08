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
20:	cbz     w2, 70f
	str     xzr, [x1], #8
	sub     w2, w2, #1
	cbnz    w2, 20b

	// jump to C code, should not return
70:	bl main

	// sleep forever
80:	wfe
	b 80b
