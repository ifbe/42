#include "libhard.h"
#define _ver_ hex32('v','e','r',0)
#define _slp_ hex32('s','l','p',0)
#define _yield_ hex32('y','i','e','l')
#define _exit_ hex32('e','x','i','t')
int percpu_coreid();
int percpu_process();
int percpu_tqueue();
int percpu_thread();
int percpu_schedule(void* cpureg);
//
int thread_disable(int qid, int tid);
int system_handler(u64 req, u64* arg);




struct saved_cpureg{
    u64 x0;
    u64 x1;
    u64 x2;
    u64 x3;
    u64 x4;
    u64 x5;
    u64 x6;
    u64 x7;
    u64 x8;     //call id
    u64 x9;
};




void syscall_version()
{
	say("ver: date=%s,time=%s\n", __DATE__, __TIME__);
}
void syscall_sleep()
{
}


void syscall_exit(void* cpureg)
{
	int core = percpu_coreid();
	int pid = percpu_process();
	int qid = percpu_tqueue();
	int tid = percpu_thread();
	say("@syscall_exit: coreid=%d,pid=%d, qid=%d,tid=%d\n", core,pid, qid,tid);
	thread_disable(qid, tid);
	percpu_schedule(cpureg);
}
void syscall_yield(void* cpureg)
{
	int core = percpu_coreid();
	int pid = percpu_process();
	int qid = percpu_tqueue();
	int tid = percpu_thread();
	say("@syscall_yield: coreid=%d,pid=%d, qid=%d,tid=%d\n", core,pid, qid,tid);
	percpu_schedule(cpureg);
}




void syscall_handler(struct saved_cpureg* cpureg)
{
	//do what i can
	switch(cpureg->x8){
	case _ver_:syscall_version();return;
	case _slp_:syscall_sleep();return;
	case _yield_:syscall_yield(cpureg);return;
	case _exit_:syscall_exit(cpureg);return;
	//default:say("unknown@syscall: %llx\n", cpureg->x8);
	}

	//let system do rest
	system_handler(cpureg->x8, (u64*)cpureg);
}
void syscall_caller(u64 req, u64* arg)
{
	//req: x8
	//arg: x0, x1, x2, x3, x4, x5, x6, x7
	asm(
		"mov x0, %0\n"		//arg0
		"mov x1, %1\n"		//arg1
		"mov x2, %2\n"		//arg2
		"mov x3, %3\n"		//arg3
		"mov x4, %4\n"		//arg4
		"mov x5, %5\n"		//arg5
		"mov x6, %6\n"		//arg6
		"mov x7, %7\n"		//arg7
		"mov x8, %8\n"		//this is call id
		"svc #0\n"
		:
		: "r"(arg[0]),"r"(arg[1]),"r"(arg[2]),"r"(arg[3]),
		  "r"(arg[4]),"r"(arg[5]),"r"(arg[6]),"r"(arg[7]),"r"(req)
		: "x0","x1","x2","x3","x4","x5","x6","x7","x8"
	);
}
