#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#define QWORD unsigned long long
#define DWORD unsigned int




void disklist();
void diskchoose(QWORD);
void diskread(QWORD,QWORD,QWORD,QWORD);

void explainarg();

void initdisk(QWORD);
void killdisk();

void initprocess(QWORD);
void killprocess();

void initwindow(QWORD);
void killwindow();

void initlog();
void say(char* , ...);




static unsigned char* world;
	//[+0x000000,+0x0fffff]:		当前系统内有多少个能读写的东西

	//[+0x100000,+0x1fffff]:		某个硬盘(或文件)的分区情况
	//[+0x200000,+0x2fffff]:		某个分区的结构
	//[+0x300000,+0x3fffff]:		当前目录里面的文件
	//[+0x400000,+0x4fffff]:		buffer

	//[+0x500000,+0x5fffff]:		终端
static unsigned char* screen;
	//[+0x000000,+0x0fffff]:		f1
	//[+0x400000,+0x7fffff]:		f2
	//[+0x800000,+0xbfffff]:		f3
	//[+0xc00000,+0xffffff]:		f4




__attribute__((constructor)) void initeverything()
{
	//世界
	int i;
	world = (unsigned char*)malloc(0x600000);		//(1+4+1)MB
	{
		if(world==NULL)MessageBox(NULL, "can't allocmem for screen", "Hello Demo", MB_OK);
	}
	for(i=0;i<0x600000;i++)world[i]=0;

	//屏幕
	screen = (unsigned char*)malloc(0x400000);		//4M
	{
		if(world==NULL)MessageBox(NULL, "can't allocmem for screen", "Hello Demo", MB_OK);
	}

	//日志
	initlog(world+0x500000);
	say("beforemain(){\n");		//必须在log之后
	say("inited memory\n");
	say("inited log\n");

	//初始化
	initdisk( (QWORD)world );
	initprocess( (QWORD)world );
	initwindow((QWORD)screen);

	//不管是不是只有arg0
	disklist();
	explainarg();

	say("}\n");
}
__attribute__((destructor)) void killeverything()
{
	say("aftermain(){\n");

	killdisk();
	killprocess();
	killwindow();

	say("killed memory\n");
	say("}\n");			//world被摧毁之后什么都不能说

	free(world);
	free(screen);
}








QWORD whereisworld()
{
	return (unsigned long long)world;
}
QWORD whereisscreen()
{
	return (unsigned long long)screen;
}
void choosetarget(QWORD in)
{
	if( in >100 )		//是一个内存地址
	{
		//第1种可能：文件的路径（比如d:\image\name.img）
		say("file:%s\n",(char*)in);
		diskchoose(in);
	}
	else		//是一个数字
	{
		QWORD path=(QWORD)world+0x100*in;

		//第2种可能：是个硬盘(比如："\\.\PHYSICALDRIVE0")
		if( *(DWORD*)path == 0x5c2e5c5c )
		{
			say("disk:%s\n",path);
			diskchoose(path);
		}
	}
}
//内存地址，第一扇区，请无视，总字节数
void readmemory(QWORD buf,QWORD startsector,QWORD ignore,DWORD count)
{
	diskread(buf,startsector,0,count);
}