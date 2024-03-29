#include "libhard.h"
void gic4_inittimer();
void percpu_schedule(void* regs);




void wait_cycles(unsigned int n)
{
	if(0 == n)return;
	while(n--) { asm volatile("nop"); }
}
void wait_msec(unsigned int n)
{
	register u64 f, t, r;
	// get the current counter frequency
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
	// read the current counter
	asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
	// calculate expire value for counter
	t += ((f/1000)*n)/1000;
	do{asm volatile ("mrs %0, cntpct_el0" : "=r"(r));}while(r<t);
}




void percputimer_tval(int us)
{
	/*定时器使用细节可查看ARMv8体系结构手册*/
	unsigned long freq, cnt, cmp;

	//freq	
	asm volatile("mrs %0, CNTFRQ_EL0" : "=r" (freq));

	//1 freq = one second later
	//asm volatile("msr CNTP_TVAL_EL0, %0" :  :"r" (freq));

	//10ms later
	asm volatile("mrs %0, CNTPCT_EL0" : "=r" (cnt));
	cmp = cnt + (freq/1000)*us/1000;
	asm volatile("msr CNTP_CVAL_EL0, %0" :  :"r" (cmp));
}
void percputimer_isr(void* regs)
{
	percpu_schedule(regs);
	percputimer_tval(10*1000);
}




u64 archtimer_ms()
{
	u64 freq,pcnt;
	asm volatile("mrs %0, CNTFRQ_EL0" : "=r" (freq));
	asm volatile("mrs %0, CNTPCT_EL0" : "=r" (pcnt));
	return (1000*pcnt) / freq;
}
u64 archtimer_us()
{
	u64 freq,pcnt;
	asm volatile("mrs %0, CNTFRQ_EL0" : "=r" (freq));
	asm volatile("mrs %0, CNTPCT_EL0" : "=r" (pcnt));
	return (1000*1000*pcnt) / freq;
}
u64 archtimer_ns()
{
	u64 freq,pcnt;
	asm volatile("mrs %0, CNTFRQ_EL0" : "=r" (freq));
	asm volatile("mrs %0, CNTPCT_EL0" : "=r" (pcnt));
	return (1000*1000*pcnt) / (freq/1000);
}




void percputimer_exit()
{
	unsigned long ctl = 0;
	asm volatile("msr CNTP_CTL_EL0, %0" : : "r" (ctl));
}
void percputimer_init()
{
	unsigned long ctl,freq, pcnt,vcnt, cval,tval;
	asm volatile("mrs %0, CNTP_CTL_EL0" : "=r" (ctl));
	asm volatile("mrs %0, CNTFRQ_EL0" : "=r" (freq));
	asm volatile("mrs %0, CNTPCT_EL0" : "=r" (pcnt));
	asm volatile("mrs %0, CNTVCT_EL0" : "=r" (vcnt));
	asm volatile("mrs %0, CNTP_CVAL_EL0" : "=r" (cval));
	asm volatile("mrs %0, CNTP_TVAL_EL0" : "=r" (tval));
	logtoall("@generictimer: ctl=%x,freq=%x, pcnt=%x,vcnt=%x, cval=%x,tval=%x\n", ctl,freq, pcnt,vcnt, cval,tval);

	//stop
	ctl = 0;
	asm volatile("msr CNTP_CTL_EL0, %0" : : "r" (ctl));

	//100us
	percputimer_tval(10*1000);

	//start
	ctl = 1;
	asm volatile("msr CNTP_CTL_EL0, %0" : : "r" (ctl));

	//gic_enirq(30)
	gic4_inittimer();
}