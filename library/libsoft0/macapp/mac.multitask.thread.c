#include <pthread.h>
#include "libsoft.h"




u64 threadcreate(void* func, void* args)
{
	unsigned long long id;
	pthread_create((void*)&id, NULL, func, args);
	return id;
}
int threaddelete(u64 h)
{
	return 0;
}
int threadsearch(void* buf, int len)
{
	say("@threadsearch\n");

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
