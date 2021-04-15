#include "libhard.h"
extern void trampoline64();
int raspi_version();
void CleanDataCache (void);
void wait_msec(int);
//
void gic4_init();
void percputimer_init();
//
void initpaging(void*);
void initexception(void*);
//
int process_registersupplier(int coreid, void* cr3);
void process_switchto(int curr, int next);
//
int thread_registerprocessor(int core, int proc);
int thread_switchto(int queueid,int curr, int queueid2,int next, int coreid,void* cpureg);
int thread_findnext(int queueid,int curr);
int thread_findproc(int queueid,int curr);




struct savereg{
	u64 x0;
	u64 x1;
	u64 x2;
	u64 x3;
	u64 x4;
	u64 x5;
	u64 x6;
	u64 x7;

	u64 x8;
	u64 x9;
	u64 x10;
	u64 x11;
	u64 x12;
	u64 x13;
	u64 x14;
	u64 x15;

	u64 x16;
	u64 x17;
	u64 x18;
	u64 x19;
	u64 x20;
	u64 x21;
	u64 x22;
	u64 x23;

	u64 x24;
	u64 x25;
	u64 x26;
	u64 x27;
	u64 x28;
	u64 x29;
	u64 x30;		//lr
	u64 sp;

	u64 elr;
	u64 spsr;
};
struct percpu{
	void* pagetable;
	void* isrstack;

	//count
	u32 irqcnt;
	u32 slpcnt;

	//cpucore
	int coreid;

	//process
	int pid;

	//thread
	int qid;
	int tid;
};
static struct percpu cpubuf[8];
static int cpucnt = 0;




void percpu_enableint(void)
{
    //开启IRQ
    asm volatile("msr daifclr, #2");
}
void percpu_disableint(void)
{
    //关闭IRQ
    asm volatile("msr daifset, #2");
}
int percpu_coreid()
{
	u64 id;
	asm("mrs %0, mpidr_el1" : "=r"(id) );

	//todo: why it is 0x80000000|id insted of id?
	id &= 0xf;
	return id;
}
int percpu_process()
{
	int coreid = percpu_coreid();
	struct percpu* per = &cpubuf[coreid];
	return per->pid;
}
int percpu_tqueue()
{
	int coreid = percpu_coreid();
	struct percpu* per = &cpubuf[coreid];
	return per->qid;
}
int percpu_thread()
{
	int coreid = percpu_coreid();
	struct percpu* per = &cpubuf[coreid];
	return per->tid;
}




int percpu_makekern(struct savereg* reg, u64 ip, u64 sp)
{
	struct savereg* run = (void*)sp;
	run->x30 = 0;
	run->sp = sp;
	run->elr = ip;
	asm("mrs x0, spsr_el1; str x0, %0" : "=m"(run->spsr) : : "x0");

	reg->sp = sp;
	return 0;
}
int percpu_makeuser(struct savereg* reg, u64 ip, u64 sp)
{
	struct savereg* run = (void*)sp;	//virt2phys(sp);
	run->x30 = 0;
	run->sp = sp;
	run->elr = ip;
	asm("mrs x0, spsr_el1; str x0, %0" : "=m"(run->spsr) : : "x0");

	reg->sp = sp;
	return 0;
}
int percpu_makearg(struct savereg* reg, u64 arg)
{
	reg->x0 = (u64)arg;
}




//cpu register
void percpu_savecpu(struct savereg* save, struct savereg* curr)
{
	save->sp = curr->sp;
}
void percpu_loadcpu(struct savereg* save, struct savereg* curr)
{
	curr->sp = save->sp;
}



/*
//fpu register
void percpu_savefpu(u64 addr)
{
}
void percpu_savefpu(u64 addr)
{
}*/




int percpu_schedule(struct savereg* regs)
{
	int coreid = percpu_coreid();

	struct percpu* per = &cpubuf[coreid];
	//say("coreid=%x, percpu@%p\n", coreid, per);
	//say("x0=%llx,x1=%llx,x2=%llx,x3=%llx, x29=%llx,x30=%llx\n",regs->x0,regs->x1,regs->x2,regs->x3, regs->x29,regs->x30);

	//check thread
	int tcurr;
	int tnext;
	tcurr = per->tid;

	per->irqcnt += 1;
	if(0 == (per->irqcnt&0xf)){
		tnext = 0;
	}
	else{
		tnext = thread_findnext(per->qid, tcurr);
	}
	//*(int*)(coreid*16 + 0) = tcurr;
	//*(int*)(coreid*16 + 4) = tnext;
	if(tcurr == tnext)return 0;
/*
	//check process
	int pcurr = per->pid;
	int pnext = thread_findproc(per->qid,tnext);

	//switch process
	if(pcurr != pnext){
		process_switchto(pcurr, pnext);
		per->pid = pnext;
	}
*/
	return 0;
	//switch thread
	thread_switchto(per->qid,tcurr, per->qid,tnext, coreid,regs);
	per->tid = tnext;

	return 0;
}




void initcpu_bsp()
{
	percpu_disableint();


//----------------haha----------------
	int coreid = percpu_coreid();
	say("@initcpu_bsp=%x\n", coreid);

	void* tmp = memorycreate(0x100000, 0);
	initexception(tmp+0x100000);
	initpaging(tmp);

	int pid = process_registersupplier(coreid, 0);

	int qid = thread_registerprocessor(coreid, 0);
	int tid = 0;

//----------------percpu data---------------
	cpubuf[0].pagetable = tmp;
	cpubuf[0].isrstack = tmp+0x100000;

	cpubuf[0].irqcnt = 0;
	cpubuf[0].slpcnt = 0;

	cpubuf[0].coreid = coreid;
	cpubuf[0].pid = 0;
	cpubuf[0].qid = qid;
	cpubuf[0].tid = tid;
	cpucnt = 1;


//----------------haha----------------
	if(4 == raspi_version()){
		gic4_init();
		percputimer_init();
		percpu_enableint();
	}

	say("@initcpu_bsp.end\n\n");
}
void initcpu_other()
{
	percpu_disableint();


//----------------haha----------------
	int coreid = percpu_coreid();
	say("@initcpu_other=%x\n",coreid);

	void* tmp = memorycreate(0x100000, 0);
	//initpaging(tmp);
	//initexception(tmp+0x100000);

	int qid = thread_registerprocessor(coreid, 0);
	int tid = 0;


//----------------percpu data---------------
	int n = cpucnt;
	cpubuf[n].pagetable = tmp;
	cpubuf[n].isrstack = tmp+0x100000;

	cpubuf[0].irqcnt = 0;
	cpubuf[0].slpcnt = 0;

	cpubuf[0].coreid = coreid;
	cpubuf[0].pid = 0;
	cpubuf[0].qid = qid;
	cpubuf[0].tid = tid;
	cpucnt = n+1;


//----------------haha----------------
	//percputimer_init();
	//percpu_enableint();

	say("@initcpu_other.end\n\n");

	volatile u64* ptr = (void*)0xd8;
	ptr[1] = 0;

	for(;;)asm("wfe");
}




void initcpu_ap()
{
	//say("@initcpu_ap\n");
	volatile u64* ptr = (void*)0xd8;

	//check
	if(ptr[1]){
		say("error@multicore: [%p] = %llx\n", &ptr[1], ptr[1]);
		return;
	}

	//write memory
	ptr[1] = (u64)trampoline64;

	//ensure above write memory
	CleanDataCache();

	//send event
	asm volatile ("sev");

	//todo: wait for this core finish init
	int timeout = 1000*1000;
	while(timeout--){
		if(0 == ptr[1])break;
		wait_msec(1);
	}

	//say("@initcpu_ap.end\n\n");
}
