#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char


QWORD diskinfo;
int type=1;


void enumeratedisk();
void choosedisk(char* path);
void readdisk(QWORD rdi,QWORD rsi,QWORD rdx,QWORD rcx);
void whereisdiskinfo(QWORD* addr);
void say(char* fmt,...);

void initmemory();
void initwindow();
void initdisk();
void initlog();
void killwindow();
void killmemory();


void listall()
{
	//清理
	char* p=(char*)diskinfo;
	int i;
	for(i=0;i<100*0x100;i++)p[i]=0;

	//扫描
	enumeratedisk();
}
void choosetarget(QWORD in)
{
	if( in >100 )		//是一个内存地址
	{
		//第1种可能：文件的路径（比如d:\image\name.img）
		say("file:%s\n",(char*)in);
		type=1;
		choosedisk((char*)in);
	}
	else		//是一个数字
	{
		char* path=(char*)(diskinfo+0x100*in);

		//第2种可能：是个硬盘(比如："\\.\PHYSICALDRIVE0")
		say("disk:%s\n",path);
		type=2;
		choosedisk(path);
	}
}
//内存地址，第一扇区，请无视，总字节数
void readmemory(QWORD buf,QWORD startsector,QWORD disk,DWORD count)
{
	if(type==1 | type==2)
	{
		readdisk(buf,startsector,buf,count);
	}
}
















__attribute__((constructor)) void initroot()
{
	initmemory();	//初始化各个部分，放第一个，必须!必须!必须!重要说3遍!
	initlog();	//日志，放第二个，必须!必须!必须!重要说3遍!

	say("beforemain(){\n");
	say("inited memory\n");
	say("inited log\n");

	//必须在log之后不管几个
	initwindow();

	//只是拿地址
	initdisk();

	//列出所有能发现的
	whereisdiskinfo(&diskinfo);		//必须!
	listall();

        //cmdline
	char buffer[100];
	int temp;
	for(temp=0;temp<100;temp++)buffer[temp]=0;
	temp=open("/proc/self/cmdline",O_RDONLY);
	if(temp==-1)say("error reading cmd line\n");
	else
	{
		read(temp,buffer,100);
		close(temp);
		//printmemory(buffer,100);
		say("cmdline:%s\n",buffer);
	}

	//
	int signal=0;
	for(temp=0;temp<100;temp++)
	{
		if(buffer[temp]==0)
		{
			signal=1;
		}
		else            //!=0
		{
			if(signal==1)
			{
				signal=2;
				break;
			}
		}
	}
	say("arg0:%s,arg1:%s\n",buffer,buffer+temp);

	//"d:\code\file\a.exe"
	//比如上面这种，就默认打开扫描到的第一个磁盘
	if(signal==2)choosetarget((QWORD)buffer+temp);
	else choosetarget(0);

	say("}\n");
}
__attribute__((destructor)) void killroot()
{
	//清理
	//killdisk();
	killwindow();
	killmemory();
}
