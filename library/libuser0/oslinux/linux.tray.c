#include "libuser.h"
int lowlevel_input();




void terminalthread(void* win)
{
	while(1)
	{
		eventwrite(lowlevel_input(), _char_, 0, 0);
	}
}
void traycreate(struct arena* win)
{
	threadcreate(terminalthread, 0);
}




void inittray()
{
}
void freetray()
{
}
