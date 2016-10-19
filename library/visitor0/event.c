#include<stdio.h>
#include<stdlib.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//event
int uievent(  u64* who, u64* what, u64* how);
int softevent(u64* who, u64* what, u64* how);
int hardevent(u64* who, u64* what, u64* how);
int bootevent(u64* who, u64* what, u64* how);
//
void signalcreate(void*,void*);
void signaldelete();
void birth();
void death();
//
void say(char*,...);




//
int sig = 0xff;




int eventread(u64* who, u64* what, u64* how)
{
	int ret;
	if(sig == 0)
	{
		what[0]=0;
		return 6;
	}
	else if(sig != 0xff)
	{
		what[0] = 0x746e69;
		how[0] = 0x3;		//ctrl+c=3, ctrl+z=0x1a

		sig = 0xff;
		return 5;
	}

	//调试端口有没有消息
	ret=bootevent(who, what, how);
	if(ret>0)return 4;

	//硬件中断完成状态报告
	ret=hardevent(who, what, how);
	if(ret>0)return 3;

	//输入/网络/系统事件
	ret=softevent(who, what, how);
	if(ret>0)return 2;

	//窗口关闭,窗口大小变化等情况
	ret=uievent(  who, what, how);
	if(ret>0)return 1;
}




void eventwrite(int who)
{
	if(sig==who)exit(-1);
	else sig = who;
	//say("int %d!\n",sig);
}




void eventcreate()
{
	signalcreate("c", eventwrite);
	//signalcreate("z", eventwrite);
}
void eventdelete()
{
}
