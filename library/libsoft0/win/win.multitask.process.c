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




int processsearch()
{
	HANDLE snap;
	snap = CreateToolhelp32Snapshot(0x2,0x0);
	if(INVALID_HANDLE_VALUE == snap){
		return 1;
	}

	//
	PROCESSENTRY32 proc;
	proc.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(snap, &proc)){
		CloseHandle(snap);
		return 1;
	}

	//写内容
	printf("procid  parent  cntthr  name\n");
	do{
		printf("%-8d%-8d%-8d%-40s\n",
			proc.th32ProcessID,
			proc.th32ParentProcessID,
			proc.cntThreads,
			proc.szExeFile
		);
	}while( Process32Next(snap, &proc) );

	CloseHandle(snap);
	return 0;
}




void processmodify(u64 pid)
{
}




u64 processread(u64 buf,u64 startaddr,u64 disk,u32 count)
{
	u64 bytesread = 0;
	ReadProcessMemory(hDev,(char*)(startaddr*512),(char*)buf,count*0x200,&bytesread);
	return bytesread;
}




void processwrite()
{
}








void processcreate(void* file, void* args)
{
/*
BOOL CreateProcess(
LPCTSTR lpApplicationName,
LPTSTR lpCommandLine,
LPSECURITY_ATTRIBUTES lpProcessAttributes,
LPSECURITY_ATTRIBUTES lpThreadAttributes,
BOOL bInheritHandles,
DWORD dwCreationFlags,
LPVOID lpEnvironment,
LPCTSTR lpCurrentDirectory,
LPSTARTUPINFO lpStartupInfo,
LPPROCESS_INFORMATIONlpProcessInformation
);
*/
}
void processdelete()
{
}
