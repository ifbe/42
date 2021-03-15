#include "libhard.h"
#define TRAMPOLINE16 0x8000
#define TRAMPOLINE64 0x9000
#define FromBsp_rsp 0xffe0
#define FromBsp_rip 0xffe8
#define ApToBsp_message 0xfff0
#define BspToAp_command 0xfff8
//
void cpuid(u32*);
int enable_fpu();
int enable_sse();
int enable_xsave();
//
void initpaging();
void initgdt();
//
void initidt_bsp();
void initidt_ap(int coreid);
//
void localapic_init();
void apictimer_init();
//
void process_registersupplier(int);
void thread_registersupplier(int);
//acpi
u64 getknowncores();
//
void sleep_us();
//for fun
void enterring3();
//for multicore
int localapic_isenabled();
int localapic_coreid();
void localapic_sendinit(u32 apic_id);
void localapic_sendstart(u32 apic_id, u32 vector);
//
void* get_trampoline16_start();
void* get_trampoline16_end();
void* get_trampoline64_start();
void* get_trampoline64_end();




void initcpu_bsp(struct item* p)
{
	//
	asm("cli");
	say("@initcpu_bsp.in\n");


//----------------prep descs----------------
	initpaging();
	initgdt();


	if(!enable_fpu())say("fail@enable_fpu\n");
	if(!enable_sse())say("fail@enable_sse\n");
	if(!enable_xsave())say("fail@enable_xsave\n");


//----------------check cpuid----------------
	u32 abcd[4];
	abcd[0] = 0;
	cpuid(abcd);
	say("cpuid.0: eax=%08x,str=%.4s%.4s%.4s\n",abcd[0], &abcd[1],&abcd[3],&abcd[2]);
	abcd[0] = 1;
	cpuid(abcd);
	say("cpuid.1: eax=%08x,ebx=%08x,ecx=%08x,edx=%08x\n",abcd[0],abcd[1],abcd[2],abcd[3]);


//----------------check coreid----------------
	localapic_init();

	int coreid = localapic_coreid();
	say("coreid = %d\n", coreid);

	process_registersupplier(coreid);
	thread_registersupplier(coreid);

	initidt_bsp();
	apictimer_init();


	//ok
	say("@initcpu_bsp.end\n\n");
	asm("sti");
}




void initcpu_other()
{
	asm("cli");
	say("@initcpu_other.in\n");


//----------------prep descs----------------
	//initpaging();		//currently same as bsp
	//initgdt();


	if(!enable_fpu())say("fail@enable_fpu\n");
	if(!enable_sse())say("fail@enable_sse\n");
	if(!enable_xsave())say("fail@enable_xsave\n");


//----------------check cpuid----------------
	u32 abcd[4];
	abcd[0] = 0;
	cpuid(abcd);
	say("cpuid.0: eax=%08x,str=%.4s%.4s%.4s\n",abcd[0], &abcd[1],&abcd[3],&abcd[2]);
	abcd[0] = 1;
	cpuid(abcd);
	say("cpuid.1: eax=%08x,ebx=%08x,ecx=%08x,edx=%08x\n",abcd[0],abcd[1],abcd[2],abcd[3]);


//----------------check coreid----------------
	localapic_init();

	int coreid = localapic_coreid();
	say("coreid=%d\n", coreid);

	thread_registersupplier(coreid);

	initidt_ap(coreid);
	apictimer_init();


	//ok
	say("@initcpu_other.end\n\n", coreid);
	asm("sti");
}




static u32 shit = 0;
static void trampoline_appcpu()
{
//----------------set flag--------------
	asm("cli");
	shit = hex32('f','u','c','k');

//----------------init cpu---------------
	initcpu_other();

//----------------goto sleep----------------
	say("appcpu: sleep wait for int\n");
	while(1)asm("hlt");
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
void initcpu_onecore(int coreid)
{
	say("wakeup cpu@%d...\n", coreid);
	if(0 == localapic_isenabled()){
		say("fail@no apic\n");
		return;
	}

	u8* trampoline16_start = get_trampoline16_start();
	u8* trampoline16_end   = get_trampoline16_end();
	u8* trampoline64_start = get_trampoline64_start();
	u8* trampoline64_end   = get_trampoline64_end();

	int j;
	u8* buf = (u8*)TRAMPOLINE16;
	int len = trampoline16_end - trampoline16_start;
	for(j=0;j<len;j++)buf[j] = trampoline16_start[j];
	say("trampoline16: [%p,%p) -> [%p,%p)\n", trampoline16_start, trampoline16_end, buf, buf+len);
	printmemory(buf, len);

	buf = (u8*)TRAMPOLINE64;
	len = trampoline64_end - trampoline64_start;
	for(j=0;j<len;j++)buf[j] = trampoline64_start[j];
	say("trampoline64: [%p,%p) -> [%p,%p)\n", trampoline64_start, trampoline64_end, buf, buf+len);
	printmemory(buf, len);

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
	*(volatile u64*)FromBsp_rip = (u64)trampoline_appcpu;
	*(volatile u64*)FromBsp_rsp = (u64)memorycreate(0x100000, 0) + 0x100000 - 0x100;
	say("rip=%llx,rsp=%llx\n", *(volatile u64*)FromBsp_rip, *(volatile u64*)FromBsp_rsp);
	*(volatile u64*)BspToAp_command = hex32('g','o','g','o');		//must after rip & rsp

	//bsp wait 2s, until ap done, then check flag
	sleep_us(1000*2000);
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




void initcpu_ap()
{
	u64 all = getknowncores();
	int bsp = localapic_coreid();
	say("knowncores=%llx, bsp=%x\n", all, bsp);

	int j;
	for(j=0;j<64;j++){
		if(0 == (all & (1<<j)))continue;
		if(j == bsp)continue;

		initcpu_onecore(j);
		break;
	}
}




int percpucoreid()
{
	return localapic_coreid();
}