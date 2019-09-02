#include <stdio.h>
#include <stdlib.h>
#include <string.h>




void* memorycreate(int size, int cmd)
{
	int j;
	char* buf = malloc(size);
	if(0 == buf)return 0;

	for(j=0;j<size;j++)buf[j] = 0;
	return buf;
}
void memorydelete(void* addr)
{
	free(addr);
}
void* memorysetup(void* addr, int ch, int len)
{
	return memset(addr, ch, len);
}
void* memorycopy(void* addr, void* buf, int len)
{
	return memcpy(addr, buf, len);
}