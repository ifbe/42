#include "libuser.h"
void* supply_alloc();
void* supply_recycle(void*);




void window_attach()
{
}
void window_detach()
{
}
void window_take()
{
}
void window_give()
{
}




void window_read()
{
}
void window_write()
{
}
void window_delete(_obj* w)
{
}
void window_create(_obj* w)
{
	w->fmt = _cli_;
	w->vfmt = 0;

	w->width = 80;
	w->height = 25;

	w->fbwidth = 80;
	//w->fbheight = 0;
}




void initwindow()
{
}
void freewindow()
{
}
