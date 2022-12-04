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
//31-16	         VENDOR_ID	//This field should be interpreted similarly to PCI's vendor ID.
// . 15	        LEG_RT_CAP	//If this bit is 1, HPET is capable of using "legacy replacement" mapping.
// . 14	          Reserved
// . 13	    COUNT_SIZE_CAP	//If this bit is 1, HPET main counter is capable of operating in 64 bit mode.
// 12:8	       NUM_TIM_CAP	//The amount of timers - 1.
//  7:0	            REV_ID	//Indicates which revision of the function is implemented; must not be 0.
//63:32	COUNTER_CLK_PERIOD	//Main counter tick period in femtoseconds (10^-15 seconds). Must not be zero, must be less or equal to 0x05F5E100, or 100 nanoseconds.
	//0x00-0x0f
	u32 cap_lo32;
	u32 cap_hi32_COUNTER_CLK_PERIOD;
	u64 pad08;

//63:2	Reserved
// . 1	LEG_RT_CNF	//0 - "legacy replacement mapping is disabled", 1="legacy replacement mapping is enabled"
// . 0	ENABLE_CNF	//Overall enable	//0="main counter is halted, timer interrupts are disabled", 1="main counter is running, timer interrupts are allowed if enabled"
	//0x10-0x1f
	u32 cfg_lo32;
	u32 cfg_hi32;
	u64 pad18;

//63:32	Reserved
// .  n	Tn_INT_STS	//For level-triggered: the default value is 0. When a corresponding timer interrupt is active, this bit is set. If it is set, software can clear it by writing 1 to this bit. Writes of 0 have no effect, For edge-triggered: this bit should be ignored. It is always set to 0.
	//0x20-0x2f
	u32 isr_lo32;
	u32 isr_hi32;
	u64 pad28;

	//0x30-0xef
	u8 pad30[0xc0];

	//0xf0-0xff
	volatile u64 countervalue;
	u64 padf8;

	struct timermmio timer[32];
}__attribute__((packed));




static u64 cnt = 0;
static void isr_hpet(void* p)
{
	//if(0==(cnt%1000))say("cnt=%d\n",cnt);
	cnt += 1;
}
u64 hpet_gettime_ms()
{
	return cnt;
}
u64 hpet_gettime_us()
{
	return cnt*1000;
}
u64 hpet_gettime_ns()
{
	return cnt*1000*1000;
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
	say("->timer%d@%p,cfgcap=%x,%x{\n", id, tmr, cfg, pin);
	say("size:is64=%x,force32=%x\n", (cfg>>5)&1, (cfg>>8)&1);
	say("periodic:cap=%x,en=%x,valset=%x\n", (cfg>>4)&1, (cfg>>3)&1, (cfg>>6)&1);
	say("ioapic:cap:%x,using=%x\n", pin, (cfg>>9)&0x1f);
	say("fsb:cap=%x,en=%x\n", (cfg>>15)&1, (cfg>>14)&1);
	say("trigger:islevel=%x,en=%x\n", (cfg>>1)&1, (cfg>>2)&1);
	say("}%d\n", id);
}
void hpet_print(struct hpetmmio* hpet)
{
	u32 cap_lo32 = hpet->cap_lo32;
	say("vendor=%x, legacysupport=%x, cntszcap=%x, numtimer=%x, revision=%x\n",
		cap_lo32>>16,
		!!(cap_lo32&0x8000),
		!!(cap_lo32&0x2000),
		((cap_lo32>>8)&0x1f)+1,
		(cap_lo32&0xff)
	);

	u32 cap_hi32 = hpet->cap_hi32_COUNTER_CLK_PERIOD;
	say("period=%d(fs)=%d(ns)\n", cap_hi32, cap_hi32/1000/1000);

	//say("cap=%x,%d(ps)\n", hpet->cap_lo32, hpet->cap_hi32_COUNTER_CLK_PERIOD);
	say("cfg=%x,%x\n", hpet->cfg_lo32, hpet->cfg_hi32);
	say("isr=%x,%x\n", hpet->isr_lo32, hpet->isr_hi32);

	hpet_print_timer(hpet, 0);
	hpet_print_timer(hpet, 1);
	hpet_print_timer(hpet, 2);
}
void hpet_setup(struct hpetmmio* hpet)
{
	//0.prepare data
	u64 interval_between_counter = hpet->cap_hi32_COUNTER_CLK_PERIOD;
	u64 interval_between_interrupt = (u64)1000*1000*1000*1000;
	u64 delta = interval_between_interrupt/interval_between_counter;
	say("dc=%llx,di=%llx,delta=%llx\n", interval_between_counter, interval_between_interrupt, delta);

	u64 curval = hpet->countervalue;
	say("hpet_setup timer0, curval=%llx\n", curval);

	struct timermmio* tmr = &hpet->timer[0];

	//1.cpu
	say("hpet_setup cpu\n");
	int cpu_apicid = localapic_coreid();		//target cpu id
	int cpu_intvec = 0x20;	//target cpu intvec
	percpu_enableint(cpu_apicid, cpu_intvec, isr_hpet, 0);

	int fsb_cap = (tmr->cfgcap>>15)&1;
	if(fsb_cap){
		//2.irqchip
		u32 msi_addr = msi_help_addr(cpu_apicid);
		u32 msi_data = msi_help_data(cpu_intvec, 0, 0);
		say("msi: addr=%x,data=%x\n", msi_addr, msi_data);

		tmr->msi_addr = msi_addr;
		tmr->msi_data = msi_data;

		//3.self
		u64 cfgcap = tmr->cfgcap;
		cfgcap &= ~((u64)0x1f<<9);
		cfgcap |= (1 << 2) | (1 << 3) | (1 << 6) | (1<<14);
		tmr->cfgcap = cfgcap;
	}
	else{
		u64 ioapic_input = firstnonezerobit(tmr->cfgcap>>32);
		say("firstnonzerobit=%llx\n",ioapic_input);

		//2.irqchip
		say("hpet_setup irqchip\n");
		int irqchip_type = hex16('i','o');
		int irqchip_foot = ioapic_input;
		irqchip_enableirq(irqchip_type, irqchip_foot, cpu_apicid, cpu_intvec);

		//3.self
		u64 cfgcap = tmr->cfgcap;
		cfgcap &= ~((u64)0x1f<<9);
		cfgcap |= (ioapic_input << 9) | (1 << 2) | (1 << 3) | (1 << 6);
		tmr->cfgcap = cfgcap;
	}

	tmr->value = curval + delta;
	tmr->value = delta;

	hpet->cfg_lo32 |= 1;
}
int hpet_check(struct hpetmmio* hpet)
{
	//while(0 == cnt);
	//say("8254 works, cnt=%x, tsc=%llx\n\n", cnt, rdtsc());
	struct timermmio* tmr = &hpet->timer[0];

	u64 hpet_t0 = cnt;
	u64 hpet_tn = hpet_t0 + 10;		//10ms
	u64 hpet_now = 0;
	u64 tsc_t0 = rdtsc();
	u64 tsc_tn = tsc_t0 + (u64)10*1000*1000*1000;
	u64 tsc_now = 0;
	say("waiting (irqsum>100) or (tsc>10g): irqsum=%lld,tsc=%lld\n", hpet_t0, tsc_t0);
	while(1){
		hpet_now = cnt;
		tsc_now = rdtsc();

		if(tsc_now > tsc_tn){
			say("(maybe noirq)irqsum=%lld,tscval=%lld,main=%llx,accu=%llx\n", hpet_now, tsc_now, hpet->countervalue, tmr->value);
			tsc_tn += (u64)10*1000*1000*1000;
		}
		if(hpet_now >= hpet_tn){
			say("(look good)irqsum=%lld,tscval=%lld,main=%llx,accu=%llx\n", hpet_now, tsc_now, hpet->countervalue, tmr->value);
			hpet_tn += 10;
		}
		if(hpet_now >= hpet_t0 + 100){
			say("time_delta=0.1s, tsc_delta=%lld, avgfreq=%lldmhz\n", tsc_now - tsc_t0, (tsc_now-tsc_t0)*10/1000/1000);
			goto good;
		}
		if(tsc_now > tsc_t0 + (u64)3*10*1000*1000*1000){
			say("hpet fail\n");
			goto fail;
		}
	}
good:
	return 1;
fail:
	return 0;
}
int inithpet(struct hpetmmio* hpet)
{
	say("@inithpet, tsc=%llx\n", rdtsc());
	if(0 == hpet)return 0;

	hpet_print(hpet);
	//say("hpet fail\n\n");return 0;			//debug

	hpet_setup(hpet);

	int ret = hpet_check(hpet);

	say("inithpet end\n\n");
	return ret;
}