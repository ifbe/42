#include "libuser.h"
int lowlevel_input();
int termwrite(void* buf, int len);




void terminalthread(struct supply* win)
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
			relationwrite(win,_dst_, stack,0, 0,0, &ev,0);
		}
	}
}




void stdio_read(_sup* sup,int foot, _syn* stack,int sp, void* arg,int idx, void* buf, int len)
{
}
void stdio_write(_sup* sup,int foot, _syn* stack,int sp, void* arg,int idx, void* buf, int len)
{
	say("%.*s", len, buf);
}
void stdio_delete(struct supply* win)
{
}
void stdio_create(struct supply* win)
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
