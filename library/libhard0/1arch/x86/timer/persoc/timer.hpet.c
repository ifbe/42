#include "libhard.h"
u64 rdtsc();
int percpu_enableint(int apicid, int intvec, void* isr, int flag);
//
int localapic_coreid();
//
int irqchip_enableirq(int chip, int pin, int apicid, int intvec);
//
u32 msi_help_addr(int);
u32 msi_help_data(int, int, int);


struct timermmio{
//63:32	  Tn_INT_ROUTE_CAP	//Timer n Interrupt Routing Capability. If bit X is set in this field, it means that this timer can be mapped to IRQX line of I/O APIC.
//31:16	          Reserved
// . 15	Tn_FSB_INT_DEL_CAP	//If this read-only bit is 1, this timer supports FSB interrupt mapping.
// . 14	     Tn_FSB_EN_CNF	//If this bit is set to 1, this timer will use FSB interrupt mapping.
// 13:9	  Tn_INT_ROUTE_CNF	//This field indicates I/O APIC routing. Allowed values can be determined using Tn_INT_ROUTE_CAP. If an illegal value is written, then value read back from this field will not match the written value.
//    8	     Tn_32MODE_CNF	//For 64-bit timer, if this field is set, the timer will be forced to work in 32-bit mode. Otherwise it has no effect.
// .  7	          Reserved
// .  6	    Tn_VAL_SET_CNF	//This field is used to allow software to directly set periodic timer's accumulator. Detailed explanation is provided further in the article.
// .  5	       Tn_SIZE_CAP	//If this read-only bit is set to 1, the size of the timer is 64-bit. Otherwise, it's 32-bit.
// .  4	    Tn_PER_INT_CAP	//If this read-only bit is set to 1, this timer supports periodic mode.
// .  3	       Tn_TYPE_CNF	//If Tn_PER_INT_CAP is 1, then writing 1 to this field enables periodic timer and writing 0 enables non-periodic mode. Otherwise, this bit will be ignored and reading it will always return 0.
// .  2	    Tn_INT_ENB_CNF	//Setting this bit to 1 enables triggering of interrupts. Even if this bit is 0, this timer will still set Tn_INT_STS.
// .  1	   Tn_INT_TYPE_CNF	//0="this timer generates edge-triggered interrupts", 1="this timer generates level-triggered interrupts. When the interrupt is generated, Tn_INT_STS is set. If another interrupt occurs before that bit is cleared, the interrupt will remain active."
// .  0	          Reserved
	volatile u64 cfgcap;

	volatile u64 value;

//64:32 Tn_FSB_INT_ADDR	//Software sets this 32-bit field to indicate the location that the FSB interrupt message should be written to.
// 31:0 Tn_FSB_INT_VAL	//Software sets this 32-bit field to indicate that value that is written during the FSB interrupt message 
	volatile u32 msi_data;
	volatile u32 msi_addr;

	volatile u64 padd;
}__attribute__((packed));


struct hpetmmio{
//63:32	COUNTER_CLK_PERIOD	//Main counter tick period in femtoseconds (10^-15 seconds). Must not be zero, must be less or equal to 0x05F5E100, or 100 nanoseconds.
//31-16	         VENDOR_ID	//This field should be interpreted similarly to PCI's vendor ID.
// . 15	        LEG_RT_CAP	//If this bit is 1, HPET is capable of using "legacy replacement" mapping.
// . 14	          Reserved
// . 13	    COUNT_SIZE_CAP	//If this bit is 1, HPET main counter is capable of operating in 64 bit mode.
// 12:8	       NUM_TIM_CAP	//The amount of timers - 1.
//  7:0	            REV_ID	//Indicates which revision of the function is implemented; must not be 0.
	//0x00-0x0f
	volatile u32 cap_lo32;
	volatile u32 cap_hi32_COUNTER_CLK_PERIOD;
	u64 pad08;

//63:2	Reserved
// . 1	LEG_RT_CNF	//0 - "legacy replacement mapping is disabled", 1="legacy replacement mapping is enabled"
// . 0	ENABLE_CNF	//Overall enable	//0="main counter is halted, timer interrupts are disabled", 1="main counter is running, timer interrupts are allowed if enabled"
	//0x10-0x1f
	volatile u32 cfg_lo32;
	volatile u32 cfg_hi32;
	u64 pad18;

//63:32	Reserved
// .  n	Tn_INT_STS	//For level-triggered: the default value is 0. When a corresponding timer interrupt is active, this bit is set. If it is set, software can clear it by writing 1 to this bit. Writes of 0 have no effect, For edge-triggered: this bit should be ignored. It is always set to 0.
	//0x20-0x2f
	volatile u32 isr_lo32;
	volatile u32 isr_hi32;
	u64 pad28;

	//0x30-0xef
	u8 pad30[0xc0];

	//0xf0-0xff
	volatile u64 countervalue;
	u64 padf8;

	struct timermmio timer[32];
}__attribute__((packed));




static struct hpetmmio* mhpet;
static u64 irqcnt = 0;




struct atthattime{
u64 tsc;
u64 isr;
u64 counter;
u64 compare;
};
static struct atthattime aftersetup;

#define HPET_DEBUGIRQ
#ifdef HPET_DEBUGIRQ
static struct atthattime debugirq[3];
#endif



static void isr_hpet(void* p)
{
#ifdef HPET_DEBUGIRQ
	if(irqcnt < 3){
	if(mhpet && (0 == debugirq[irqcnt].compare)){
		struct timermmio* tmr = &mhpet->timer[0];
		debugirq[irqcnt].tsc = rdtsc();
		debugirq[irqcnt].isr = mhpet->isr_lo32;
		debugirq[irqcnt].counter = mhpet->countervalue;
		debugirq[irqcnt].compare = tmr->value;
		//logtoall("isr_hpet: tsc=%lld,isr=%x,counter=%lld,compare=%lld\n", rdtsc(), mhpet->isr_lo32, mhpet->countervalue, tmr->value);
	}
	}
#endif
	//if(0==(irqcnt%1000))logtoall("irqcnt=%d\n",irqcnt);

	irqcnt += 1;
}
u64 hpet_gettime_ms()
{
	return irqcnt;
}
u64 hpet_gettime_us()
{
	return irqcnt*1000;
}
u64 hpet_gettime_ns()
{
	return irqcnt*1000*1000;
}




int firstnonezerobit(u64 x)
{
	int j = 0;
	while(j<32){
		if(x&1)break;
		x >>= 1;
		j++;
	}
	return j;
}
void hpet_print_timer(struct hpetmmio* hpet, int id)
{
	struct timermmio* tmr = &hpet->timer[id];
	u32 cfg = tmr->cfgcap&0xffffffff;
	u32 pin = tmr->cfgcap>>32;
	logtoall("->timer%d@%p,cfgcap=%x,%x{\n", id, tmr, cfg, pin);
	logtoall("size:is64=%x,force32=%x\n", (cfg>>5)&1, (cfg>>8)&1);
	logtoall("periodic:cap=%x,en=%x,valset=%x\n", (cfg>>4)&1, (cfg>>3)&1, (cfg>>6)&1);
	logtoall("ioapic:cap:%x,using=%x\n", pin, (cfg>>9)&0x1f);
	logtoall("fsb:cap=%x,en=%x\n", (cfg>>15)&1, (cfg>>14)&1);
	logtoall("trigger:islevel=%x,en=%x\n", (cfg>>1)&1, (cfg>>2)&1);
	logtoall("}%d\n", id);
}
void hpet_print(struct hpetmmio* hpet)
{
	u32 cap_lo32 = hpet->cap_lo32;
	logtoall("vendor=%x, legacysupport=%x, cntszcap=%x, numtimer=%x, revision=%x\n",
		cap_lo32>>16,
		!!(cap_lo32&0x8000),
		!!(cap_lo32&0x2000),
		((cap_lo32>>8)&0x1f)+1,
		(cap_lo32&0xff)
	);

	u32 cap_hi32 = hpet->cap_hi32_COUNTER_CLK_PERIOD;
	logtoall("period=%d(fs)=%d(ns)\n", cap_hi32, cap_hi32/1000/1000);

	//logtoall("cap=%x,%d(ps)\n", hpet->cap_lo32, hpet->cap_hi32_COUNTER_CLK_PERIOD);
	logtoall("cfg=%x,%x\n", hpet->cfg_lo32, hpet->cfg_hi32);
	logtoall("isr=%x,%x\n", hpet->isr_lo32, hpet->isr_hi32);

	hpet_print_timer(hpet, 0);
	hpet_print_timer(hpet, 1);
	hpet_print_timer(hpet, 2);
}
void hpet_setup(struct hpetmmio* hpet)
{
	//stop
	hpet->cfg_lo32 &= 0xfffffffe;

	//0.prepare data
	u64 interval_between_counter = hpet->cap_hi32_COUNTER_CLK_PERIOD;
	u64 interval_between_interrupt = (u64)1000*1000*1000*1000;
	u64 delta = interval_between_interrupt/interval_between_counter;
	logtoall("%lld / %lld = %lld\n", interval_between_interrupt, interval_between_counter, delta);

	struct timermmio* tmr = &hpet->timer[0];

	//1.cpu
	logtoall("hpet_setup cpu\n");
	int cpu_apicid = localapic_coreid();		//target cpu id
	int cpu_intvec = 0x20;	//target cpu intvec
	percpu_enableint(cpu_apicid, cpu_intvec, isr_hpet, 0);

	u64 cfgcap;
	int fsb_cap = (tmr->cfgcap>>15)&1;
	if(fsb_cap){
		//2.irqchip
		u32 msi_addr = msi_help_addr(cpu_apicid);
		u32 msi_data = msi_help_data(cpu_intvec, 0, 0);
		logtoall("hpet_setup msi: addr=%x,data=%x\n", msi_addr, msi_data);

		tmr->msi_addr = msi_addr;
		tmr->msi_data = msi_data;

		//3.self
		cfgcap = tmr->cfgcap;
		cfgcap &= ~((u64)0x1f<<9);
		cfgcap |= (1 << 3) | (1 << 6) | (1<<14);
	}
	else{
		u64 ioapic_input = firstnonezerobit(tmr->cfgcap>>32);
		logtoall("hpet_setup ioapic:%llx\n", ioapic_input);

		//2.irqchip
		int irqchip_type = hex16('i','o');
		int irqchip_foot = ioapic_input;
		irqchip_enableirq(irqchip_type, irqchip_foot, cpu_apicid, cpu_intvec);

		//3.self
		cfgcap = tmr->cfgcap;
		cfgcap &= ~((u64)0x1f<<9);
		cfgcap |= (ioapic_input << 9) | (1 << 3) | (1 << 6);
	}

	u64 curval = hpet->countervalue;
	logtoall("hpet_setup before setval: tsc=%lld, cfgcap=%llx, counter=%lld, compare=%lld\n", rdtsc(), cfgcap, curval, tmr->value);

	tmr->cfgcap = cfgcap;	//dont enable irq: otherwise receive {counter=0,compare=-1}
	tmr->value = curval + delta;
	tmr->value = delta;
	tmr->cfgcap |= (1 << 2);	//enable irq after value set
	hpet->cfg_lo32 |= 1;
	logtoall("hpet_setup after setval: tsc=%lld, cfgcap=%llx, counter=%lld, compare=%lld\n", rdtsc(), tmr->cfgcap, hpet->countervalue, tmr->value);

	aftersetup.tsc = rdtsc();
	aftersetup.isr = hpet->isr_lo32;
	aftersetup.counter = hpet->countervalue;
	aftersetup.compare = tmr->value;
	logtoall("hpet_setup all done: tsc=%lld, isr=%llx, counter=%lld, compare=%lld\n", aftersetup.tsc, aftersetup.isr, aftersetup.counter, aftersetup.compare);
}
int hpet_check(struct hpetmmio* hpet)
{
	int ret;
	struct timermmio* tmr = &hpet->timer[0];
	u64 irq_c0 = irqcnt;		//before loop
	u64 tsc_t0 = rdtsc();
	u64 irq_cn = irq_c0 + 10;		//+10ms
	u64 tsc_tn = tsc_t0 + (u64)10*1000*1000*1000;
	u64 irq_chk = 0;		//check point
	u64 tsc_chk = 0;
	u64 irq_now = 0;		//now
	u64 tsc_now = 0;
	logtoall("waiting for something change: irqsum=%lld,tsc=%lld\n", irq_c0, tsc_t0);
	while(1){
		irq_now = irqcnt;
		tsc_now = rdtsc();
		if(irqcnt){				//irq come
			if(0 == irq_chk){	//not yet set value
				irq_chk = irq_now;
				tsc_chk = tsc_now;
			}
		}

		if(tsc_now > tsc_tn){		//print every 10G tsc
			logtoall("(maybe noirq)irqsum=%lld,tscval=%lld,counter=%lld,compare=%lld\n", irq_now, tsc_now, hpet->countervalue, tmr->value);
			tsc_tn += (u64)10*1000*1000*1000;
		}
		if(irq_now >= irq_cn){		//print every 10 irq
			logtoall("(look good)irqsum=%lld,tscval=%lld,counter=%lld,compare=%lld\n", irq_now, tsc_now, hpet->countervalue, tmr->value);
			irq_cn += 10;
		}
		if(irq_now >= irq_c0 + 200){		//success: over 200 irq
			logtoall("tsc_delta=%lld, irq_delta=%lld, avgfreq=%lldmhz\n", tsc_now-tsc_chk, irq_now-irq_chk, (tsc_now-tsc_chk)/(irq_now-irq_chk)/1000);
			ret = 1;
			break;
		}
		if(tsc_now > tsc_t0 + (u64)3*10*1000*1000*1000){	//abnormal: over 30G tsc
			logtoall("hpet fail\n");
			ret = 0;
			break;
		}
	}

#ifdef HPET_DEBUGIRQ
	logtoall("debugtirq0: tsc=%lld,isr=%llx,counter=%lld,compare=%lld\n", debugirq[0].tsc, debugirq[0].isr, debugirq[0].counter, debugirq[0].compare);
	logtoall("debugtirq1: tsc=%lld,isr=%llx,counter=%lld,compare=%lld\n", debugirq[1].tsc, debugirq[1].isr, debugirq[1].counter, debugirq[1].compare);
	logtoall("debugtirq2: tsc=%lld,isr=%llx,counter=%lld,compare=%lld\n", debugirq[2].tsc, debugirq[2].isr, debugirq[2].counter, debugirq[2].compare);
#endif

	return ret;
}
int inithpet(struct hpetmmio* hpet)
{
	logtoall("@inithpet, tsc=%llx\n", rdtsc());
	if(0 == hpet)return 0;

	mhpet = hpet;

	hpet_print(hpet);
	//logtoall("hpet fail\n\n");return 0;			//debug

	hpet_setup(hpet);

	int ret = hpet_check(hpet);

	logtoall("inithpet end\n\n");
	return ret;
}
