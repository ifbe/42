#include "libsoft.h"
void haltwaitforint();
int localapic_coreid();




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

struct saved_fpureg{
	u8 xsavearea[1024+0x40];
}__attribute__((packed));

struct saved_pagetable{
}__attribute__((packed));

struct saved_vmcb{
}__attribute__((packed));

struct taskstate{
	volatile int            lock;	//0 = free, n = locked
	volatile int           state;	//0 = disable, n = normal
	volatile int    BindToCoreId;	//-1 = any, 0 to n = processor id
	volatile int CoreRunThisTask;	//-1 = none, 0 to n = processor id
	volatile struct saved_cpureg cpureg;
	volatile struct saved_fpureg fpureg;
	volatile struct saved_pagetable* pt;
	volatile struct saved_vmcb* vmcb;
}__attribute__((packed));




static volatile void* cpubuffer = 0;
static volatile int cpucount = 0;
//
static volatile struct taskstate** percputasktable = 0;
static volatile int percputaskcount[8];




void fpu_fxsave(u64 addr)
{
	asm("movq %0, %%rax\n"
		"fxsave (%%rax)\n"
		:
		:"m"(addr)
	);
}
void fpu_fxrstor(u64 addr)
{
	asm("movq %0, %%rax\n"
		"fxrstor (%%rax)\n"
		:
		:"m"(addr)
	);
}
void fpu_xsave(u64 addr)
{
	asm("movq %0, %%rcx;\n"
		"movl $0xffffffff, %%edx;\n"
		"movl $0xffffffff, %%eax;\n"
		"xsaveopt (%%rcx);\n"
		:
		:"m"(addr)
	);
}
void fpu_rstor(u64 addr)
{
	asm("movq %0, %%rcx;\n"
		"movl $0xffffffff, %%edx;\n"
		"movl $0xffffffff, %%eax;\n"
		"xrstor (%%rcx);\n"
		:
		:"m"(addr)
	);
}




static void test1()
{
	u64 time;
	while(1){
		time = timeread();
		say("core=%d, task=%d, time=%llx\n", localapic_coreid(), 1, time);
		haltwaitforint();
	}
}
static void test2()
{
	u64 time;
	while(1){
		time = timeread();
		say("core=%d, task=%d, time=%llx\n", localapic_coreid(), 2, time);
		haltwaitforint();
	}
}




u64 threadcreate(void* code, void* arg)
{
	int j;
	int that = -2;
	int least = 9999;
	for(j=0;j<cpucount;j++){
		if(least > percputaskcount[j]){
			that = j;
			least = percputaskcount[j];
		}
	}
	if(that < 0)return 0;

	int taskcount = percputaskcount[that];
	volatile struct taskstate* tasktable = percputasktable[that];
	volatile struct taskstate* task = &tasktable[taskcount];

	task->cpureg.ip = (u64)code;
	task->cpureg.cs = 0x10;
	task->cpureg.flag = 0x202;
	task->cpureg.sp = (u64)memorycreate(0x100000, 0) + 0xffff0;
	task->cpureg.ss = 0x18;

	task->cpureg.rdi = (u64)arg;	//di,si,dx,cx,r8,r9
	task->cpureg.rcx = (u64)arg;	//cx,dx,r8,r9

	task->lock = 0;
	task->state = 1;
	task->BindToCoreId = -1;	//localapic_coreid();
	task->CoreRunThisTask = -1;

	percputaskcount[that] += 1;		//must after all above
	return 0;
}
int threaddelete(u64 id)
{
	return 0;
}




int schedulethread_findtable(int coreid)
{
	int j;
	for(j=0;j<cpucount;j++){
		if(coreid == percputasktable[j]->BindToCoreId)return j;
	}
	return -1;
}
void schedulethread_savecurr(volatile struct taskstate* pcurr, u8* cputmp, int coreid)
{
	int j;
	u8* cpubuf = (void*)&pcurr->cpureg;
	for(j=0;j<sizeof(struct saved_cpureg);j++)cpubuf[j] = cputmp[j];

	u64 fpubuf = (u64)&pcurr->fpureg;
	fpu_fxsave((fpubuf+0x40) - (fpubuf&0x3f));
}
void schedulethread_loadnext(volatile struct taskstate* pnext, u8* cputmp, int coreid)
{
	int j;

	u8* cpubuf = (void*)&pnext->cpureg;
	for(j=0;j<sizeof(struct saved_cpureg);j++)cputmp[j] = cpubuf[j];

	u64 fpubuf = (u64)&pnext->fpureg;
	fpu_fxrstor((fpubuf+0x40) - (fpubuf&0x3f));
}
void schedulethread(struct saved_cpureg* cpureg)
{
	//if(first 1008/1024 of one sec)dont change
	//u64 time = timeread() >> 10;
	//if((time&0x3ff) < 0x3f0)return;

	//0.which core want change, and his own table
	int coreid = localapic_coreid();
	int itable = schedulethread_findtable(coreid);
	if(itable < 0)return;

	volatile struct taskstate* tasktable = percputasktable[itable];
	int taskcount = percputaskcount[itable];

	//1.find curr: CoreRunThisTask == coreid
	int j,icurr=0;
	volatile struct taskstate* pcurr = 0;
	for(j=0;j<taskcount;j++){
		if(coreid == tasktable[j].CoreRunThisTask){
			icurr = j;
			pcurr = &tasktable[j];
			break;
		}
	}
	if(0 == pcurr)goto fuckshit;	//change nothing

	//2.find next: 
	int k,inext=0;
	volatile struct taskstate* pnext = 0;
	for(j=icurr+1;j<icurr+taskcount;j++){
		k = j % taskcount;
		if(k != icurr){
		if(-1 == tasktable[k].CoreRunThisTask){
		if(1 == tasktable[k].state){
//		if((-1 == tasktable[k].BindToCoreId) | (coreid == tasktable[k].BindToCoreId)){
//		if(0 == __sync_lock_test_and_set(&tasktable[k].lock,1)){
			inext = k;
			pnext = &tasktable[k];
			break;
//		}	//try to own lock
//		}	//bindto noone | bindto myself
		}	//this task is alive
		}	//no one running this
		}	//not same as curr
	}
	if(0 == pnext)goto fuckshit;
//say("core=%d,curr=%d,next=%d\n",coreid, icurr,inext);

	schedulethread_savecurr(pcurr, (u8*)cpureg, coreid);

	schedulethread_loadnext(pnext, (u8*)cpureg, coreid);

	pcurr->CoreRunThisTask = -1;			//no cpu @ this task
	pnext->CoreRunThisTask = coreid;		//this cpu @ this task

	//__sync_lock_release(&pnext->lock);

fuckshit:
	return;
}
void incomingthread(int coreid)
{
	say("incoming thread: from coreid=%d\n", coreid);

	volatile struct taskstate* tasktable = cpubuffer + 0x10000*cpucount;

	volatile struct taskstate* task = &tasktable[0];

	task->cpureg.ip = 0x5a5a5a5a;
	task->cpureg.cs = 0x10;
	task->cpureg.flag = 0x202;
	task->cpureg.sp = 0xfedcba98;
	task->cpureg.ss = 0x18;

	task->lock = 0;
	task->state = 1;
	task->BindToCoreId = coreid;
	task->CoreRunThisTask = coreid;

	percputasktable[cpucount] = tasktable;
	percputaskcount[cpucount] = 1;

	cpucount += 1;
}




void initthread()
{
	say("@initthread\n");

	int j;
	u8* tmp = memorycreate(0x100000, 0);
	for(j=0;j<0x100000;j++)tmp[j] = 0;

	cpubuffer = (void*)tmp;
	cpucount = 0;
}
void freethread()
{
}