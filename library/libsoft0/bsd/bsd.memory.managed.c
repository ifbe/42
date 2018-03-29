#include<stdio.h>
#include<stdlib.h>




void* memorycreate(int size)
{
	return malloc(size);
}
int memorydelete(void* addr)
{
	free(addr);
	return 0;
}