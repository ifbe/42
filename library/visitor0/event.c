#include<stdio.h>
#include<stdlib.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//event
int   uievent(u64* what, u64* who, u64* where, u64* when);
int softevent(u64* what, u64* who, u64* where, u64* when);
int hardevent(u64* what, u64* who, u64* where, u64* when);
int bootevent(u64* what, u64* who, u64* where, u64* when);
//libhard
int snatch(void*);
int release(void*);
//libboot
void say(char*,...);




//
int lock = 0;
int sig = 0xff;
//
static int enq = 0;
static int deq = 0;
static char eventqueue[0x100000];




int eventread(u64* what, u64* who, u64* where, u64* when)
{
	int ret;
	if(sig == 0)
	{
		who[0]=0;
		return 6;
	}
	else if(sig != 0xff)
	{
		who[0] = 0x746e69;
		what[0] = sig;		//ctrl+c=3, ctrl+z=0x1a
		sig = 0xff;

		return 5;
	}

	//调试端口有没有消息
	ret=bootevent(what, who, where, when);
	if(ret>0)return 4;

	//硬件中断完成状态报告
	ret=hardevent(what, who, where, when);
	if(ret>0)return 3;

	//输入/网络/系统事件
	ret=softevent(what, who, where, when);
	if(ret>0)return 2;

	//窗口关闭,窗口大小变化等情况
	ret=  uievent(what, who, where, when);
	if(ret>0)return 1;
}




void eventwrite(int who)
{
	int next;
	static u64* p;

	//safely update the pointer
	snatch(&lock);
	next = (enq+0x20)%0x100000;
	if(next == deq)
	{
		//full
		release(&lock);
		say("queue full, droping event!\n");
		return;
	}
	enq = next;
	release(&lock);

	//put event to place
	p = (u64*)(eventqueue + enq);
	p[0] = 0;	//what
	p[1] = 0;	//who
	//p[2] = 0;	//where
	//p[3] = 0;	//when

	//......
	if(sig==who)exit(-1);
	else sig = who;
	//say("int %d!\n",sig);
}
