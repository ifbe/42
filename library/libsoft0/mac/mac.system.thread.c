#include<unistd.h>
#include<pthread.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




u64 startthread(void* func, void* args)
{
	unsigned long long id;
	pthread_create((void*)&id, NULL, func, args);
	return id;
}
void stopthread()
{
}
