#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
void say(char* fmt,...);




HANDLE hDev;
char* memoryinfo;




void initprocess()
{
}
void freeprocess()
{
}




void listprocess()
{
	//只需列出进程名字和编号
	HANDLE temp;
	temp = CreateToolhelp32Snapshot(0x2,0x0);
	if(temp==INVALID_HANDLE_VALUE){
		return;
	}

	//
	PROCESSENTRY32 proc;
	proc.dwSize = sizeof(PROCESSENTRY32);

	//找位置，跟在后面
	char* p=(char*)memoryinfo;
	int i=0;
	while(1)
	{
		if( p[i] == 0 )break;
		else i+=0x100;
	}

	//写内容
	printf("%-40s%-20s\n","proc.szExeFile","proc.th32ProcessID");
	while( Process32Next(temp,&proc) )
	{
		printf("%-40s%d\n",proc.szExeFile,proc.th32ProcessID);
		sprintf((char*)memoryinfo+i,"%-40s%d\n",proc.szExeFile,proc.th32ProcessID);
		i+=0x100;
		if(i>0x100*100)break;
	}
	CloseHandle(temp);
	return;
}




void chooseprocess(u64 pid)
{
}




u64 readprocess(u64 buf,u64 startaddr,u64 disk,u32 count)
{
	u64 bytesread = 0;
	ReadProcessMemory(hDev,(char*)(startaddr*512),(char*)buf,count*0x200,&bytesread);
	return bytesread;
}




void writeprocess()
{
}








void createprocess(char* addr)
{
	memoryinfo=addr;
}
void deleteprocess()
{
}
