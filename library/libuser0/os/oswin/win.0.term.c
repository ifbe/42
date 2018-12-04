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
			nodetree_rootwrite(win, 0, &ret, 1);
		}
	}
}
void termcreate(struct arena* win)
{
	threadcreate(joystickthread, win);
	threadcreate(terminalthread, win);
}




void initterm()
{
}
void freeterm()
{
}