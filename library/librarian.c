#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

#include<stdio.h>
#include<stdlib.h>
void filelist();
void filetarget(QWORD);
void fileread(QWORD,QWORD,QWORD,QWORD);
void fileinit(QWORD);

//8m
static char*   world;
static char*   inbuf;	//+0x400000
static char*  outbuf;	//+0x500000
static char*  logbuf;	//+0x600000
static char* listbuf;	//+0x700000

//4m
static char* data;
static unsigned long long resolution=(768<<16)+1024;




//+0x100000
void say(char* rcx,QWORD rdx,QWORD r8,QWORD r9)
{
	int start=0;
	int temp=0;
	QWORD offsety;
	QWORD offsetx;
	while(1)			//举例“123443%d\n\n    44532”
	{
		if(rcx[temp] == 0)		//0
		{
			offsety=*(DWORD*)(outbuf+0xffff0);
			offsetx=*(DWORD*)(outbuf+0xffff8);
			int writecount=snprintf(outbuf+offsety+offsetx,0x80-offsetx,
					rcx+start,rdx,r8,r9);

			*(DWORD*)(outbuf+0xffff8)+=writecount;

			break;
		}
		if( rcx[temp] < 0x10)	//"\n":0xa?		0xd?
		{
			offsety=*(DWORD*)(outbuf+0xffff0);
			offsetx=*(DWORD*)(outbuf+0xffff8);
			snprintf(outbuf+offsety+offsetx,0x80-offsetx,
					rcx+start,rdx,r8,r9);

			*(DWORD*)(outbuf+0xffff8)=0;
			*(DWORD*)(outbuf+0xffff0)=(offsety+0x80)%0xffff0;
			temp+=1;
			start=temp;

			continue;
		}
		temp++;
	}
}

//+0x200000
void diary(char* rcx,...)
{
	asm("jmp printf");
}

//+0x300000
void list()			//ls?
{
	filelist();
}
void target(QWORD what)		//choose是soft做的...名字不能一样...
{
	filetarget(what);
}
void readmemory(QWORD buf,QWORD sector,QWORD ignore,DWORD count)		//read?
{
	//if(this is disk or file)
	fileread(buf,sector,0,count);
}
void writememory()		//write?
{
	
}








//++++++++++++++++++++++++++++
char* whereisworld()
{
	return world;
}
void initworld()
{
	//8m
	int i=0;
	char* world= (char*)malloc(0x800000);		//8M
	if(world == NULL){printf("can't allloc memory for world\n");exit(-1);}
	for(i=0;i<0x800000;i++)world[i]=0;

	//memory
	inbuf  = world+0x400000;
	outbuf = world+0x500000;
	logbuf = world+0x600000;
	listbuf= world+0x700000;

	//finally
	fileinit(listbuf);
}
void killworld()
{
	free(world);
}








//跟屏幕有关的只有这四个函数
void writewindow(QWORD first,QWORD second)
{
	/*
	//总共256个屏幕
	if( first < 256 )writescreen(first);

	//设置当前窗口大小
	if( first=='size' )

	//设置当前屏幕属性
	if hide
	...
	*/
}
void readwindow(QWORD first,QWORD second)
{
	/*
	//想要当前窗口数据
	if( first < 256 )return data;

	//想要当前窗口大小
	if( first == 'size' )
	return x+(y<<16)+(z<<32)+(w<<48)

	//想要其他东西
	*/
}
void openwindow()
{
	//
	data = (char*)malloc(0x400000);			//4M
	if(data == NULL){printf("can't allloc memory for screen\n");exit(-1);}

	//screen
	initwindow(data);
}
void closewindow()
{
	killwindow();
	free(data);
}