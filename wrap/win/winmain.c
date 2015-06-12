#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

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
	enumerateprocess();
}
void choosetarget(QWORD in)
{
	if( in >100 )		//是一个内存地址
	{
		//第1种可能：文件的路径（比如d:\image\name.img）
		type=1;
		choosedisk(in);
	}
	else		//是一个数字
	{
		char* path=(char*)(diskinfo+0x100*in);

		//第2种可能：是个硬盘(比如："\\.\PHYSICALDRIVE0")
		if( *(DWORD*)path == 0x5c2e5c5c )
		{
			type=2;
			choosedisk(path);
		}

		//第3种可能：是某个进程的虚拟内存（比如："a.exe"）
		else
		{
			type=3;
			//chooseprocess(number);
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
	else if(type==3)
	{
		myreadprocessmemory(buf, startsector*0x200, buf, count*0x200 );
	}
}
















//int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
//choosetarget((QWORD)lpCmdLine);
__attribute__((constructor)) void initroot()
{
	//初始化各个部分
	makememory();
	makewindow();


	//给这几个准备变量
	initlog();
	initdisk();
	initprocess();


	//列出所有能发现的，选择一个
	whereisdiskinfo(&diskinfo);
	listall();
	openfirst();
}
__attribute__((destructor)) void killroot()
{
	//清理
	killmemory();
	killwindow();
}