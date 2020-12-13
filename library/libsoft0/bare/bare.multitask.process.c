#include "libsoft.h"




struct saved_register{
	u64 r8;
	u64 r9;
	u64 r10;
	u64 r11;
	u64 r12;
	u64 r13;
	u64 r14;
	u64 r15;

	u64 rax;
	u64 rbx;
	u64 rcx;
	u64 rdx;
	u64 rsi;
	u64 rdi;
	u64 rbp;

	u64 ip;
	u64 cs;
	u64 flag;
	u64 sp;
	u64 ss;
}__attribute__((packed));

struct saved_pagetable{
}__attribute__((packed));

struct saved_vmcb{
}__attribute__((packed));

struct taskstate{
	int CoreRunThisTask;	//-1 = none, 0 to n = processor id
	struct saved_register reg;
	struct saved_pagetable* pt;
	struct saved_vmcb* vmcb;
}__attribute__((packed));




static struct taskstate tasktable[8];
static int taskcount = 0;




void scheduleprocess(struct saved_register* p, int coreid, int time)
{
	coreid = 0;

	//1.find curr: CoreRunThisTask == coreid
	int j;
	struct taskstate* curr = 0;
	for(j=0;j<taskcount;j++){
		if(coreid == tasktable[j].CoreRunThisTask){
			curr = &tasktable[j];
		}
	}
	if(0 == curr)return;

	//2.find next: if(first 1008/1024 of one sec){choose 0}, else{anyone not runing}
	struct taskstate* next = 0;
	if((time&0x3ff) < 0x3f0){
		next = &tasktable[0];
	}
	else{
		for(j=0;j<taskcount;j++){
			if(-1 == tasktable[j].CoreRunThisTask){
				next = &tasktable[j];
			}
		}
	}
	if(0 == next)return;
	if(next == curr)return;

	//3.save curr from stack
	u8* buf = (void*)&curr->reg;
	u8* tmp = (void*)p;
	for(j=0;j<sizeof(struct saved_register);j++)buf[j] = tmp[j];

	//4.load next to stack
	buf = (void*)&next->reg;
	for(j=0;j<sizeof(struct saved_register);j++)tmp[j] = buf[j];

	curr->CoreRunThisTask = -1;			//no cpu
	next->CoreRunThisTask = coreid;		//this cpu
}




static void test1()
{
	u64 time;
	while(1){
		time = timeread();
		say("pid=1: time=%llx\n",time);
		asm("hlt");
	}
}
static void test2()
{
	u64 time;
	while(1){
		time = timeread();
		say("pid=2: time=%llx\n",time);
		asm("hlt");
	}
}




void initprocess()
{
	int j;
	u8* tmp = (void*)tasktable;
	for(j=0;j<8*sizeof(struct taskstate);j++)tmp[j] = 0;

	tasktable[0].CoreRunThisTask = 0;

	tasktable[1].CoreRunThisTask = -1;
	tasktable[1].reg.ip = (u64)test1;
	tasktable[1].reg.cs = 0x10;
	tasktable[1].reg.flag = 0x202;
	tasktable[1].reg.sp = (u64)memorycreate(0x100000, 0);
	tasktable[1].reg.ss = 0x18;

	tasktable[2].CoreRunThisTask = -1;
	tasktable[2].reg.ip = (u64)test2;
	tasktable[2].reg.cs = 0x10;
	tasktable[2].reg.flag = 0x202;
	tasktable[2].reg.sp = (u64)memorycreate(0x100000, 0);
	tasktable[2].reg.ss = 0x18;

	taskcount = 3;
}
void freeprocess()
{
}




int readprocess()
{
	return 0;
}
int writeprocess()
{
	return 0;
}
int startprocess()
{
	return 0;
}
int stopprocess()
{
	return 0;
}
int createprocess()
{
	return 0;
}
int deleteprocess()
{
	return 0;
}