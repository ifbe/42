#include "libhard.h"




void parsememmap_uefi(void* p)
{
	if(0 == p)return;
	printmemory(p, 0x100);

	say("\n\n");
}