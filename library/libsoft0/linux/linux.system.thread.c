#include <pthread.h>
unsigned long long startthread(void* func, void* args)
{
	unsigned long long id;
	pthread_create((void*)&id, NULL, func, args);
	return id;
}
void stopthread()
{
}
