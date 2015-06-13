#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>

#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char


HANDLE hDev;
char* diskinfo;
static BYTE tempname[20]={'\\','\\','.','\\','P','h','y','s','i','c','a','l','D','r','i','v','e','0','\0','\0'};




void enumeratedisk()
{
	//printf("enu disk\n");
	//disk暂时根本不管是什么，默认就是当前第一个硬盘
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
void choosedisk(char* path)
{
	//say("choosedisk:%s\n",path);
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
void readdisk(QWORD buf,QWORD startsector,QWORD disk,DWORD count)
{
	LARGE_INTEGER li;
	li.QuadPart = startsector*512;
	SetFilePointer (hDev,li.LowPart,&li.HighPart,FILE_BEGIN);

	unsigned long dwret = 0;
	ReadFile(hDev,(char*)buf,count*512,&dwret,0);
	if(dwret!=count*512)printf("read %d bytes,GetLastError()=%d\n",dwret,GetLastError());
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
        printf("hFile error\n");
        return -1;
    }

	LARGE_INTEGER li;
	li.QuadPart = offset;
	SetFilePointer (hFile,li.LowPart,&li.HighPart,FILE_BEGIN);

	unsigned long dwBytesWritten = 0;
	WriteFile(hFile,memaddr,count,&dwBytesWritten,NULL);

	CloseHandle(hFile);
}
void initdisk()
{
	whereisdiskinfo(&diskinfo);
	say("inited disk\n");
}