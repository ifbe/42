#include "libhard.h"
u64 rdtsc();
int percpu_enableint(int apicid, int intvec, void* isr, int flag);
int irqchip_enableirq(int chip, int pin, int apicid, int intvec);


struct timermmio{
	volatile u64 cfgcap;
	volatile u64 value;
	volatile u64 fsbintroute;
	volatile u64 padd;
}__attribute__((packed));


struct hpetmmio{
//31-16	VENDOR_ID	This field should be interpreted similarly to PCI's vendor ID.
//15	LEG_RT_CAP	If this bit is 1, HPET is capable of using "legacy replacement" mapping.
//14	Reserved	-
//13	COUNT_SIZE_CAP	If this bit is 1, HPET main counter is capable of operating in 64 bit mode.
//12:8	NUM_TIM_CAP	The amount of timers - 1.
//7:0	REV_ID	Indicates which revision of the function is implemented; must not be 0.
//63:32	COUNTER_CLK_PERIOD	Main counter tick period in femtoseconds (10^-15 seconds). Must not be zero, must be less or equal to 0x05F5E100, or 100 nanoseconds.
	//0x00-0x0f
	u32 cap_lo32;
	u32 cap_hi32_COUNTER_CLK_PERIOD;
	u64 pad08;

	//0x10-0x1f
	u32 cfg_lo32;
	u32 cfg_hi32;
	u64 pad18;

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
	say("->timer%d@%p\n", id, tmr);
	say("int_type=%x,int_en=%x\n", !!(tmr->cfgcap&2), !!(tmr->cfgcap&4));
	say("periodic_cap=%x,periodic_en=%x\n", !!(tmr->cfgcap&16), !!(tmr->cfgcap&8));
	say("is64=%x,force32=%x\n", !!(tmr->cfgcap&0x20), !!(tmr->cfgcap&0x100));
	say("writable=%x\n", !!(tmr->cfgcap&0x40));
	say("fsb_cap=%x,fsb_en=%x\n", !!(tmr->cfgcap&0x80), !!(tmr->cfgcap&0x40));
	say("ioapic_cap:%x,ioapic_using=%x\n", tmr->cfgcap>>32, (tmr->cfgcap>>9)&0x1f);
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
	//1.cpu
	say("hpet_setup cpu\n");
	int cpu_apicid = 0;		//target cpu id
	int cpu_intvec = 0x20;	//target cpu intvec
	percpu_enableint(cpu_apicid, cpu_intvec, isr_hpet, 0);

	//2.irqchip
	say("hpet_setup irqchip\n");
	//int irqchip_type = hex32('i','o','a','p','i','c');
	//int irqchip_foot = 2;		//gsi2(ioapic2)
	int irqchip_type = hex16('i','o');
	int irqchip_foot = 2;		//isa0
	irqchip_enableirq(irqchip_type, irqchip_foot, cpu_apicid, cpu_intvec);

	//3.self
	u64 interval_between_counter = hpet->cap_hi32_COUNTER_CLK_PERIOD;
	u64 interval_between_interrupt = (u64)1000*1000*1000*1000;
	u64 delta = interval_between_interrupt/interval_between_counter;
	say("dc=%llx,di=%llx,delta=%llx\n", interval_between_counter, interval_between_interrupt, delta);

	u64 curval = hpet->countervalue;
	say("hpet_setup timer0, curval=%llx\n", curval);

	struct timermmio* tmr = &hpet->timer[0];
	u64 ioapic_input = firstnonezerobit(tmr->cfgcap>>32);
	say("firstnonzerobit=%llx\n",ioapic_input);
	u64 cfgcap = tmr->cfgcap;
	cfgcap &= ~((u64)0x1f<<9);
	cfgcap |= (ioapic_input << 9) | (1 << 2) | (1 << 3) | (1 << 6);
	tmr->cfgcap = cfgcap;
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
			say("maybe noirq: irqsum=%lld,tsc=%lld\n", hpet_now, tsc_now);
			tsc_tn += (u64)10*1000*1000*1000;
		}
		if(hpet_now >= hpet_tn){
			say("hpet=%lld,tsc=%lld,main=%llx,accu=%llx\n", hpet_now, tsc_now, hpet->countervalue, tmr->value);
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