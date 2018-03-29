#include<windows.h>
#define u64 unsigned long long




u64 threadcreate(void* func, void* args)
{
	u64 id;
	HANDLE fd = CreateThread(NULL, 0, func, args, 0, (void*)&id);
	return id;
}
void threaddelete(u64 h)
{
}