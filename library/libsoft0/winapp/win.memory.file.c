#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>
#include "libsoft.h"
int utf2unicode(void* src, void* dst);
void* system_alloc();




void initosfile(){
}
void freeosfile(){
}


_obj* getobjfromhandle(HANDLE hh)
{
	return 0;
}
HANDLE gethandlefromobj(_obj* oo)
{
	return (HANDLE)oo->fileinfo.fd;
}




static char name[0x20]={
	'\\','\\','.','\\',
	'P','h','y','s','i','c','a','l','D','r','i','v','e','0','\0','\0'};
static u64 getsize(HANDLE hand,char* path,char* dest)
{
	//logtoall("%llx\n",*(u64*)path);
	if( *(u64*)path == 0x737968505c2e5c5c )
	{
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
			logtoall("can't get size:%llx\n",GetLastError());
		}

		//
		*(u64*)dest=out.Length.QuadPart;
		//logtoall("%x\n",dest);
	}
	else
	{
		GetFileSizeEx( hand , (PLARGE_INTEGER)dest );
	}
}
void osfile_list(char* dest)
{
	//
	int num=0;
	int j=0;
	char tmp[0x1000];
	if(0==dest)dest = tmp;

	//clear
	for(num=0;num<0x1000;num++)
	{
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
			//logtoall("%x\n",dest+0x18);
			getsize(hand, name, dest+0x18 );

			//[0x20,0x3f]:name
			for(j=0;j<0x20;j++)dest[j+0x20]=name[j];

			//next
			CloseHandle(hand);
			printf("%.4s	[%llx,%llx]	%s\n" ,
				(u64*)(dest+0),
				*(u64*)(dest+0x10),
				*(u64*)(dest+0x18),
				(char*)(dest+0x20)
			);
			dest += 0x40;
		}
	}
}
int file_search(void* buf, int len)
{
	logtoall("@filesearch\n");
	return 0;
}
int file_modify(void* buf, int len)
{
	return 0;
}




_obj* file_create(void* orig, int flag)
{
	u8* path = orig;
	if(path == 0)return 0;
	if(path[0] == 0)return 0;

	int j,k;
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

	HANDLE fd;
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
		logtoall("error:%d@createfile:%s\n", GetLastError(), path);
		return 0;
	}

	_obj* oo = system_alloc();
	oo->fileinfo.fd = (u64)fd;
	return oo;
}
int file_delete(_obj* oo)
{
	HANDLE fd = gethandlefromobj(oo);
	return CloseHandle(fd);
}
int file_reader(_obj* oo, int xx, p64 arg, int cmd, void* mem, int len)
{
	LARGE_INTEGER li;
	DWORD val;
	int ret;

	HANDLE file = gethandlefromobj(oo);
	if(_pos_ == cmd){
		li.QuadPart = arg;
		SetFilePointer (file, li.LowPart, &li.HighPart, FILE_BEGIN);
	}//from head

	ret = ReadFile(file, mem, len, &val, 0);
	if(ret == 0)logtoall("file_reader: ret=%d,val=%d,error=%d\n", ret, val, GetLastError());
	return val;
}
int file_writer(_obj* oo, int xx, p64 arg, int cmd, void* mem, int len)
{
	LARGE_INTEGER li;
	DWORD val;
	int ret;

	HANDLE file = gethandlefromobj(oo);
	if(_pos_ == cmd){
		li.QuadPart = arg;
		SetFilePointer(file, li.LowPart, &li.HighPart, FILE_BEGIN);
	}//from head

	ret = WriteFile(file, mem, len, &val, NULL);
	if(ret == 0)logtoall("file_writer:ret=%d,val=%d,error=%d\n", ret, val, GetLastError());
	return val;
}
