#include "libuser.h"
int lowlevel_input();




void terminalthread(struct arena* win)
{
	int ret;
	while(1)
	{
		ret = lowlevel_input();
		if(0 == win->orel0){
			eventwrite(ret, _char_, 0, 0);
		}
		else {
			//say("%x\n", ret);
			relationwrite(win, _dst_, &ret, 1);
		}
	}
}
void stdcreate(struct arena* win)
{
	threadcreate(terminalthread, win);
}




void initstd()
{
}
void freestd()
{
}