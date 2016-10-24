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
//libsoft
void sleep_us(int);
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




void* eventread()
{
	int ret;
	while(enq == deq)
	{
		sleep_us(10000);
	}
	ret = deq;
	deq = (deq+0x20)%0x100000;
	return eventqueue + ret;
/*
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
*/
}




void eventwrite(u64 what, u64 who)
{
	int this,temp;
	static u64* p;

	//safely update the pointer
	snatch(&lock);
	this = enq;
	temp = (this+0x20)%0x100000;
	if(temp == deq)
	{
		//full
		release(&lock);
		say("queue full, droping event!\n");
		return;
	}
	enq = temp;
	release(&lock);

	if(who == 0x64626b)
	{
		if(what == 3)
		{
			temp = (this+0x100000-0x20)%0x100000;
			p = (u64*)(eventqueue + temp);
			if(p[1] == 0x64626b)
			{
				if(p[0] == 3)exit(-1);
			}
		}
	}

	//put event to place
	p = (u64*)(eventqueue + this);
	p[0] = what;
	p[1] = who;
	//p[2] = 0;	//where
	//p[3] = 0;	//when

	//debug
	//say("%llx,%llx\n",p[0],p[1]);
}
