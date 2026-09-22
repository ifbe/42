#include <stdio.h>
#include <stdlib.h>
#include <string.h>




void* memory_alloc(int size)
{
	int j;
	char* buf = malloc(size);
	if(0 == buf)return 0;

	for(j=0;j<size;j++)buf[j] = 0;
	return buf;
}
void* memory_alloc_align(int size, int cmd)
{
	int j;
	char* buf = malloc(size);
	if(0 == buf)return 0;

	for(j=0;j<size;j++)buf[j] = 0;
	return buf;
}
void memory_free(void* addr)
{
	free(addr);
}
void* memory_setval(void* addr, int ch, int len)
{
	return memset(addr, ch, len);
}
void* memory_copy(void* addr, void* buf, int len)
{
	return memcpy(addr, buf, len);
}
void* memory_realloc(void* addr, int len)
{
	return realloc(addr, len);
}