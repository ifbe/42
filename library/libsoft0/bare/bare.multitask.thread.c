#include "libsoft.h"
int percpucoreid();
void haltwaitforint();
//
void fpu_fxsave(u64 addr);
void fpu_fxrstor(u64 addr);
void fpu_xsave(u64 addr);
void fpu_rstor(u64 addr);




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
	volatile u32           count;
	volatile u32           state;	//0 = disable, n = normal
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
static volatile struct taskstate* percputasktable[8];
static volatile int percputaskcount[8];



/*
static void test1()
{
	u64 time;
	while(1){
		time = timeread();
		say("core=%d, task=%d, time=%llx\n", percpucoreid(), 1, time);
		haltwaitforint();
	}
}
static void test2()
{
	u64 time;
	while(1){
		time = timeread();
		say("core=%d, task=%d, time=%llx\n", percpucoreid(), 2, time);
		haltwaitforint();
	}
}*/




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

	u64 fpubuf = (u64)&task->fpureg;
	fpu_fxsave((fpubuf+0x40) - (fpubuf&0x3f));

	task->count = 0;
	task->state = 1;
	task->BindToCoreId = -1;	//percpucoreid();
	task->CoreRunThisTask = -1;

	percputaskcount[that] += 1;		//must after all above
	return 0;
}
int threaddelete(u64 id)
{
	return 0;
}
int tasksearch(void* buf, int len)
{
	int j,k;
	volatile struct taskstate* tasktable;
	say("@tasksearch\n");
	say("%p,%p,%d\n", percputasktable, percputaskcount,cpucount);

	for(j=0;j<cpucount;j++){
		say("cpu[%d]:\n", j);
		tasktable = percputasktable[j];
		for(k=0;k<percputaskcount[j];k++){
			say("%d: ip=%llx,sp=%llx\n", k, tasktable[k].cpureg.ip, tasktable[k].cpureg.sp);
		}
	}
	return 0;
}
int taskmodify(void* buf, int len)
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
int schedulethread_findcurr(volatile struct taskstate* tasktable, int taskcount, int coreid)
{
	int j=0;
	for(j=0;j<taskcount;j++){
		if(coreid == tasktable[j].CoreRunThisTask){
			return j;
		}
	}
	return -1;
}
int schedulethread_findnext(volatile struct taskstate* tasktable, int taskcount, int icurr)
{
	//0: next = 0
	if(0 == (tasktable[0].count&0xff))return 0;

	//[1,255]: next = all except the 0
	int j,inext=0;
	for(j=icurr+1;j<taskcount;j++){
		if(-1 == tasktable[j].CoreRunThisTask){
		if(1 == tasktable[j].state){
			return j;
		}	//this task is alive
		}	//no one running this
	}
	for(j=1;j<icurr;j++){
		if(-1 == tasktable[j].CoreRunThisTask){
		if(1 == tasktable[j].state){
			return j;
		}	//this task is alive
		}	//no one running this
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
	int coreid = percpucoreid();

	int itable = schedulethread_findtable(coreid);
	if(itable < 0)return;

	volatile struct taskstate* tasktable = percputasktable[itable];
	int taskcount = percputaskcount[itable];

	tasktable[0].count += 1;

	int icurr = schedulethread_findcurr(tasktable, taskcount, coreid);
	if(icurr < 0)goto fuckshit;

	int inext = schedulethread_findnext(tasktable, taskcount, icurr);
	if(inext < 0)goto fuckshit;

	if(icurr == inext)goto fuckshit;

	schedulethread_savecurr(&tasktable[icurr], (u8*)cpureg, coreid);

	schedulethread_loadnext(&tasktable[inext], (u8*)cpureg, coreid);

	tasktable[icurr].CoreRunThisTask = -1;			//no cpu @ this task
	tasktable[inext].CoreRunThisTask = coreid;		//this cpu @ this task

fuckshit:
	return;
}
void threadmanager_registersupplier(int coreid)
{
	say("incoming thread: from coreid=%d\n", coreid);

	volatile struct taskstate* tasktable = cpubuffer + 0x10000*cpucount;

	volatile struct taskstate* task = &tasktable[0];

	task->cpureg.ip = 0x5a5a5a5a;
	task->cpureg.cs = 0x10;
	task->cpureg.flag = 0x202;
	task->cpureg.sp = 0xfedcba98;
	task->cpureg.ss = 0x18;

	task->count = 0;
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