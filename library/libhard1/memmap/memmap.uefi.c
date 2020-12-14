#include "libhard.h"




void initmemmap(void* p)
{
	if(0 == p)return;
	printmemory(p, 0x100);
}