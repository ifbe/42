#include "libhard.h"
void* mmiobase();


#define SYSTMR 0x3000
//
#define CS   0x00	//System Timer Control/Status
#define CNTL 0x04	//System Timer Counter Lower 32 bits
#define CNTH 0x08	//System Timer Counter Higher 32 bits
#define CMP0 0x0c	//System Timer Compare 0
#define CMP1 0x10	//System Timer Compare 1
#define CMP2 0x14	//System Timer Compare 2
#define CMP3 0x18	//System Timer Compare 3
//
#define SYSTMR_LO	(*(volatile unsigned int*)(mmio+CNTL))
#define SYSTMR_HI	(*(volatile unsigned int*)(mmio+CNTH))




static void* mmio = 0;




u64 get_system_timer()
{
	unsigned int h=-1, l;

	// we must read MMIO area as two separate 32 bit reads
	h = SYSTMR_HI;
	l = SYSTMR_LO;

	// we have to repeat it if high word changed during read
	if(h != SYSTMR_HI) {
		h = SYSTMR_HI;
		l = SYSTMR_LO;
	}

	// compose long int value
	return ((u64)h << 32) | l;
}
void wait_msec_st(unsigned int n)
{
	// we must check if it's non-zero, because qemu does not emulate
	// system timer, and returning constant zero would mean infinite loop
	u64 t = get_system_timer();
	if(0 == t)return;

	while(get_system_timer() < t+n);
}




void initsystmr()
{
	logtoall("@initsystmr\n");
	mmio = mmiobase() + SYSTMR;
	logtoall("\n");
}