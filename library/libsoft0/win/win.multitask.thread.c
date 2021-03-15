#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <tlhelp32.h>
#include "libsoft.h"




int threadsearch(void* buf, int len)
{
	HANDLE snap;
	snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, _getpid());
	if(INVALID_HANDLE_VALUE == snap){
		return 1;
	}

	THREADENTRY32 th32;
	th32.dwSize = sizeof(THREADENTRY32);
	if (!Thread32First(snap, &th32)){
		CloseHandle(snap);
		return 1;
	}

	printf("Thread  procid\n");
	do{
		printf("%-8d%-8d\n",
			th32.th32ThreadID,
			th32.th32OwnerProcessID
		);
	}while(Thread32Next(snap, &th32));

	CloseHandle(snap);
	return 0;
}
int taskmodify(void* buf, int len)
{
	return 0;
}




u64 threadcreate(void* func, void* args)
{
	u64 id;
	HANDLE fd = CreateThread(NULL, 0, func, args, 0, (void*)&id);
	return id;
}
int threaddelete(u64 h)
{
	return 0;
}




void initthread()
{
}
void freethread()
{
}
