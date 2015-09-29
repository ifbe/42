#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>

#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char
void diary(char* fmt,...);








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
        diary("hFile error\n");
        return -1;
    }

	LARGE_INTEGER li;
	li.QuadPart = offset;
	SetFilePointer (hFile,li.LowPart,&li.HighPart,FILE_BEGIN);

	unsigned long dwBytesWritten = 0;
	WriteFile(hFile,memaddr,count,&dwBytesWritten,NULL);

	CloseHandle(hFile);
}
int file2mem(char* memaddr,char* filename,QWORD offset,QWORD count)
{
	
}








HANDLE hDev;
char* fileinfo;
void filelist()
{
	//printf("enu file\n");
	//暂时根本不管是什么，默认就是当前第一个硬盘
	BYTE tempname[20]={'\\','\\','.','\\','P','h','y','s','i','c','a','l','D','r','i','v','e','0','\0','\0'};
	DWORD i=0;
	char* p=(char*)fileinfo;
	for(i=0;i<0x10000;i++)
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
			printf("%d    \\\\.\\PHYSICALDRIVE%d\n",i,i);

			//打开成功的，path才会不等于0
			int j;
			for(j=0;j<20;j++)
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
void filetarget(QWORD in)
{
	char* path;
	if( in >100 )		//是一个内存地址
	{
		//第1种可能：文件的路径（比如d:\image\name.img）
		diary("file:%s\n",(char*)in);
		path=(char*)in;
	}
	else		//是一个数字
	{
		path=fileinfo+0x100*in;

		//第2种可能：是个硬盘(比如："\\.\PHYSICALDRIVE0")
		//if( *(DWORD*)path == 0x5c2e5c5c )
		//{
			//diary("disk:%s\n",path);
		//}
	}

	//测试能否成功打开
	HANDLE temphandle=CreateFile(path,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
	if(temphandle == INVALID_HANDLE_VALUE)
	{
		diary("cannot open:%s\n",path);
		return;
	}
	else CloseHandle(temphandle);

	//关掉原先已经打开的，然后打开这个
	if(hDev!=NULL)CloseHandle(hDev);
	hDev=CreateFile(path,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);

	return;
}
//目的内存地址，来源首扇区，无视，总字节数
void fileread(QWORD buf,QWORD startsector,QWORD ignore,DWORD count)
{
	LARGE_INTEGER li;
	li.QuadPart = startsector*512;
	SetFilePointer (hDev,li.LowPart,&li.HighPart,FILE_BEGIN);

	unsigned long dwret = 0;
	ReadFile(hDev,(char*)buf,count*512,&dwret,0);
	if(dwret!=count*512)printf("read %d bytes,GetLastError()=%d\n",dwret,GetLastError());
}
//来源内存地址，目的首扇区，无视，总字节数
void filewrite(QWORD buf,QWORD startsector,QWORD ignore,DWORD count)
{
	
}








void fileinit(QWORD addr)
{
	diary("disk initing\n");

	fileinfo=(char*)addr;
	filelist();
}
void filekill()
{
	if(hDev!=NULL)CloseHandle(hDev);
	diary("disk killed\n");
}