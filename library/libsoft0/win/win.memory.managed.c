#include<stdio.h>
#include<stdlib.h>




void* startmemory(int size)
{
        return malloc(size);
}
int stopmemory(void* addr)
{
        free(addr);
	return 0;
}
void creatememory()
{
}
void deletememory()
{
}
