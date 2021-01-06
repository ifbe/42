#include "libsoft.h"
void haltwaitforint();
int localapic_coreid();




struct saved_register{
	u8 xmm0[16];
	u8 xmm1[16];
	u8 xmm2[16];
	u8 xmm3[16];
	u8 xmm4[16];
	u8 xmm5[16];
	u8 xmm6[16];
	u8 xmm7[16];
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

struct saved_pagetable{
}__attribute__((packed));

struct saved_vmcb{
}__attribute__((packed));

struct taskstate{
	int            lock;	//0 = free, n = locked
	int           state;	//0 = disable, n = normal
	int    BindToCoreId;	//-1 = any, 0 to n = processor id
	int CoreRunThisTask;	//-1 = none, 0 to n = processor id
	struct saved_register reg;
	struct saved_pagetable* pt;
	struct saved_vmcb* vmcb;
}__attribute__((packed));




static struct taskstate tasktable[8];
static int taskcount = 0;




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
	struct taskstate* task = &tasktable[taskcount];
	taskcount += 1;

	task->lock = 0;
	task->state = 1;
	task->BindToCoreId = -1;
	task->CoreRunThisTask = -1;

	task->reg.ip = (u64)code;
	task->reg.cs = 0x10;
	task->reg.flag = 0x202;
	task->reg.sp = (u64)memorycreate(0x100000, 0) + 0xffff0;
	task->reg.ss = 0x18;

	task->reg.rdi = (u64)arg;	//di,si,dx,cx,r8,r9
	task->reg.rcx = (u64)arg;	//cx,dx,r8,r9
	return 0;
}
int threaddelete(u64 id)
{
	return 0;
}




void incomingthread(int coreid)
{
	say("incoming thread: from coreid=%d\n", coreid);

	struct taskstate* task = &tasktable[taskcount];
	taskcount += 1;

	task->lock = 0;
	task->state = 1;
	task->BindToCoreId = coreid;
	task->CoreRunThisTask = coreid;
}
void schedulethread(struct saved_register* p)
{
	//if(first 1008/1024 of one sec)dont change
	//u64 time = timeread() >> 10;
	//if((time&0x3ff) < 0x3f0)return;

	//0.which core want change
	int coreid = localapic_coreid();

	//1.find curr: CoreRunThisTask == coreid
	int j;
	struct taskstate* curr = 0;
	for(j=0;j<taskcount;j++){
		if(coreid == tasktable[j].CoreRunThisTask){
			curr = &tasktable[j];
			break;
		}
	}
	if(0 == curr)return;	//change nothing

	//2.find next: 
	struct taskstate* next = 0;
	for(j=0;j<taskcount;j++){
		if(curr != &tasktable[j]){
		if(-1 == tasktable[j].CoreRunThisTask){
		if((-1 == tasktable[j].BindToCoreId) | (coreid == tasktable[j].BindToCoreId)){
		if(1 == tasktable[j].state){
		if(0 == __sync_lock_test_and_set(&tasktable[j].lock,1)){
			next = &tasktable[j];
			break;
		}	//try to own lock
		}	//this task is alive
		}	//bindto noone | bindto myself
		}	//no one running this
		}	//not same as curr
	}
	if(0 == next)return;

	//3.save curr from stack
	u8* buf = (void*)&curr->reg;
	u8* tmp = (void*)p;
	for(j=0;j<sizeof(struct saved_register);j++)buf[j] = tmp[j];
	curr->CoreRunThisTask = -1;			//no cpu @ this task
	*(u8*)( ((u64)curr-(u64)tasktable) / sizeof(struct taskstate)) = -1;	//for dbg

	//4.load next to stack
	buf = (void*)&next->reg;
	for(j=0;j<sizeof(struct saved_register);j++)tmp[j] = buf[j];
	next->CoreRunThisTask = coreid;		//this cpu @ this task
	__sync_lock_release(&next->lock);
	*(u8*)( ((u64)next-(u64)tasktable) / sizeof(struct taskstate)) = coreid;	//for dbg
}




void initthread()
{
	say("@initthread\n");

	int j;
	u8* tmp = (void*)tasktable;
	for(j=0;j<8*sizeof(struct taskstate);j++)tmp[j] = 0;

	taskcount = 0;
}
void freethread()
{
}