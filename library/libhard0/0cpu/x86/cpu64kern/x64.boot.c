#include "libhard.h"
//
#define APPCPU 0x20000		//[20000,2ffff]
#define APPCPU_GDT (APPCPU+0x000)
#define APPCPU_TSS (APPCPU+0x1000)
#define APPCPU_IDT (APPCPU+0x2000)
#define APPCPU_RSP (APPCPU+0x10000)
//
#define BSPCPU 0x30000		//[30000,3ffff]
#define BSPCPU_GDT (BSPCPU+0x000)
#define BSPCPU_TSS (BSPCPU+0x1000)
#define BSPCPU_IDT (BSPCPU+0x2000)
#define BSPCPU_RSP (BSPCPU+0x10000)
//
#define CR3BUF 0x40000
#define CR3END 0x80000



#define TRAMPOLINE16 0x8000
#define TRAMPOLINE64 0x9000
//
#define FromBsp_rsp 0xffe0
#define FromBsp_rip 0xffe8
#define ApToBsp_message 0xfff0
#define BspToAp_command 0xfff8
//
#define APMSG_16bit 16
#define APMSG_64bit 64
#define APMSG_asmtoc 88
#define APMSG_finish 0xff
void sendmsg_ap_to_bsp(int val)
{
	*(volatile u64*)ApToBsp_message = val;
}
int get_msg_from_ap()
{
	return *(volatile u64*)ApToBsp_message;
}
#define BSPCMD_16to64 64
#define BSPCMD_asmtoc 88
#define BSPCMD_finish 0xff
void msg_bsp_to_ap(int val)
{
	*(volatile u64*)BspToAp_command = val;
}
int get_msg_from_bsp()
{
	return *(volatile u64*)BspToAp_command;
}




//acpi
u64 acpi_getknowncores();
void cpuid(u32*);
//
int enable_fpu();
int enable_sse();
int enable_xsave();
void fpu_fxsave(u64 addr);
void fpu_fxrstor(u64 addr);
void fpu_xsave(u64 addr);
void fpu_rstor(u64 addr);
//
void initpaging(void*, int, void*, int);
void pagetable_use(u8* cr3);
//
void initgdt(void*);
void initgdt_ap(void*);
//
void initidt_bsp();
void initidt_ap(int coreid);
//for multicore
void localapic_init();
int localapic_isenabled();
int localapic_coreid();
int localapic_sendinit(u32 apic_id);
int localapic_sendstart(u32 apic_id, u32 vector);
//
void apictimer_init();
//
void* get_trampoline16_start();
void* get_trampoline16_end();
void* get_trampoline64_start();
void* get_trampoline64_end();
//
void process_registersupplier(int core, void* pt);
void process_switchto(int curr, int next);
//
int thread_registerprocessor(int core, int proc);
int thread_switchto(int queueid,int curr, int queueid2,int next, int coreid,void* cpureg);
int thread_findnext(int queueid,int curr);
int thread_findproc(int queueid,int curr);




//
struct saved_cpureg{
	//[0,3f]
	u64 r8;
	u64 r9;
	u64 r10;
	u64 r11;
	u64 r12;
	u64 r13;
	u64 r14;
	u64 r15;
	//[40,77]
	u64 rax;
	u64 rbx;
	u64 rcx;
	u64 rdx;
	u64 rsi;
	u64 rdi;
	u64 rbp;
	//[78,9f]
	u64 ip;
	u64 cs;
	u64 flag;
	u64 sp;
	u64 ss;
	//[a0,??]
}__attribute__((packed));

struct percpu{
	u64 gdt;
	u64 tss;
	u64 idt;
	u64 ring0stack;

	u32 irqcnt;
	u32 slpcnt;

	int coreid;

	//this core current thread
	int thread;
	int queueid;

	//this core current process
	int procid;

	//virtual machine
	int vmid;

	//manage mode
	int smid;
};
static struct percpu cpubuf[8];
static int cpucnt = 0;




#define KERNCODE 0x10
#define KERNDATA 0x18
#define USERCODE 0x20
#define USERDATA 0x28
void percpu_makeuser(struct saved_cpureg* cpu, u64* arg, u64 ip, u64 sp, u64 ip_pa, u64 sp_pa)
{
	cpu->rdi = arg[0];	//di,si,dx,cx,r8,r9
	cpu->rcx = arg[0];	//cx,dx,r8,r9

	cpu->ip = ip;
	cpu->cs = USERCODE | 3;
	cpu->flag = 0x202;
	cpu->sp = sp;
	cpu->ss = USERDATA | 3;
}
void percpu_makekern(struct saved_cpureg* cpu, u64* arg, u64 ip, u64 sp)
{
	cpu->rdi = arg[0];	//di,si,dx,cx,r8,r9
	cpu->rcx = arg[0];	//cx,dx,r8,r9

	cpu->ip = ip;
	cpu->cs = KERNCODE;
	cpu->flag = 0x202;
	cpu->sp = sp;
	cpu->ss = KERNDATA;
}




//cpu register
void percpu_savecpu(u64* saveaddr, u64* workaddr)
{
	int j;
	for(j=0;j<sizeof(struct saved_cpureg)/8;j++)saveaddr[j] = workaddr[j];
}
void percpu_loadcpu(u64* saveaddr, u64* workaddr)
{
	int j;
	for(j=0;j<sizeof(struct saved_cpureg)/8;j++)workaddr[j] = saveaddr[j];
}




//fpu register
void percpu_savefpu(u64 addr)
{
	fpu_fxsave(addr);
}
void percpu_loadfpu(u64 addr)
{
	fpu_fxrstor(addr);
}




int percpu_coreid()
{
	return localapic_coreid();
}
void* percpu_finddatabyid(int coreid)
{
	int j;
	for(j=0;j<cpucnt;j++){
		if(coreid == cpubuf[j].coreid)return &cpubuf[j];
	}
	return 0;
}
int percpu_process()
{
	int coreid = localapic_coreid();

	struct percpu* per = percpu_finddatabyid(coreid);

	return per->procid;
}
int percpu_tqueue()
{
	int coreid = localapic_coreid();

	struct percpu* per = percpu_finddatabyid(coreid);

	return per->queueid;
}
int percpu_thread()
{
	int coreid = localapic_coreid();

	struct percpu* per = percpu_finddatabyid(coreid);

	return per->thread;
}
int percpu_schedule(struct saved_cpureg* cpureg)
{
	//percpu coreid
	int coreid = localapic_coreid();
	//if(wierd)return 0;

	//percpu data
	struct percpu* per = percpu_finddatabyid(coreid);
	if(0 == per)return 0;

	//thread_schedule(cpureg);
	//return 0;

	//check thread
	int tcurr;
	int tnext;
	tcurr = per->thread;

	per->irqcnt += 1;
	if(0 == (per->irqcnt&0xf)){
		tnext = 0;
	}
	else{
		tnext = thread_findnext(per->queueid, tcurr);
	}
	//*(int*)(coreid*16 + 0) = tcurr;
	//*(int*)(coreid*16 + 4) = tnext;
	if(tcurr == tnext)return 0;

	//check process
	int pcurr = per->procid;
	int pnext = thread_findproc(per->queueid,tnext);

	//switch process
	if(pcurr != pnext){
		process_switchto(pcurr, pnext);
		per->procid = pnext;
	}

	//switch thread
	thread_switchto(per->queueid,tcurr, per->queueid,tnext, coreid,cpureg);
	per->thread = tnext;

	return 0;
}




void initcpu_bsp(struct item* p)
{
	//
	asm("cli");
	logtoall("@initcpu_bsp.in\n");


//----------------prep descs----------------
	//paging mapping: 1T space=[0,0x100,0000,0000)
	u8* pdir = memoryalloc(0x400000, 0x100000);
	initpaging((void*)CR3BUF, 0x10000, pdir, 0x400000);

	//gdt
	initgdt((void*)BSPCPU_GDT);

	if(!enable_fpu())logtoall("fail@enable_fpu\n");
	if(!enable_sse())logtoall("fail@enable_sse\n");
	if(!enable_xsave())logtoall("fail@enable_xsave\n");


//----------------check cpuid----------------
	u32 abcd[4];
	abcd[0] = 0;
	cpuid(abcd);
	logtoall("cpuid.0: eax=%08x,str=%.4s%.4s%.4s\n",abcd[0], &abcd[1],&abcd[3],&abcd[2]);
	abcd[0] = 1;
	cpuid(abcd);
	logtoall("cpuid.1: eax=%08x,ebx=%08x,ecx=%08x,edx=%08x\n",abcd[0],abcd[1],abcd[2],abcd[3]);


//----------------check coreid----------------
	localapic_init();

	int coreid = localapic_coreid();
	logtoall("coreid = %d\n", coreid);

	process_registersupplier(coreid, (void*)CR3BUF);
	int queueid = thread_registerprocessor(coreid, 0);

	initidt_bsp();


//----------------percpu data---------------
	cpubuf[0].gdt = BSPCPU_GDT;
	cpubuf[0].tss = BSPCPU_TSS;
	cpubuf[0].ring0stack = BSPCPU_RSP;
	cpubuf[0].idt = BSPCPU_IDT;

	cpubuf[0].irqcnt = 0;
	cpubuf[0].slpcnt = 0;

	cpubuf[0].coreid = coreid;
	cpubuf[0].procid = 0;
	cpubuf[0].thread = 0;
	cpubuf[0].queueid = queueid;
	cpucnt = 1;

	apictimer_init();

//----------------bspcpu inited------------------
	logtoall("@initcpu_bsp.end\n\n");
	asm("sti");
}




void initcpu_other()
{
	asm("cli");
	logtoall("@initcpu_other.in\n");


//----------------prep descs----------------
	//initpaging((void*)CR3BUF);	//already there from x64.boothelp.asm
	initgdt_ap((void*)APPCPU_GDT);

	if(!enable_fpu())logtoall("fail@enable_fpu\n");
	if(!enable_sse())logtoall("fail@enable_sse\n");
	if(!enable_xsave())logtoall("fail@enable_xsave\n");


//----------------check cpuid----------------
	u32 abcd[4];
	abcd[0] = 0;
	cpuid(abcd);
	logtoall("cpuid.0: eax=%08x,str=%.4s%.4s%.4s\n",abcd[0], &abcd[1],&abcd[3],&abcd[2]);
	abcd[0] = 1;
	cpuid(abcd);
	logtoall("cpuid.1: eax=%08x,ebx=%08x,ecx=%08x,edx=%08x\n",abcd[0],abcd[1],abcd[2],abcd[3]);


//----------------check coreid----------------
	localapic_init();

	int coreid = localapic_coreid();
	logtoall("coreid=%d\n", coreid);

	int queueid = thread_registerprocessor(coreid, 0);

	initidt_ap(coreid);


//----------------percpu data---------------
	int n = cpucnt;
	cpubuf[n].gdt = APPCPU_GDT;
	cpubuf[n].tss = APPCPU_TSS;
	cpubuf[n].ring0stack = APPCPU_RSP;
	cpubuf[n].idt = APPCPU_IDT;

	cpubuf[0].irqcnt = 0;
	cpubuf[0].slpcnt = 0;

	cpubuf[n].coreid = coreid;
	cpubuf[n].procid = 0;
	cpubuf[n].thread = 0;
	cpubuf[n].queueid = queueid;
	cpucnt = n+1;

	apictimer_init();

//----------------appcpu inited------------------
	logtoall("@initcpu_other.end\n\n", coreid);
	asm("sti");
}




static void trampoline_appcpu()
{
	asm("cli");

//----------------set flag,wait cmd--------------
	sendmsg_ap_to_bsp(APMSG_asmtoc);
	while(get_msg_from_bsp() != BSPCMD_finish);

//----------------init cpu---------------
	initcpu_other();

//----------------set flag,wait cmd--------------
	sendmsg_ap_to_bsp(APMSG_finish);

//----------------goto sleep----------------
	logtoall("appcpu: sleep wait for int\n");
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
	logtoall("wakeup cpu@%d...\n", coreid);
	if(0 == localapic_isenabled()){
		logtoall("fail@no apic\n");
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
	logtoall("trampoline16: [%p,%p) -> [%p,%p)\n", trampoline16_start, trampoline16_end, buf, buf+len);
	printmemory(buf, len);

	buf = (u8*)TRAMPOLINE64;
	len = trampoline64_end - trampoline64_start;
	for(j=0;j<len;j++)buf[j] = trampoline64_start[j];
	logtoall("trampoline64: [%p,%p) -> [%p,%p)\n", trampoline64_start, trampoline64_end, buf, buf+len);
	printmemory(buf, len);

	//prep flag
	logtoall("reset flag=0\n");
	volatile u8* flag = (u8*)ApToBsp_message;
	*flag = 0;

	//send INIT IPI
	logtoall("+++sending INIT IPI\n");
	int ret = localapic_sendinit(coreid);
	if(ret < 0)goto fail;
	logtoall("---sent\n\n");

	//wait 10ms
	logtoall("wait 10ms\n\n");
	sleep_us(10*1000);

	//send SIPI IPI
	int isdone = 0;
	logtoall("+++sending START IPI\n");
	for(j=0;j<2;j++){
		logtoall("START IPI %d\n",j);
		//send SIPI IPI
		localapic_sendstart(coreid, TRAMPOLINE16>>12);

		//wait 200us
		sleep_us(1000);

		//check flag
		if(get_msg_from_ap() == APMSG_16bit){
			logtoall("---ap in 16bit mode\n\n");
			isdone = 1;
			break;
		}
		else logtoall("---flag=%x\n", get_msg_from_ap());
	}
	if(0 == isdone){
		goto fail;
	}

givecmdtoap:
	//wait 1000us, check flag
	logtoall("+++sending 16to64\n");
	msg_bsp_to_ap(BSPCMD_16to64);		//must after rip & rsp
	sleep_us(1000);
	if(APMSG_64bit == get_msg_from_ap()){
		logtoall("---ap in 64bit mode\n\n");
	}
	else{
		logtoall("---flag=%x\n", get_msg_from_ap());
		goto fail;
	}

	*(volatile u64*)FromBsp_rip = (u64)trampoline_appcpu;
	*(volatile u64*)FromBsp_rsp = (u64)memoryalloc(0x100000, 0) + 0x100000 - 0x100;
	logtoall("rip=%llx,rsp=%llx\n\n", *(volatile u64*)FromBsp_rip, *(volatile u64*)FromBsp_rsp);

	//wait 1000us, check flag
	logtoall("+++sending asmtoc\n");
	msg_bsp_to_ap(BSPCMD_asmtoc);
	sleep_us(1000);
	if(APMSG_asmtoc == get_msg_from_ap()){
		logtoall("---ap in c code\n\n");
	}
	else{
		logtoall("---flag=%x\n", get_msg_from_ap());
		goto fail;
	}

	//bsp wait 1s, until ap done, then check flag
	logtoall("+++sending finish\n");
	msg_bsp_to_ap(BSPCMD_finish);
	sleep_us(1000*1000);
	if(APMSG_finish == get_msg_from_ap()){
		logtoall("---ap finish\n\n");
		goto allgood;
	}
	else{
		logtoall("---flag=%x\n", get_msg_from_ap());
		goto fail;
	}

fail:
	logtoall("fail@AP error\n\n");
	return;

allgood:
	return;
}




void initcpu_ap()
{
	u64 all = acpi_getknowncores();
	int bsp = localapic_coreid();
	logtoall("knowncores=%llx, bsp=%x\n", all, bsp);

	int j;
	for(j=0;j<64;j++){
		if(0 == (all & (1<<j)))continue;
		if(j == bsp)continue;

		initcpu_onecore(j);
		break;
	}
}