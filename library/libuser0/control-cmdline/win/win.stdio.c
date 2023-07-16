#include "libuser.h"
int lowlevel_input();
int termwrite(void* buf, int len);




void terminalthread(_obj* win)
{
	int ret;
	struct event ev;
	struct halfrel stack[0x80];
	while(1)
	{
		ret = lowlevel_input();
		if( (0 == win->orel0) && (0 == win->irel0) ){
			eventwrite(ret, _char_, 0, 0);
		}
		else {
			//say("%x\n", ret);
			ev.why = ret;
			ev.what = _char_;
			give_data_into_peer(win,_dst_, stack,0, 0,0, &ev,0);
		}
	}
}




void stdio_take(_obj* sup,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf, int len)
{
}
void stdio_give(_obj* sup,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf, int len)
{
	say("%.*s", len, buf);
}




void stdio_delete(_obj* win)
{
}
void stdio_create(_obj* win)
{
	termwrite("\n", 1);
	threadcreate(terminalthread, win);
}




void freestd()
{
}
void initstd()
{
}
