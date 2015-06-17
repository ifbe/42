#include <stdio.h>
#include <stdlib.h>

#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char


QWORD diskinfo;
int type=1;




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
		choosedisk(in);
	}
	else		//是一个数字
	{
		char* path=(char*)(diskinfo+0x100*in);

		//第2种可能：是个硬盘(比如："\\.\PHYSICALDRIVE0")
		if( *(DWORD*)path == 0x5c2e5c5c )
		{
			say("disk:%s\n",path);
			type=2;
			choosedisk(path);
		}

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
	//初始化各个部分，放第一个，必须!必须!必须!重要说3遍!
	initmemory();		//只有这里不能用say();

	//日志，放第二个，必须!必须!必须!重要说3遍!
	initlog();

	//必须在log之后不管几个
	initwindow();

	//只是拿地址
	initdisk();

	//列出所有能发现的
	whereisdiskinfo(&diskinfo);		//必须!
	listall();

	//"d:\code\file\a.exe"
	//比如上面这种，就默认打开扫描到的第一个磁盘
	choosetarget(0);
}
__attribute__((destructor)) void killroot()
{
	//清理
	//killdisk();
	killwindow();
	killmemory();
}
