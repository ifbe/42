#include "libboot.h"
int miniuart_getc();




static struct event ev[2];
void* pollenv()
{
	int ret = miniuart_getc();
	if(ret > 0xff)return 0;

	ev->why = ret;
	ev->what = _char_;
	return ev;
}




void freerunenv()
{
}
void initrunenv()
{
}
