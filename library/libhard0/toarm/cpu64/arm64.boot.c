#include "libhard.h"
extern void trampoline64();
//
void initpaging(void*);
void initexception(void*);
//
int process_registersupplier(int coreid, void* cr3);
int thread_registerprocessor(int core, int proc);




struct percpu{
	void* pagetable;
	void* exception;

	int coreid;
	int pid;
	int qid;
	int tid;
};
static struct percpu cpubuf[8];
static int cpucnt = 0;




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
	return 0;
}
int percpu_thread()
{
	return 0;
}




int percpu_makekern()
{
	return 0;
}
int percpu_makeuser()
{
	return 0;
}
int percpu_makearg()
{
	return 0;
}




int percpu_savecpu()
{
	return 0;
}
int percpu_loadcpu()
{
	return 0;
}
int percpu_savefpu()
{
	return 0;
}
int percpu_loadfpu()
{
	return 0;
}
int percpu_schedule()
{
	return 0;
}




void initcpu_bsp()
{
	int coreid = percpu_coreid();
	say("@initcpu_bsp=%x\n", coreid);

	void* tmp = memorycreate(0x100000, 0);
	initexception(tmp+0x100000);
	initpaging(tmp);

	int pid = process_registersupplier(coreid, 0);

	int qid = thread_registerprocessor(coreid, 0);
	int tid = 0;

	cpubuf[0].pagetable = tmp;
	cpubuf[0].exception = tmp+0x100000;
	cpubuf[0].coreid = coreid;
	cpubuf[0].pid = 0;
	cpubuf[0].qid = qid;
	cpubuf[0].tid = tid;
	cpucnt = 1;

	//percpu_inittimer();

	say("@initcpu_bsp.end\n\n");
}
void initcpu_other()
{
	int coreid = percpu_coreid();
	say("@initcpu_other=%x\n",coreid);

	void* tmp = memorycreate(0x100000, 0);
	//initpaging(tmp);
	//initexception(tmp+0x100000);

	int qid = thread_registerprocessor(coreid, 0);
	int tid = 0;

	int n = cpucnt;
	cpubuf[n].pagetable = tmp;
	cpubuf[n].exception = tmp+0x100000;
	cpubuf[n].coreid = coreid;
	cpubuf[n].pid = 0;
	cpubuf[n].qid = qid;
	cpubuf[n].tid = tid;
	cpucnt = n+1;

	//percpu_inittimer();

	say("@initcpu_other.end\n\n");
	for(;;);
}




void initcpu_ap()
{
	//say("@initcpu_ap\n");

	*(u64*)0xe0 = (u64)trampoline64;
	asm volatile ("sev");
	//todo: wait for this core finish init

	//say("@initcpu_ap.end\n\n");
}
