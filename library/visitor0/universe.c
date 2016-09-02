#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include<stdio.h>
#include<stdlib.h>
//libui
void charactercreate(char*,char*);
void characterdelete();
void displaycreate(char*,char*);
void displaydelete();
//libsoft
void arterycreate(char*,char*);
void arterydelete();
void systemcreate(char*,char*);
void systemdelete();
//libhard
void bodycreate(char*,char*);
void bodydelete();
void drivercreate(char*,char*);
void driverdelete();
//libboot
void debugcreate(char*,char*);
void debugdelete();
void basiccreate(char*,char*);
void basicdelete();
//slave
void commandcreate(char*,char*);
void commanddelete();
void eventcreate(char*,char*);
void eventdelete();
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
//get address
void createuniverse()
{
	//temp
	u64 i;
	u64 temp;
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
	temp = ( (u64)rawuniverse ) & 0xfff;
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
	createuniverse();	//16m

	//[0,4)：构架相关，以及内核日志
	basiccreate( 0 , basic );
	debugcreate( 0 , basic );

	//[4,7)：硬件驱动，以及底层协议栈
	drivercreate( 0 , body );
	bodycreate( 0 , body );

	//[8,c)：文件读写，以及详细分析
	systemcreate( 0 , memory );
	arterycreate( 0 , memory );

	//[c,f)：窗口开闭，以及用户界面
	displaycreate( 0 , character );
	charactercreate( 0 , character );

	//
	commandcreate(0,universe);
	eventcreate(0,universe);
}
__attribute__((destructor)) void death()
{
	//
	if(rawuniverse != 0)
	{
		eventdelete();
		commanddelete();
	}

	//libui
	if(character != 0)
	{
		characterdelete();
		displaydelete();
		character = 0;
	}

	//libsoft
	if(memory != 0)
	{
		arterydelete();
		systemdelete();
		memory = 0;
	}

	//libhard
	if(body != 0)
	{
		bodydelete();
		driverdelete();
		body = 0;
	}

	//libboot
	if(basic != 0)
	{
		debugdelete();
		basicdelete();
		basic = 0;
	}

	//
	if(rawuniverse != 0)
	{
		free(rawuniverse);
		rawuniverse=0;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
