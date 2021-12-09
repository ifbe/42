#include "libhard.h"
void gic4_isr(void* regs);
void syscall_handler(void* regs);




void exception_spsel0_sync(void* regs)
{
	say("@exception_spsel0_sync\n");
	for(;;);
}
void exception_spsel0_irq(void* regs)
{
	say("@exception_spsel0_irq\n");
	for(;;);
}
void exception_spsel0_fiq(void* regs)
{
	say("@exception_spsel0_fiq\n");
	for(;;);
}
void exception_spsel0_serr(void* regs)
{
	say("@exception_spsel0_serr\n");
	for(;;);
}




void exception_spsel1_sync(void* regs)
{
	u64 esr,far;
	asm("mrs %0, esr_el1" : "=r"(esr));
	asm("mrs %0, far_el1" : "=r"(far));
	say("@exception_spsel1_sync: esr=%llx,far=%llx\n", esr, far);

	switch(esr>>26){
	case 0x15:syscall_handler(regs);return;
	}
	for(;;);
}
void exception_spsel1_irq(void* regs)
{
	//say("@exception_spsel1_irq: esr=%llx,far=%llx\n", esr, elr, spsr, far);
	gic4_isr(regs);
}
void exception_spsel1_fiq(void* regs)
{
	say("@exception_spsel1_fiq\n");
	for(;;);
}
void exception_spsel1_serr(void* regs)
{
	say("@exception_spsel1_serr\n");
	for(;;);
}




void exception_lowel64_sync(void* regs)
{
	u64 esr,far;
	asm("mrs %0, esr_el1" : "=r"(esr));
	asm("mrs %0, far_el1" : "=r"(far));
	say("@exception_lowel64_sync: esr=%llx,far=%llx\n", esr, far);

	switch(esr>>26){
	case 0x15:syscall_handler(regs);return;
	}

	u64 elr,spsr;
	u64 ttbr0,ttbr1;
	if(0x24 == (esr>>26)){
		asm("mrs %0, elr_el1" : "=r"(elr));
		asm("mrs %0, spsr_el1" : "=r"(spsr));
		say("@elr=%llx,spsr=%llx\n", elr, spsr);
		printmemory((void*)elr-4, 0x100);

		asm("mrs %0, ttbr0_el1" : "=r"(ttbr0));
		asm("mrs %0, ttbr1_el1" : "=r"(ttbr1));
		say("@ttbr0=%llx,ttbr1=%llx\n", ttbr0, ttbr1);
	}
	for(;;);
}
void exception_lowel64_irq(void* regs)
{
	//say("@exception_lowel64_irq\n");
	gic4_isr(regs);
}
void exception_lowel64_fiq(void* regs)
{
	say("@exception_lowel64_fiq\n");
	for(;;);
}
void exception_lowel64_serr(void* regs)
{
	say("@exception_lowel64_serr\n");
	for(;;);
}




void exception_lowel32_sync(void* regs)
{
	say("@exception_lowel32_sync\n");
	for(;;);
}
void exception_lowel32_irq(void* regs)
{
	say("@exception_lowel32_irq\n");
	for(;;);
}
void exception_lowel32_fiq(void* regs)
{
	say("@exception_lowel32_fiq\n");
	for(;;);
}
void exception_lowel32_serr(void* regs)
{
	say("@exception_lowel32_serr\n");
	for(;;);
}




void initexception(void* vec)
{
	say("@initexception: vector@%p\n", vec);

	asm volatile("ldr x0, %0; msr vbar_el1, x0" : : "m"(vec));

	asm volatile("mov x8, #0x6576; movk x8, #0x72, lsl #16; svc #0");
}