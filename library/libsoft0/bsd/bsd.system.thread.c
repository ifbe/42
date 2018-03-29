#include <pthread.h>




unsigned long long threadcreate(void* func, void* args)
{
	unsigned long long id;
	pthread_create((void*)&id, NULL, func, args);
	return id;
}
void threaddelete(u64 h)
{
}