#include "libhard.h"
void cpuid(u32*);
void initpaging();
void initgdt();
void initidt();
//for fun
void enterring3();
//for multicore
void* get_entry_ap_start();
void* get_entry_ap_end();




//BSP:send INIT IPI and SIPI IPI
//AP: set flag "AP started"
//BSP:check flag "AP started"
//		if(yes)alloc buffer as stack, write stack to trampoline, set flag "AP continue"
//		else resend SIPI IPI
//AP: check flag "AP continue"
//		if(yes)from real mode into long mode, load stack from trampoline, set flag "AP ready"
//		else wait for flag
//BSP:checkflag "AP ready"
//		if(yes)thiscore done
//		else thiscore fail
void initcpun(int coreid)
{
	void* entry_ap_start = get_entry_ap_start();
	void* entry_ap_end   = get_entry_ap_end();
	say("start@%p, end@%p\n", entry_ap_start, entry_ap_end);
	printmemory(entry_ap_start, entry_ap_end-entry_ap_start);

	//1.send INIT IPI
	//2.wait 10ms
	//3.send SIPI IPI
	//4.wait 200us
}
void initcpu0(struct item* p)
{
	//
	asm("cli");


//----------------check cpuid----------------
	u32 abcd[4];
	abcd[0] = 0;
	cpuid(abcd);
	say("cpuid.0: eax=%08x,str=%.4s%.4s%.4s\n",abcd[0], &abcd[1],&abcd[3],&abcd[2]);
	abcd[0] = 1;
	cpuid(abcd);
	say("cpuid.1: eax=%08x,ebx=%08x,ecx=%08x,edx=%08x\n",abcd[0],abcd[1],abcd[2],abcd[3]);


//----------------prep descs----------------
	initpaging();
	initgdt();
	initidt();


//----------------jump to ring3----------------
	say("ring3 try...\n");
	enterring3();
	dbg("ring3 god!!!\n");


//----------------back to ring0----------------
	dbg("ring0 try...\n");
	asm("int $0x80");
	say("ring0 god!!!\n");


	//ok
	asm("sti");
}
void initcpu(struct item* p)
{
	say("initing cpu0...\n");
	initcpu0(p);

	say("initing cpu%d...\n",1);
	initcpun(1);
}