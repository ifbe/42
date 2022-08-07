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
void window_write(_obj* src)
{
	int x,y;
	u8* ibuf = src->buf;
	u8* obuf = (void*)0xa0000;
	for(x=0;x<320*200;x++)obuf[x] = ibuf[x];
}
void window_delete(_obj* win)
{
}
void window_create(_obj* win)
{
	win->fmt = hex32('8','b','i','t');
	win->vfmt = 0;

	win->width = 320;
	win->height = 200;

	win->fbwidth = 320;
	//win->fbheight = ;

	win->len = 320*200;
	win->buf = (void*)(u64)0x2000000;
}




void initwindow()
{
}
void freewindow()
{
}
