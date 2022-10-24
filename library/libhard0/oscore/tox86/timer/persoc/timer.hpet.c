#include "libhard.h"
u64 rdtsc();


struct hpetmmio{
//31-16	VENDOR_ID	This field should be interpreted similarly to PCI's vendor ID.
//15	LEG_RT_CAP	If this bit is 1, HPET is capable of using "legacy replacement" mapping.
//14	Reserved	-
//13	COUNT_SIZE_CAP	If this bit is 1, HPET main counter is capable of operating in 64 bit mode.
//12:8	NUM_TIM_CAP	The amount of timers - 1.
//7:0	REV_ID	Indicates which revision of the function is implemented; must not be 0.
	u32 cap_lo32;
//63:32	COUNTER_CLK_PERIOD	Main counter tick period in femtoseconds (10^-15 seconds). Must not be zero, must be less or equal to 0x05F5E100, or 100 nanoseconds.
	u32 cap_hi32_COUNTER_CLK_PERIOD;
	u64 pad08;

	u32 cfg_lo32;
	u32 cfg_hi32;
	u64 pad18;

	u32 isr_lo32;
	u32 isr_hi32;
	u64 pad28;
}__attribute__((packed));


void inithpet(struct hpetmmio* hpet)
{
	say("@inithpet, tsc=%llx\n", rdtsc());
	if(0 == hpet)return;

	u32 cap_lo32 = hpet->cap_lo32;
	say("vendor=%x, legacysupport=%x, cntszcap=%x, numtimcap=%x, revision=%x\n",
		cap_lo32>>16,
		!!(cap_lo32&0x8000),
		!!(cap_lo32&0x2000),
		(cap_lo32>>8)&0x1f,
		(cap_lo32&0xff)
	);

	u32 cap_hi32 = hpet->cap_hi32_COUNTER_CLK_PERIOD;
	say("period=%d(fs)=%d(ns)\n", cap_hi32, cap_hi32/1000/1000);

	//say("cap=%x,%d(ps)\n", hpet->cap_lo32, hpet->cap_hi32_COUNTER_CLK_PERIOD);
	say("cfg=%x,%x\n", hpet->cfg_lo32, hpet->cfg_hi32);
	say("isr=%x,%x\n", hpet->isr_lo32, hpet->isr_hi32);
}