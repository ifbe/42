#include "libhard.h"
extern void vectors();




void exception_spsel0_sync(u64 esr, u64 elr, u64 spsr, u64 far)
{
    say("@exception_spsel0_sync\n");
    for(;;);
}
void exception_spsel0_irq(u64 esr, u64 elr, u64 spsr, u64 far)
{
    say("@exception_spsel0_irq\n");
    for(;;);
}
void exception_spsel0_fiq(u64 esr, u64 elr, u64 spsr, u64 far)
{
    say("@exception_spsel0_fiq\n");
    for(;;);
}
void exception_spsel0_serr(u64 esr, u64 elr, u64 spsr, u64 far)
{
    say("@exception_spsel0_serr\n");
    for(;;);
}




void exception_spsel1_sync(u64 esr, u64 elr, u64 spsr, u64 far)
{
    say("@exception_spsel1_sync\n");
    say("esr=%llx,elr=%llx,spsr=%llx,far=%llx\n", esr, elr, spsr, far);
}
void exception_spsel1_irq(u64 esr, u64 elr, u64 spsr, u64 far)
{
    say("@exception_spsel1_irq\n");
    for(;;);
}
void exception_spsel1_fiq(u64 esr, u64 elr, u64 spsr, u64 far)
{
    say("@exception_spsel1_fiq\n");
    for(;;);
}
void exception_spsel1_serr(u64 esr, u64 elr, u64 spsr, u64 far)
{
    say("@exception_spsel1_serr\n");
    for(;;);
}




void exception_lowel64_sync(u64 esr, u64 elr, u64 spsr, u64 far)
{
    say("@exception_lowel64_sync\n");
    for(;;);
}
void exception_lowel64_irq(u64 esr, u64 elr, u64 spsr, u64 far)
{
    say("@exception_lowel64_irq\n");
    for(;;);
}
void exception_lowel64_fiq(u64 esr, u64 elr, u64 spsr, u64 far)
{
    say("@exception_lowel64_fiq\n");
    for(;;);
}
void exception_lowel64_serr(u64 esr, u64 elr, u64 spsr, u64 far)
{
    say("@exception_lowel64_serr\n");
    for(;;);
}




void exception_lowel32_sync(u64 esr, u64 elr, u64 spsr, u64 far)
{
    say("@exception_lowel32_sync\n");
    for(;;);
}
void exception_lowel32_irq(u64 esr, u64 elr, u64 spsr, u64 far)
{
    say("@exception_lowel32_irq\n");
    for(;;);
}
void exception_lowel32_fiq(u64 esr, u64 elr, u64 spsr, u64 far)
{
    say("@exception_lowel32_fiq\n");
    for(;;);
}
void exception_lowel32_serr(u64 esr, u64 elr, u64 spsr, u64 far)
{
    say("@exception_lowel32_serr\n");
    for(;;);
}




void initexception(void* sp)
{
    say("@initexception\n");

    u64 el;
    asm("mrs %0, CurrentEL" :"=r"(el));
    say("CurrentEL=%x\n", (el>>2)&3);

    //asm("ldr x0, %0; msr sp_el1, x0" : : "m"(sp));

    void* vec = vectors;
    asm("ldr x0, %0; msr vbar_el1, x0" : : "m"(vec));

    asm("svc #0");
}