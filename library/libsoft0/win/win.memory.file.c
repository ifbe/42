#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>

#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
void say(char* fmt,...);

static HANDLE selected;
static char tempname[0x20]={'\\','\\','.','\\','P','h','y','s','i','c','a','l','D','r','i','v','e','0','\0','\0'};




static u64 getsize(HANDLE hand,char* path,char* dest)
{
	//say("%llx\n",*(u64*)path);
	if( *(u64*)path == 0x737968505c2e5c5c )
	{
		//磁盘大小这么拿
		GET_LENGTH_INFORMATION out;
		u32 count;
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
			(LPDWORD)&count,
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




//file名字，mem地址，文件内偏移，总字节数
int writefile(u8* filename,u8* memaddr,u64 offset,u64 count)
{
	HANDLE hFile;
	LARGE_INTEGER li;
	unsigned long written = 0;

	if(filename != 0)
	{
		li.QuadPart = offset;
		SetFilePointer (selected, li.LowPart, &li.HighPart, FILE_BEGIN);
		WriteFile(selected, memaddr, count, &written, NULL);
	}
	else
	{
		//
		hFile = CreateFile(
			filename, GENERIC_WRITE, 0,
			NULL, OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL
		);
		if(hFile==INVALID_HANDLE_VALUE)
		{
			say("error@open\n");
			return -1;
		}

		if(offset != 0)
		{
			li.QuadPart = offset;
			SetFilePointer (hFile,li.LowPart,&li.HighPart,FILE_BEGIN);
		}

		//
		WriteFile(hFile, memaddr, count, &written, NULL);

		//
		CloseHandle(hFile);
	}
}
//
int readfile(u8* filename,u8* memaddr,u64 offset,u64 count)
{
	HANDLE hFile;
	LARGE_INTEGER li;
	unsigned long written = 0;

	if(filename == 0)
	{
		li.QuadPart = offset;
		SetFilePointer (selected, li.LowPart, &li.HighPart, FILE_BEGIN);
		ReadFile(selected, memaddr, count, &written, 0);
	}
	else 
	{
		//
		hFile = CreateFile(
			filename, GENERIC_WRITE, 0,
			NULL, OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL
		);
		if(hFile==INVALID_HANDLE_VALUE)
		{
			say("error@open\n");
			return -1;
		}

		if(offset != 0)
		{
			li.QuadPart = offset;
			SetFilePointer (hFile, li.LowPart, &li.HighPart, FILE_BEGIN);
		}

		//
		ReadFile(hFile, memaddr, count, &written, 0);

		//
		CloseHandle(hFile);
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
		tempname[17]=0x30+num;
		HANDLE temphandle=CreateFile(tempname,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
		if(temphandle != INVALID_HANDLE_VALUE)
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
			getsize( temphandle , tempname , dest+0x18 );

			//[0x20,0x3f]:name
			for(j=0;j<0x20;j++)dest[j+0x20]=tempname[j];

			//next
			CloseHandle(temphandle);
			printf("%llx	,	%llx	:	%s\n" ,*(u64*)(dest+0) , *(u64*)(dest+8) , (char*)(dest+0x10) );
			dest += 0x40;
		}
	}//10个记录
}
void choosefile(char* buf)
{
}




//
int startfile(char* path)
{
	//检查
	if(path[0]==0)return 0;

	//测试能否成功打开
	HANDLE temphandle=CreateFile(path,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
	if(temphandle == INVALID_HANDLE_VALUE)
	{
		say("(error%d)createfile:%x\n",errno,temphandle);
		return 0;
	}
	else CloseHandle(temphandle);

	//关掉原先已经打开的，然后打开这个
	if(selected!=NULL)CloseHandle(selected);
	selected=CreateFile(path,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);

	//size
	u64 size=0;
	getsize(selected,path,(void*)&size);

	say("(%s	,	%llx)\n",path,size);
	return 1;
}
void stopfile()
{
	if(selected!=NULL)
	{
		CloseHandle(selected);
		selected=NULL;
	}
}
void createfile()
{
}
void deletefile()
{
	stopfile();
}
