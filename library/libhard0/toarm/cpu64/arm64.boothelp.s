

.section ".text"
.global trampoline64
trampoline64:
    ldr x1, =0x4000
    mov sp, x1

    bl initcpu_other


.global percpu_inittimer
percpu_inittimer:
    mrs x1, CNTFRQ_EL0
    msr CNTP_TVAL_EL0, x1
    mov x0, 1
    msr CNTP_CTL_EL0, x0
    ret
