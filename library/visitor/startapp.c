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
void sleep_us(int);
u64 gettime();
void arterycreate(char*,char*);
void arterydelete();
void systemcreate(char*,char*);
void systemdelete();
//libhard
int snatch(void*);
int release(void*);
void bodycreate(char*,char*);
void bodydelete();
void drivercreate(char*,char*);
void driverdelete();
//libboot
void debugcreate(char*,char*);
void debugdelete();
void basiccreate(char*,char*);
void basicdelete();
//
void say(void*, ...);




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//raw memory
static char* rawuniverse=0;	//unaligned
static char* universe=0;	//aligned
//processed memory
static char*     basic=0;		//4m
static char*      body=0;		//4m
static char*    memory=0;		//4m
static char* character=0;		//4m
//
static int enq = 0;
static int deq = 0;
static int lock = 0;
//
static u64 haha[4];
static char eventqueue[0x100000];




//get address
void createuniverse()
{
	//temp
	u64 i;
	u64 temp;
	int size=2 * 0x400000;
	//int size=4 * 0x400000;


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

/*
	//4.分配内存
	basic    = universe + 0;
	body     = universe + (1*0x400000);
	memory   = universe + (2*0x400000);
	character= universe + (3*0x400000);
*/
	//4.分配内存
	basic    = 0;
	body     = 0;
	memory   = universe + (0*0x400000);
	character= universe + (1*0x400000);
	//say("basic@%llx,body@%llx,memory@%llx,character@%llx\n",basic,body,memory,character);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
__attribute__((destructor)) void death()
{
	//
	printf("\n");

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

		exit(-1);
	}
}
void* birth()
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

	return memory;
}
void eventwrite(u64 why, u64 what, u64 where, u64 when)
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
		say("droping event: %llx,%llx,%llx,%llx\n", why, what, where, when);
		return;
	}
	enq = temp;
	release(&lock);

	//put event to place
	p = (u64*)(eventqueue + this);
	p[0] = why;
	p[1] = what;
	p[2] = where;   //where
	p[3] = when;    //when

	//debug
	//say("%llx,%llx,%llx,%llx\n", p[0], p[1], p[2], p[3]);
}
void* eventread()
{       
	int ret;
	if(enq != deq)
	{
		ret = deq;
		deq = (deq+0x20)%0x100000;
		return eventqueue + ret;
	}
	
	//haha[0] = 0;
	haha[1] = 0x656d6974;
	//haha[2] = 0;
	haha[3] = gettime() + 10000;
	
	sleep_us(10000);
	return haha;
}       
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
