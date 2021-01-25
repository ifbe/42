#include <windows.h>
#include "libsoft.h"




u64 threadcreate(void* func, void* args)
{
	u64 id;
	HANDLE fd = CreateThread(NULL, 0, func, args, 0, (void*)&id);
	return id;
}
void threaddelete(u64 h)
{
}
int tasksearch(void* buf, int len)
{
	say("@tasksearch\n");

	return 0;
}
int taskmodify(void* buf, int len)
{
	return 0;
}




void initthread()
{
}
void freethread()
{
}