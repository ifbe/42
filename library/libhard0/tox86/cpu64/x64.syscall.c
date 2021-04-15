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
	switch(cpureg->rdx){
	case _yield_:syscall_yield(cpureg);return;
	case _exit_:syscall_exit(cpureg);return;
	case _ver_:syscall_version();goto byebye;
	case _slp_:syscall_sleep();goto byebye;
	//default:say("unknown@syscall: %llx\n", cpureg->rdx);
	}

	//let system do rest
	u64 arg[8];
	arg[0] = cpureg->rax;
	arg[1] = cpureg->rbx;
	arg[2] = cpureg->rsi;
	arg[3] = cpureg->rdi;
	arg[4] = cpureg->r8;
	arg[5] = cpureg->r9;
	arg[6] = cpureg->r14;
	arg[7] = cpureg->r15;
	system_handler(cpureg->rdx, arg);

byebye:
	cpureg->rax = arg[0];
	cpureg->rbx = arg[1];
	cpureg->rsi = arg[2];
	cpureg->rdi = arg[3];
	cpureg->r8  = arg[4];
	cpureg->r9  = arg[5];
	cpureg->r14 = arg[6];
	cpureg->r15 = arg[7];
}
void syscall_caller(u64 req, u64* arg)
{
	//req: rdx
	//arg: rax, rbx, rsi, rdi, r8, r9, r14, r15
	asm(
		"movq %0, %%rax\n"		//arg0
		"movq %1, %%rbx\n"		//arg1
		"movq %2, %%rsi\n"		//arg2
		"movq %3, %%rdi\n"		//arg3
		"movq %4, %%r8\n"		//arg4
		"movq %5, %%r9\n"		//arg5
		"movq %6, %%r14\n"		//arg6
		"movq %7, %%r15\n"		//arg7
		"movq %8, %%rdx\n"		//this is call id
		"int $0x80\n"
		:
		: "r"(arg[0]),"r"(arg[1]),"r"(arg[2]),"r"(arg[3]),
		  "r"(arg[4]),"r"(arg[5]),"r"(arg[6]),"r"(arg[7]),"r"(req)
	);
}




/*
isr80_ring3to0:
	push rax

	mov       rax, 0x10		;kernel cs @ 10
	mov [rsp+0x10], rax
	mov       rax, 0x18		;kernel ss @ 18
	mov [rsp+0x28], rax

	pop rax
	iretq


global enterring3
enterring3:
	pop             rax		;user ip
	mov [rsp-0x28], rax
	mov       rax, 0x23		;user cs
	mov [rsp-0x20], rax
	mov      rax, 0x202		;rflag
	mov [rsp-0x18], rax
	mov        rax, rsp		;user sp
	mov [rsp-0x10], rax
	mov       rax, 0x2b		;user ss
	mov [rsp-0x08], rax

	sub rsp, 0x28
	iretq
*/


