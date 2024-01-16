#include "libsoft.h"
int percpu_coreid();
int percpu_savefpu(void* buf);
int percpu_loadfpu(void* buf);
//
int percpu_savecpu(void* buf, void* saved);
int percpu_loadcpu(void* buf, void* saved);
int percpu_makekern(void* buf, void* arg, void* ip, void* ss);
int percpu_makeuser(void* buf, void* arg, u64 ip, u64 sp, u64 ip_pa, u64 sp_pa);




struct saved_status{
	u32            type;
	u32           state;	//0 = disable, n = normal
	int    BindToCoreId;	//-1 = any, 0 to n = processor id
	int CoreRunThisTask;	//-1 = none, 0 to n = processor id
	int procid;
	int vmcbid;
}__attribute__((packed));

struct threadstate{
	//xsaveopt: use=1024byte, align=64byte
	u8 fpu[1024];

	u8 cpu[512];

	u8 sys[256];

	struct saved_status info;

	u8 padding[2048 - 1024 - 512 - 256 - sizeof(struct saved_status)];
}__attribute__((packed));




static void* cpubuffer = 0;
static int cpucount = 0;
//
static struct threadstate* percputasktable[8];
static int percputaskcount[8];




u64 thread_forthisprocess(int core, int proc, u64 pa, u64 va)
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
	struct threadstate* tasktable = percputasktable[that];
	struct threadstate* task = &tasktable[taskcount];

	u64 arg[2];
	u64 ip_pa = pa;
	u64 sp_pa = pa + 0x1ffe00;	//va + 2m - 512b
	u64 ip_va = va;
	u64 sp_va = va + 0x1ffe00;	//va + 2m - 512b
	percpu_makeuser(&task->cpu, arg, ip_va, sp_va, ip_pa, sp_pa);
	percpu_savefpu(&task->fpu);

	task->info.BindToCoreId = -1;	//percpu_coreid();
	task->info.CoreRunThisTask = -1;
	task->info.procid = proc;
	task->info.vmcbid = 0;
	task->info.type = 3;
	task->info.state = 1;

	percputaskcount[that] += 1;		//must after all above
	return 0;
}
int thread_registerprocessor(int coreid, int procid)
{
	logtoall("incoming thread: from coreid=%d\n", coreid);

	int queueid = cpucount;

	struct threadstate* tasktable = cpubuffer + 0x10000*queueid;

	struct threadstate* task = &tasktable[0];

	void* ip = (void*)0x5a5a5a5a;
	void* sp = (void*)0xfedcba98;
	percpu_savefpu(&task->fpu);

	task->info.BindToCoreId = coreid;
	task->info.CoreRunThisTask = coreid;
	task->info.procid = procid;
	task->info.vmcbid = 0;
	task->info.type = 0;
	task->info.state = 1;

	percputasktable[queueid] = tasktable;
	percputaskcount[queueid] = 1;

	cpucount += 1;
	return queueid;
}




u64 threadcreate(void* ip, void* arg)
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
	struct threadstate* tasktable = percputasktable[that];
	struct threadstate* task = &tasktable[taskcount];

	void* sp = memoryalloc(0x100000, 0) + 0xffe00;
	percpu_makekern(&task->cpu, &arg, ip, sp);
	percpu_savefpu(&task->fpu);

	task->info.BindToCoreId = -1;	//percpu_coreid();
	task->info.CoreRunThisTask = -1;
	task->info.procid = 0;
	task->info.vmcbid = 0;
	task->info.type = 0;
	task->info.state = 1;

	percputaskcount[that] += 1;		//must after all above
	return 0;
}
int threaddelete(u64 id)
{
	return 0;
}
int threadsearch(void* buf, int len)
{
	int j,k;
	struct threadstate* tasktable;
	struct threadstate* task;
	logtoall("@threadsearch\n");
	logtoall("%p,%p,%d\n", percputasktable, percputaskcount, cpucount);

	for(j=0;j<cpucount;j++){
		logtoall("cpu[%d]:\n", j);
		tasktable = percputasktable[j];
		for(k=0;k<percputaskcount[j];k++){
			task = &tasktable[k];
/*			logtoall("%d@%p: want=%d,core=%d, proc=%d,vmcb=%d, cs=%x,ip=%llx, ss=%x,sp=%llx\n",
				k, task,
				task->info.BindToCoreId, task->info.CoreRunThisTask,
				task->info.procid, task->info.vmcbid,
				task->cpu.cs, task->cpu.ip,
				task->cpu.ss, task->cpu.sp
			);*/
			logtoall("%d@%p: type=%d,stat=%d, want=%d,core=%d, proc=%d,vmcb=%d\n",
				k, task,
				task->info.type, task->info.state,
				task->info.BindToCoreId, task->info.CoreRunThisTask,
				task->info.procid, task->info.vmcbid
			);
		}
	}
	return 0;
}
int threadmodify(void* buf, int len)
{
	return 0;
}




int thread_findtableforcore(int coreid)
{
	int j;
	for(j=0;j<cpucount;j++){
		if(coreid == percputasktable[j]->info.BindToCoreId)return j;
	}
	return -1;
}
int thread_findcurrintable(struct threadstate* tasktable, int taskcount, int coreid)
{
	int j=0;
	for(j=0;j<taskcount;j++){
		if(coreid == tasktable[j].info.CoreRunThisTask){
			return j;
		}
	}
	return -1;
}
int thread_findnextintable(struct threadstate* tasktable, int taskcount, int icurr)
{
	//[1,255]: next = all except the 0
	int j,inext=0;
	for(j=icurr+1;j<taskcount;j++){
		if(-1 == tasktable[j].info.CoreRunThisTask){
		if(1 == tasktable[j].info.state){
			return j;
		}	//this task is alive
		}	//no one running this
	}
	for(j=1;j<icurr;j++){
		if(-1 == tasktable[j].info.CoreRunThisTask){
		if(1 == tasktable[j].info.state){
			return j;
		}	//this task is alive
		}	//no one running this
	}
	return icurr;
}
void thread_switchbuffer(struct threadstate* curr, struct threadstate* next, void* cpureg, int coreid)
{
	percpu_savecpu(curr->cpu, cpureg);
	percpu_savefpu(curr->fpu);

	percpu_loadcpu(next->cpu, cpureg);
	percpu_loadfpu(curr->fpu);

	curr->info.CoreRunThisTask = -1;			//no cpu @ this task
	next->info.CoreRunThisTask = coreid;		//this cpu @ this task
}/*
void thread_schedule(void* cpureg)
{
	//if(first 1008/1024 of one sec)dont change
	//u64 time = timeread_us() >> 10;
	//if((time&0x3ff) < 0x3f0)return;

	//0.which core want change, and his own table
	int coreid = percpu_coreid();

	int itable = thread_findtableforcore(coreid);
	if(itable < 0)return;

	struct threadstate* tasktable = percputasktable[itable];
	int taskcount = percputaskcount[itable];

	int icurr = thread_findcurrintable(tasktable, taskcount, coreid);
	if(icurr < 0)goto fuckshit;

	int inext = thread_findnextintable(tasktable, taskcount, icurr);
	if(inext < 0)goto fuckshit;

	if(icurr == inext)goto fuckshit;
	thread_switchbuffer(&tasktable[icurr], &tasktable[inext], cpureg, coreid);

fuckshit:
	return;
}*/




int thread_disable(int queueid, int curr)
{
	struct threadstate* tasktable = percputasktable[queueid];
	tasktable[curr].info.state = 0;
	return 0;
}
int thread_findnext(int queueid, int curr)
{
	struct threadstate* tasktable = percputasktable[queueid];
	int taskcount = percputaskcount[queueid];

	return thread_findnextintable(tasktable, taskcount, curr);
}
int thread_findproc(int queueid, int curr)
{
	struct threadstate* tasktable = percputasktable[queueid];
	return tasktable[curr].info.procid;
}
void thread_switchto(int queueid, int curr, int queueid2, int next, int coreid, void* cpureg)
{
	struct threadstate* tasktable = percputasktable[queueid];
	int taskcount = percputaskcount[queueid];

	thread_switchbuffer(&tasktable[curr], &tasktable[next], cpureg, coreid);
}




void initthread()
{
	logtoall("@initthread\n");

	int j;
	u8* tmp = memoryalloc(0x100000, 0);
	for(j=0;j<0x100000;j++)tmp[j] = 0;

	cpubuffer = (void*)tmp;
	cpucount = 0;
}
void freethread()
{
}
