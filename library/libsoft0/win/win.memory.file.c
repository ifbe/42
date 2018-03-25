#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
int utf2unicode(void* src, void* dst);
void say(char* fmt,...);

static char name[0x20]={
	'\\','\\','.','\\',
	'P','h','y','s','i','c','a','l','D','r','i','v','e','0','\0','\0'};




static u64 getsize(HANDLE hand,char* path,char* dest)
{
	//say("%llx\n",*(u64*)path);
	if( *(u64*)path == 0x737968505c2e5c5c )
	{
		//磁盘大小这么拿
		GET_LENGTH_INFORMATION out;
		u32 len;
		int ret;

		//
		ret=DeviceIoControl
		(
			hand,
			IOCTL_DISK_GET_LENGTH_INFO,
			NULL,
			0,
			&out,
			sizeof(GET_LENGTH_INFORMATION),
			(LPDWORD)&len,
			NULL
		);
		if(ret==FALSE)
		{
			say("can't get size:%llx\n",GetLastError());
		}

		//
		*(u64*)dest=out.Length.QuadPart;
		//say("%x\n",dest);
	}
	else
	{
		//文件大小这么拿
		GetFileSizeEx( hand , (PLARGE_INTEGER)dest );
	}
}




void listfile(char* dest)
{
	//
	int num=0;
	int j=0;

	//clear
	for(num=0;num<0x10000;num++)
	{
		//全部清零
		dest[num]=0;
	}

	//
	for(num=0;num<10;num++)
	{
		name[17] = 0x30+num;
		HANDLE hand = CreateFile(name,
			GENERIC_READ,FILE_SHARE_READ,0,
			OPEN_EXISTING,0,0
		);
		if(hand != INVALID_HANDLE_VALUE)
		{
			//[0,7]:type
			//[8,f]:subtype
			*(u64*)(dest+0)=0x6b736964;
			*(u64*)(dest+8)=0x3f;

			//[0x10,0x17]:start
			//[0x18,0x1f]:end
			*(u64*)(dest+0x10)=0;
			*(u64*)(dest+0x18)=0;
			//say("%x\n",dest+0x18);
			getsize(hand, name, dest+0x18 );

			//[0x20,0x3f]:name
			for(j=0;j<0x20;j++)dest[j+0x20]=name[j];

			//next
			CloseHandle(hand);
			printf("%llx,	%llx:	%s\n" ,
				*(u64*)(dest+0),
				*(u64*)(dest+8),
				(char*)(dest+0x10)
			);
			dest += 0x40;
		}
	}//10个记录
}
void choosefile(char* buf)
{
}
int writefile(HANDLE file, u64 off, u8* mem, u64 len)
{
	LARGE_INTEGER li;
	DWORD val;
	int ret;

	li.QuadPart = off;
	SetFilePointer(file, li.LowPart, &li.HighPart, FILE_BEGIN);
	ret = WriteFile(file, mem, len, &val, NULL);
	if(ret == 0)say("ret=%d,val=%d,error=%d\n", ret, val, GetLastError());
	return val;
}
int readfile(HANDLE file, u64 off, u8* mem, u64 len)
{
	LARGE_INTEGER li;
	DWORD val;
	int ret;

	li.QuadPart = off;
	SetFilePointer (file, li.LowPart, &li.HighPart, FILE_BEGIN);
	ret = ReadFile(file, mem, len, &val, 0);
	if(ret == 0)say("ret=%d,val=%d,error=%d\n", ret, val, GetLastError());
	return val;
}
HANDLE startfile(u8* path, int flag)
{
	int j,k;
	HANDLE fd;
	if(path == 0)return 0;
	if(path[0] == 0)return 0;

	k = 0;
	for(j=0;j<0x1000;j++)
	{
		if(path[j] >= 0x80)
		{
			k = 0xc0;
			break;
		}
	}

	if('w' == flag)flag = OPEN_ALWAYS;
	else flag = OPEN_EXISTING;

	if(k < 0x80)
	{
		fd	= CreateFileA(
			path,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ,
			0,
			flag,
			FILE_ATTRIBUTE_NORMAL,
			0
		);
	}
	else
	{
		u16 str[0x1000];
		k = 0;

		for(j=0;j<0x1000;j++)
		{
			if(path[j] < 0x20)
			{
				str[k] = 0;
				break;
			}
			else if(path[j] < 0x80)
			{
				str[k] = path[j];
			}
			else
			{
				j += utf2unicode(path+j, &str[k]) - 1;
			}

			//printf("%x ", str[k]);
			k += 1;
		}
		//printf("\n");

		fd	= CreateFileW(
			str,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ,
			0,
			flag,
			FILE_ATTRIBUTE_NORMAL,
			0
		);
	}

	if(INVALID_HANDLE_VALUE == fd)
	{
		say("error:%d@createfile:%s\n", GetLastError(), path);
		return 0;
	}
	return fd;
}
void stopfile(HANDLE fd)
{
	CloseHandle(fd);
}
void createfile()
{
}
void deletefile()
{
}
