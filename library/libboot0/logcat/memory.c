#include <stdio.h>
#include <stdlib.h>
#include <string.h>




void* memorycreate(int size)
{
	return malloc(size);
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