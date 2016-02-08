#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdlib.h>
//libui
int uicommand(char*);
int uievent(QWORD* first,QWORD* second);
void initcharacter(char*);
void killcharacter();
void initwindow(char*);
void killwindow();
//libsoft
int softcommand(char*);
int softevent(QWORD* first,QWORD* second);
void initsoftware(char*);
void killsoftware();
void initmemory(char*);
void killmemory();
//libhard
int hardcommand(char*);
int hardevent(QWORD* first,QWORD* second);
void initbody(char*);
void killbody();
void initdriver(char*);
void killdriver();
//libboot
int bootcommand(char*);
int bootevent(QWORD* first,QWORD* second);
void initdebug(char*);	//listen,say
void killdebug();
void initbasic(char*);	//
void killbasic();
//
void say(char*,...);








//raw memory
static char* rawuniverse=0;	//unaligned
static char* universe=0;	//aligned
//processed memory
static char*     world=0;		//4m
static char*      body=0;		//4m
static char*    memory=0;		//4m
static char* character=0;		//4m
//return address
char* whereismemory()
{
	return memory;
}
char* whereischaracter()
{
	return character;
}
//get address
void inituniverse()
{
	//temp
	QWORD i;
	QWORD temp;
	int size=2 * 0x400000;


	//1.申请内存，为了对齐要多申请0x1000
	rawuniverse=malloc(size+0x1000);
	if(rawuniverse==NULL)
	{
		printf("no enough momery\n");
		exit(-1);
	}


	//2.把申请到的进行一次0x1000对齐
	//[0x   0,0x1401000)	->	[0x1000,0x1401000)
	//[0x 234,0x1401234)	->	[0x1000,0x1401000)
	//[0x fff,0x1401fff)	->	[0x1000,0x1401000)
	//[0x1000,0x1402000)	->	[0x2000,0x1402000)
	temp = ( (QWORD)rawuniverse ) & 0xfff;
	universe = rawuniverse + 0x1000 - temp;


	//3.清零
	for(i=0;i<size;i++)
	{
		universe[i]=0;
	}

	//4.分配内存
	//world=universe + 0;
	//body=universe + (1*0x400000);
	//memory=universe + (2*0x400000);
	//character=universe + (3*0x400000);
	memory=universe + 0;
	character=universe + 0x400000;
}








int stillalive=1;
int waitevent(QWORD* first,QWORD* second)
{
	int ret;
	if(stillalive==0)
	{
		first[0]=0;
		return 0;
	}

	//调试端口有没有消息
	ret=bootevent(first,second);
	if(ret>0)return 11;

	//硬件中断完成状态报告
	ret=hardevent(first,second);
	if(ret>0)return 22;

	//输入/网络/系统事件
	ret=softevent(first,second);
	if(ret>0)return 33;

	//窗口关闭,窗口大小变化等情况
	ret=uievent(first,second);
	if(ret>0)return 44;
}








int helpme(char* p)
{
	unsigned char cmd[4];
	unsigned int temp;

	*(unsigned int*)cmd=0;
	for(temp=0;temp<4;temp++)
	{
		if(p[temp]==0)break;
		else cmd[temp]=p[temp];
	}
	temp=*(unsigned int*)cmd;
	//say("%x\n",temp);

	//'q'
	if(temp=='q')
	{
		stillalive=0;
		return 1;
	}

	//0x74697865='exit'
	if(temp==0x74697865)
	{
		stillalive=0;
		return 1;
	}

	//0x706c6568='help'
	else if(temp==0x706c6568)
	{
		//physical
		say("help ?             (help what)\n");
		say("list ?             (list physical)\n");
		say("into ?             (choose one)\n");
		say("read ?             (physical print)\n");
		say("write ?            (not allowed currently)\n\n");

		//logical
		say("mount ?            (mount which)\n");
		say("ls ?               (list logical)\n");
		say("cd ?               (change directory)\n");
		say("load ?             (load this file)\n");
		say("store ?            (store this file)\n");

		return 2;
	}

	return 0;
}
int command(char* p)
{
	//?
	int ret=helpme(p);
	if(ret>0)return 1;

	//在libboot里面找
	//say("searching libboot\n");
	ret=bootcommand(p);
	if(ret>0)return 11;

	//在libhard里面找
	//say("searching libhard\n");
	ret=hardcommand(p);
	if(ret>0)return 21;

	//在libsoft里面找
	//say("searching libsoft\n");
	ret=softcommand(p);
	if(ret>0)return 31;

	//在libui里面找
	//say("searching libui\n");
	ret=uicommand(p);
	if(ret>0)return 41;

	//在host系统上找

	//没找到
	//say("unknown command:%s\n",p);
	return 99;
}








void birth()
{
	//必须放第一个
	inituniverse();	//16m

	//[0,4)：构架相关，以及内核日志
	initbasic( world );
	say("[0,4):boot0 done\n");
	initdebug( world );
	say("[0,4):boot1 done\n");

	//[4,7)：硬件驱动，以及底层协议栈
	initdriver( body );
	say("[4,8):hard0 done\n");
	initbody( body );
	say("[4,8):hard1 done\n");

	//[8,c)：文件读写，以及详细分析
	initmemory( memory );
	say("[8,c):soft0 done\n");
	initsoftware( memory );
	say("[8,c):soft1 done\n");

	//[c,f)：窗口开闭，以及用户界面
	initwindow( character );
	say("[12,16):ui0 done\n");
	initcharacter( character );
	say("[12,16):ui1 done\n");
}
__attribute__((destructor)) void death()
{
	//4+4+4+4
	if(character != 0)
	{
		killcharacter();
		killwindow();
		character=0;
	}
	if(memory != 0)
	{
		killsoftware();
		killmemory();
		memory=0;
	}
	if(body != 0)
	{
		killbody();
		killdriver();
		body=0;
	}
	if(world != 0)
	{
		killdebug();
		killbasic();
		world=0;
	}

	//
	if(rawuniverse !=0)
	{
		free(rawuniverse);
		rawuniverse=0;
	}
}
