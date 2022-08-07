#include "libuser.h"
int input(void* buf, int len);
void* supply_alloc();
void* supply_recycle(void*);




void window_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
}
void window_give(_obj* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct event* ev = buf;
	if(_kbd_ == ev->what){
		if(0x1b == ev->why)eventwrite(0,0,0,0);
	}
	if(_char_ == ev->what)input(buf, 1);
}
void window_attach()
{
}
void window_detach()
{
}




void window_read()
{
}
void window_write()
{
}
void window_delete(_obj* wnd)
{
}
void window_create(_obj* wnd)
{
	wnd->fmt = _cli_;
	wnd->vfmt = 0;

	wnd->width = 80;
	wnd->height = 25;

	wnd->fbwidth = 80;
	wnd->fbheight = 0;
}




void* window_alloc()
{
	return supply_alloc();
}




void initwindow()
{
}
void freewindow()
{
}
