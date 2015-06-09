#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>

#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char

//每0x40字节存放分区的基本信息
struct diskinfo
{
	unsigned char path[0x40];
	unsigned char name[0x40];
	unsigned char sn[0x40];
	unsigned char unused[0x40];
};
static struct diskinfo diskinfo[100];

HANDLE hDev;
static BYTE tempname[20]={'\\','\\','.','\\','P','h','y','s','i','c','a','l','D','r','i','v','e','0','\0','\0'};




static void enumeratedisk()
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
			/*
			unsigned long long returncount;
			DeviceIoControl(temphandle,			//HANDLE hDevice, 
							IOCTL_XXX,			//DWORD dwIoControlCode, 
							NULL,				//LPVOID lpInBuffer, 
							0,					//DWORD nInBufferSize, 
							diskinfo[i].sn,		//LPVOID lpOutBuffer, 
							0x40,				//DWORD nOutBufferSize, 
							&returncount,			//LPDWORD lpBytesReturned, 
							0);			//LPOVERLAPPED lpOverlapped
			*/

			//
			CloseHandle(temphandle);
			//printf("%d    \\\\.\\PHYSICALDRIVE%d\n",i,i);

			//打开成功的，path才会不等于0
			int j=0;
			for(;j<20;j++)
			{
				diskinfo[i].path[j]=tempname[j];
			}
		}
		else
		{
			//printf("physicaldrive%d,GetLastError()=:%d\n",i,GetLastError());
		}
	}//10个记录
}
void enumerateprocess()
{
	//只需列出进程名字和编号
	PROCESSENTRY32 proc;
	HANDLE pHandle;
	proc.dwSize = sizeof(PROCESSENTRY32);
	pHandle = CreateToolhelp32Snapshot(0x2,0x0);
	if(pHandle==INVALID_HANDLE_VALUE){
		return;
	}

	//找位置，跟在后面
	char* p=(char*)diskinfo;
	int i=0;
	while(1)
	{
		if( p[i] == 0 )break;
		else i+=0x100;
	}

	//写内容
	printf("%-40s%-20s\n","proc.szExeFile","proc.th32ProcessID");
	while( Process32Next(pHandle,&proc) )
	{
		printf("%-40s%d\n",proc.szExeFile,proc.th32ProcessID);
		sprintf((char*)diskinfo+i,"%-40s%d\n",proc.szExeFile,proc.th32ProcessID);
		i+=0x100;
		if(i>0x100*100)break;
	}
	CloseHandle(pHandle);
	return;
}
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
__attribute__((constructor)) void initdisk()
{
	//列出
	listall();

	//打开找到的第一个硬盘并且选择它
	int i;
	for(i=0;i<10;i++)
	{
		if(diskinfo[i].path[0]==0)continue;
		hDev=CreateFile((BYTE*)diskinfo+i*0x100,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
		if(hDev == INVALID_HANDLE_VALUE)
		{
			printf("can't open first disk\n");
		}
		else
		{
			memcpy((BYTE*)diskinfo+0xf00,(BYTE*)diskinfo+i*0x100,0x100);
		}
		break;
	}
}
__attribute__((destructor)) void freedisk()
{
	CloseHandle(hDev);
}
















void whereisdiskinfo(unsigned long long* p)
{
	*p=(unsigned long long)diskinfo;
}
void focus(QWORD in)
{
	//第1种可能：传进来的是个文件的地址
	if( in >100 )
	{
		//比如d:\image\name.img
		char* path=(char*)in;
		say("here2:%s\n",path);

		return;
	}




	//第2种可能：传进来的是个硬盘
	char* path=diskinfo[in].path;
	if(*(DWORD*)path == 0x5c2e5c5c)		//选择的是硬盘
	{
		//测试能否成功打开
		HANDLE temphandle=CreateFile(path,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
		if(temphandle == INVALID_HANDLE_VALUE)
		{
			say("cannot open\n");
			return;
		}
		else CloseHandle(temphandle);

		//关掉原先已经打开的磁盘，然后把这个虚拟磁盘文件当成硬盘来用并且选定
		CloseHandle(hDev);
		hDev=CreateFile(path,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);

		return;
	}




	//第3种可能：传进来的是某个进程的虚拟内存
}
//内存地址，第一扇区，请无视，总字节数
void readmemory(QWORD buf,QWORD startsector,QWORD disk,DWORD count)
{
	LARGE_INTEGER li;
	li.QuadPart = startsector*512;
	SetFilePointer (hDev,li.LowPart,&li.HighPart,FILE_BEGIN);

	unsigned long dwret = 0;
	ReadFile(hDev,(unsigned char*)buf,count*512,&dwret,0);
	if(dwret!=count*512)printf("read %d bytes,GetLastError()=%d\n",dwret,GetLastError());
/*
	int i,j;
	for(i=0;i<dwRet;i+=16)
	{
		QWORD skip=*(QWORD*)(buf+i);
		if(skip==0)continue;

		printf("[%4x,%4x]",i,i+15);

		for(j=0;j<16;j++)
		{
			BYTE temp=*(BYTE*)(buf+i+j);
			printf("%4x",temp);
		}
		printf("\n",i,i+15);
	}
*/
	//return dwRet;
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