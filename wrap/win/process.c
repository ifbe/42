#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>


static BYTE processinfo[0x80*0x20];


__attribute__((constructor)) void initprocess()
{
	//ֻ���г��������ֺͱ��
	PROCESSENTRY32 proc;
	HANDLE pHandle;
	proc.dwSize = sizeof(PROCESSENTRY32);
	pHandle = CreateToolhelp32Snapshot(0x2,0x0);
	if(pHandle==INVALID_HANDLE_VALUE){
		return;
	}

	//-----------
	printf("%-40s%-20s\n","proc.szExeFile","proc.th32ProcessID");
	int i=0;
	while( Process32Next(pHandle,&proc) )
	{
		printf("%-40s%d\n",proc.szExeFile,proc.th32ProcessID);
		sprintf(processinfo+i,"%-40s%d\n",proc.szExeFile,proc.th32ProcessID);
		i+=0x80;
		if(i>0x80*0x20)break;
	}
	CloseHandle(pHandle);
	return;
}
//__attribute__((destructor)) void freeprocess()




void whereisprocessinfo(unsigned long long* p)
{
	*p=(unsigned long long)processinfo;
}