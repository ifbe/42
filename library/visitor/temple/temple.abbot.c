#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdlib.h>
//libui
void characterinit(char*,char*);
void characterkill();
void windowinit(char*,char*);
void windowkill();
//libsoft
void arteryinit(char*,char*);
void arterykill();
void systeminit(char*,char*);
void systemkill();
//libhard
void bodyinit(char*,char*);
void bodykill();
void driverinit(char*,char*);
void driverkill();
//libboot
void debuginit(char*,char*);
void debugkill();
void basicinit(char*,char*);
void basickill();
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
static char*     basic=0;		//4m
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
	int size=4 * 0x400000;


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
	basic    = universe + 0;
	body     = universe + (1*0x400000);
	memory   = universe + (2*0x400000);
	character= universe + (3*0x400000);
	//say("basic@%llx,body@%llx,memory@%llx,character@%llx\n",basic,body,memory,character);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void birth()
{
	//必须放第一个
	inituniverse();	//16m

	//[0,4)：构架相关，以及内核日志
	basicinit( 0 , basic );
	say("[0,4):boot0 done\n");
	debuginit( 0 , basic );
	say("[0,4):boot1 done\n");

	//[4,7)：硬件驱动，以及底层协议栈
	driverinit( 0 , body );
	say("[4,8):hard0 done\n");
	bodyinit( 0 , body );
	say("[4,8):hard1 done\n");

	//[8,c)：文件读写，以及详细分析
	systeminit( 0 , memory );
	say("[8,c):soft0 done\n");
	arteryinit( 0 , memory );
	say("[8,c):soft1 done\n");

	//[c,f)：窗口开闭，以及用户界面
	windowinit( 0 , character );
	say("[12,16):ui0 done\n");
	characterinit( 0 , character );
	say("[12,16):ui1 done\n");
}
__attribute__((destructor)) void death()
{
	//4+4+4+4
	if(character != 0)
	{
		characterkill();
		windowkill();
		character=0;
	}
	if(memory != 0)
	{
		arterykill();
		systemkill();
		memory=0;
	}
	if(body != 0)
	{
		bodykill();
		driverkill();
		body=0;
	}
	if(basic != 0)
	{
		debugkill();
		basickill();
		basic=0;
	}

	//
	if(rawuniverse !=0)
	{
		free(rawuniverse);
		rawuniverse=0;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
