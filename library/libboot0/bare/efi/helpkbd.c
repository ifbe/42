#include "libboot.h"
void* read8042(void*);




static struct event ev[1];
void* pollenv()
{
	return read8042(ev);
}