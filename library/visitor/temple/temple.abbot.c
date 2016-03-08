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
void initsoftware(char*);
void killsoftware();
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
//final
void final_init(char*);
void final_kill(char*);
void final_open(char*);
void final_close(char*);
void final_list(char*);
void final_choose(char*);
void final_read(char*);
void final_write(char*);
//
int buf2arg(char*,char**,char**);
int compare(char*,char*);
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








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int stillalive=1;
int waitevent(QWORD* first,QWORD* second)
{
        int ret;
        //say("here\n");
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
int command(char* buffer)
{
        int ret;
        char* arg0;
        char* arg1;
        //say("command=%s\n",buffer);
        buf2arg(buffer,&arg0,&arg1);
        if(arg0==0)return 0;

        //'q'
        if(arg0[0]=='q')
        {
                stillalive=0;
                return 0;
        }
        //'exit
        if((arg0[0]==0x65)&&(arg0[1]==0x78)&&(arg0[2]==0x69)&&(arg0[3]==0x74))
        {
                stillalive=0;
                return 0;
        }
        //'help'
        if((arg0[0]==0x68)&&(arg0[1]==0x65)&&(arg0[2]==0x6c)&&(arg0[3]==0x70))
        {
                //"create","destory","start","stop"
                say("init ?             =init=make=fabricate\n");
                say("kill ?             =kill=smash=wreck\n");
                say("open ?             =mount=enter=start\n");
                say("close ?            =unmount=leave=stop\n\n");

                //"observe","change","get","put"
                say("ls ?               =list=summary=view=check\n");
                say("cd ?               =choose=into=switch=clap\n");
                say("read ?             =load=get=eat=copy\n");
                say("write ?            =store=put=spit=paste\n\n");

                return 1;
        }

        //"create","destory","start","stop"
        ret=compare( arg0 , "init");
        if(ret==0)
        {
                final_init(arg1);
                return 2;
        }
        ret=compare( arg0 , "kill");
        if(ret==0)
        {
                final_kill(arg1);
                return 2;
        }
        ret=compare( arg0 , "open");
        if(ret==0)
        {
                final_open(arg1);
                return 2;
        }
        ret=compare( arg0 , "close");
        if(ret==0)
        {
                final_close(arg1);
                return 2;
        }

        //"observe","change","get","put"
        ret=compare( arg0 , "ls" );
        if(ret==0)
        {
                final_list(arg1);
                return 4;
        }
        ret=compare( arg0 , "cd" );
        if(ret==0)
        {
                final_choose(arg1);
                return 4;
        }
        ret=compare( arg0 , "read" );
        if(ret==0)
        {
                final_read(arg1);
                return 4;
        }
        ret=compare( arg0 , "write" );  //dangerous
        if(ret==0)
        {
                final_write(arg1);
                return 4;
        }

	//
        return 8;
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
	initsystem( memory );
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
