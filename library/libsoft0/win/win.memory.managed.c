#include<stdio.h>
#include<stdlib.h>




void* startmemory(int size)
{
	return malloc(size);
}
void stopmemory(void* addr)
{
	free(addr);
}
void creatememory()
{
}
void deletememory()
{
}
