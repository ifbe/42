#include "libhard.h"
#define TRAMPOLINE16 0x8000
#define FromBsp_rip 0xffe0
#define FromBsp_rsp 0xffe8
#define ApToBsp_message 0xfff0
#define BspToAp_command 0xfff8
void cpuid(u32*);
void initpaging();
void initgdt();
void initidt();
//
void sleep_us();
//for fun
void enterring3();
//for multicore
int localapic_isenabled();
int localapic_coreid();
void localapic_sendinit(u32 apic_id);
void localapic_sendstart(u32 apic_id, u32 vector);
void* get_entry_ap_start();
void* get_entry_ap_end();




static u32 shit = 0;
static void initcpu_other()
{
	shit = hex32('f','u','c','k');
	//say("[from ap]: coreid=%x\n", localapic_coreid());

	while(1){
		*(u64*)0 += 97;		//test only
	}
}




void initcpu_bsp(struct item* p)
{
	//
	asm("cli");
	say("initing cpu0...\n");


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
	say("\n\n");
}




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
void initcpu_ap(int coreid)
{
	say("initing cpu%d...\n", coreid);
	if(0 == localapic_isenabled()){
		say("fail@no apic\n");
		return;
	}

	u8* entry_ap_start = get_entry_ap_start();
	u8* entry_ap_end   = get_entry_ap_end();
	u8* trampoline = (u8*)TRAMPOLINE16;
	int length = entry_ap_end - entry_ap_start;
	say("code16: [%p,%p) -> [%p,%p)\n", entry_ap_start, entry_ap_end, trampoline, trampoline+length);

	int j;
	for(j=0;j<length;j++)trampoline[j] = entry_ap_start[j];
	printmemory(trampoline, length);

	say("sending INIT IPI\n");
	//send INIT IPI
	localapic_sendinit(coreid);
	//wait 10ms
	sleep_us(10*1000);

	//prep flag
	volatile u8* flag = (u8*)ApToBsp_message;
	*flag = 0;

	//send SIPI IPI, wait for flag
	for(j=0;j<2;j++){
		say("sending START IPI %d\n",j);
		//send SIPI IPI
		localapic_sendstart(coreid, TRAMPOLINE16>>12);

		//wait 200us, check flag
		sleep_us(1000);
		say("flag=%x\n", *flag);
		if(16 == *flag){
			say("ap in 16bit mode\n");
			goto givecmdtoap;
		}
	}
	say("fail@AP silent\n");
	say("\n\n");
	return;

givecmdtoap:
	*(u64*)BspToAp_command = hex32('g','o','g','o');
	*(void**)FromBsp_rip = initcpu_other;
	*(void**)FromBsp_rsp = memorycreate(0x100000, 0) + 0x100000 - 16;

	//wait 200us, check flag
	sleep_us(1000);
	say("flag=%x\n", *flag);
	if(64 == *flag){
		say("ap in 64bit mode\n");
		goto allgood;
	}
	say("fail@AP error\n");
	say("\n\n");
	return;

allgood:
	say("ap working: %x\n", shit);
	say("\n\n");
}