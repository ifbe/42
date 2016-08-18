#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>

#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char
void say(char* fmt,...);

HANDLE hDev;
static char tempname[0x20]={'\\','\\','.','\\','P','h','y','s','i','c','a','l','D','r','i','v','e','0','\0','\0'};




static QWORD getsize(HANDLE hand,char* path,char* dest)
{
	//say("%llx\n",*(QWORD*)path);
	if( *(QWORD*)path == 0x737968505c2e5c5c )
	{
		//磁盘大小这么拿
		GET_LENGTH_INFORMATION out;
		DWORD count;
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
		*(QWORD*)dest=out.Length.QuadPart;
		//say("%x\n",dest);
	}
	else
	{
		//文件大小这么拿
		GetFileSizeEx( hand , (PLARGE_INTEGER)dest );
	}
}




//mem地址，file名字，文件内偏移，总字节数
int mem2file(char* memaddr,char* filename,QWORD offset,QWORD count)
{
    HANDLE hFile;//文件句柄
    hFile=CreateFile(
        filename,//创建或打开的文件或设备的名称(这里是txt文件)。
        GENERIC_WRITE,// 文件访问权限,写
        0,//共享模式,这里设置0防止其他进程打开文件或设备
        NULL,//SECURITY_ATTRIBUTES结构，安全描述，这里NULL代表默认安全级别
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,//设置文件的属性，里面有高速缓存的选项
        NULL);

	//这里失败不会返回NULL，而是INVALID_HANDLE_VALUE
	if(hFile==INVALID_HANDLE_VALUE)
	{
		say("hFile error\n");
		return -1;
	}

	LARGE_INTEGER li;
	li.QuadPart = offset;
	SetFilePointer (hFile,li.LowPart,&li.HighPart,FILE_BEGIN);

	unsigned long dwBytesWritten = 0;
	WriteFile(hFile,memaddr,count,&dwBytesWritten,NULL);

	CloseHandle(hFile);
}
//
int file2mem(char* memaddr,char* filename,QWORD offset,QWORD count)
{
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
			*(QWORD*)(dest+0)=0x6b736964;
			*(QWORD*)(dest+8)=0x3f;

			//[0x10,0x17]:start
			//[0x18,0x1f]:end
			*(QWORD*)(dest+0x10)=0;
			*(QWORD*)(dest+0x18)=0;
			//say("%x\n",dest+0x18);
			getsize( temphandle , tempname , dest+0x18 );

			//[0x20,0x3f]:name
			for(j=0;j<0x20;j++)dest[j+0x20]=tempname[j];

			//next
			CloseHandle(temphandle);
			printf("%llx    ,    %llx    :    %s\n" ,*(QWORD*)(dest+0) , *(QWORD*)(dest+8) , (char*)(dest+0x10) );
			dest += 0x40;
		}
	}//10个记录
}
void choosefile(char* buf)
{
}
//目的内存地址，来源首扇区，无视，总字节数
void readfile(QWORD buf,QWORD startsector,QWORD ignore,DWORD count)
{
	LARGE_INTEGER li;
	li.QuadPart = startsector*512;
	SetFilePointer (hDev,li.LowPart,&li.HighPart,FILE_BEGIN);

	unsigned long dwret = 0;
	ReadFile(hDev,(char*)buf,count*512,&dwret,0);
	if(dwret!=count*512)printf("read %d bytes,GetLastError()=%d\n",dwret,GetLastError());
}
//来源内存地址，目的首扇区，无视，总字节数
void writefile(QWORD buf,QWORD startsector,QWORD ignore,DWORD count)
{
}








//
void startfile(char* path)
{
	//检查
	if(path[0]==0)return;

	//测试能否成功打开
	HANDLE temphandle=CreateFile(path,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
	if(temphandle == INVALID_HANDLE_VALUE)
	{
		say("(startfile error)createfile\n");
		return;
	}
	else CloseHandle(temphandle);

	//关掉原先已经打开的，然后打开这个
	if(hDev!=NULL)CloseHandle(hDev);
	hDev=CreateFile(path,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);

	//size
	QWORD size=0;
	getsize(hDev,path,(void*)&size);

	say("(%s    ,    %llx)\n",path,size);
}
void stopfile()
{
}
void createfile()
{
}
void deletefile()
{
	if(hDev!=NULL)
	{
		CloseHandle(hDev);
		hDev=NULL;
	}
}
