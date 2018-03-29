#include<stdio.h>
#include<stdlib.h>




void* memorycreate(int size)
{
	return malloc(size);
}
void memorydelete(void* addr)
{
	free(addr);
}