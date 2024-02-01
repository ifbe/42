#include "libuser.h"
void* supply_alloc();
void* supply_recycle(void*);




void window_take(_obj* wnd)
{
}
void window_give(_obj* wnd)
{
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
	wnd->vfmt = _cli_;

	wnd->whdf.width = 80;
	wnd->whdf.height = 25;

	wnd->whdf.fbwidth = 80;
	//wnd->whdf.fbheight = 0;
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
