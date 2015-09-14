#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char




void say(char* fmt,...);




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