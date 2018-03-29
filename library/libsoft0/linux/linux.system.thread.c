#include <pthread.h>
#define u64 unsigned long long




u64 threadcreate(void* func, void* args)
{
	unsigned long long id;
	pthread_create((void*)&id, NULL, func, args);
	return id;
}
void threaddelete(u64 h)
{
}