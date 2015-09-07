#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>

#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char




void say(char* fmt,...);




char* diskinfo;
HANDLE hDev;




void disklist()
{
	//printf("enu disk\n");
	//disk暂时根本不管是什么，默认就是当前第一个硬盘
	BYTE tempname[20]={'\\','\\','.','\\','P','h','y','s','i','c','a','l','D','r','i','v','e','0','\0','\0'};
	DWORD i=0;
	char* p=(char*)diskinfo;
	for(i=0;i<0x1000;i++)
	{
		//全部清零
		p[i]=0;
	}
	for(i=0;i<10;i++)
	{
		tempname[17]=0x30+i;
		HANDLE temphandle=CreateFile(tempname,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
		if(temphandle != INVALID_HANDLE_VALUE)
		{
			//
			CloseHandle(temphandle);
			//printf("%d    \\\\.\\PHYSICALDRIVE%d\n",i,i);

			//打开成功的，path才会不等于0
			int j=0;
			for(;j<20;j++)
			{
				p[i*0x100+j]=tempname[j];
			}
		}
		else
		{
			//printf("physicaldrive%d,GetLastError()=:%d\n",i,GetLastError());
		}
	}//10个记录
}
void diskchoose(QWORD in)
{
	char* path;
	if( in >100 )		//是一个内存地址
	{
		//第1种可能：文件的路径（比如d:\image\name.img）
		path=(char*)in;
		say("file:%s\n",path);
	}
	else
	{
		path=(char*)(diskinfo+0x100*in);
		if( *(DWORD*)path == 0x5c2e5c5c )		//是一个数字
		{
			//第2种可能：是个硬盘(比如："\\.\PHYSICALDRIVE0")
			say("disk:%s\n",path);
		}
	}




	//测试能否成功打开
	HANDLE temphandle=CreateFile(path,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
	if(temphandle == INVALID_HANDLE_VALUE)
	{
		say("cannot open:%s\n",path);
		return;
	}
	else CloseHandle(temphandle);

	//关掉原先已经打开的，然后打开这个
	if(hDev!=NULL)CloseHandle(hDev);
	hDev=CreateFile(path,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);

	return;
}
//内存地址，第一扇区，请无视，总字节数
void diskread(QWORD buf,QWORD startsector,QWORD disk,DWORD count)
{
	LARGE_INTEGER li;
	li.QuadPart = startsector*512;
	SetFilePointer (hDev,li.LowPart,&li.HighPart,FILE_BEGIN);

	unsigned long dwret = 0;
	ReadFile(hDev,(char*)buf,count*512,&dwret,0);
	if(dwret!=count*512)printf("read %d bytes,GetLastError()=%d\n",dwret,GetLastError());
}








void initdisk(QWORD addr)
{
	diskinfo=(char*)addr;
	say("inited disk\n");
}
void killdisk()
{}