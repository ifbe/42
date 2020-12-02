#include "libuser.h"
int input(void* buf, int len);




void window_take(struct supply* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
}
void window_give(struct supply* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct event* ev = buf;
	if(_kbd_ == ev->what){
		if(0x1b == ev->why)eventwrite(0,0,0,0);
	}
	if(_char_ == ev->what)input(buf, 1);
}
void windowlist()
{
}
void windowchoose()
{
}
void windowstop()
{
}
void windowstart()
{
}
void windowdelete(struct supply* wnd)
{
}
void windowcreate(struct supply* wnd)
{
	wnd->fmt = _cli_;
	wnd->vfmt = 0;

	wnd->width = 80;
	wnd->height = 25;

	wnd->fbwidth = 80;
	wnd->fbheight = 0;
}




void initwindow()
{
}
void freewindow()
{
}
