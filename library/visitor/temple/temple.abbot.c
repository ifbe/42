#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdlib.h>
//libui
void initcharacter(char*);
void killcharacter();
void initwindow(char*);
void killwindow();
//libsoft
void initartery(char*);
void killartery();
void initsystem(char*);
void killsystem();
//libhard
void initbody(char*);
void killbody();
void initdriver(char*);
void killdriver();
//libboot
void initdebug(char*);	//listen,say
void killdebug();
void initbasic(char*);	//
void killbasic();
//event
int uievent(QWORD* first,QWORD* second);
int softevent(QWORD* first,QWORD* second);
int hardevent(QWORD* first,QWORD* second);
int bootevent(QWORD* first,QWORD* second);
//
void say(char*,...);








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
	initsystem( memory );
	say("[8,c):soft0 done\n");
	initartery( memory );
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
		killartery();
		killsystem();
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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
