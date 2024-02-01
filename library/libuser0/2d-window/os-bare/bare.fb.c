#include "libuser.h"
void* supply_alloc();
void* supply_recycle(void*);
int wndmgr_take(void*,void*, void*,int, void*,int, void*,int);
int wndmgr_give(void*,void*, void*,int, void*,int, void*,int);




struct fbinfo{
	u32 buf;
	u32 pad0;

	u32 fmt;
	u32 pad1;

	u16 w;
	u16 zerow;
	u32 padd2;

	u16 h;
	u16 zeroh;
	u32 padd3;
};
static u32* screen = 0;
static u32 format = 4;
//
void* cachedwindow = 0;




void window_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//logtoall("wnd=%p,stack=%p\n",wnd,stack);
	wndmgr_take(wnd,foot, stack,sp, arg,key, buf,len);

	int j;
	u32* ibuf = wnd->rgbabuf;
	u32* obuf = screen;
	for(j=0;j<1024*768;j++)
	{
		obuf = (void*)obuf + format;
		*obuf = ibuf[j];
	}
}
void window_give(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//printmemory(buf, 0x20);
	wndmgr_give(wnd,0, stack,sp, 0,0, buf,len);
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
void window_delete(_obj* w)
{
}
void window_create(_obj* wnd)
{
	wnd->vfmt = _bgra8888_;

	wnd->width = 1024;
	wnd->height = 768;

	wnd->fbwidth = 1024*4;
	//wnd->fbheight = 0;

	wnd->rgbabuf = (void*)0x4000000;
}




void* window_alloc()
{
	if(cachedwindow)return cachedwindow;
	return supply_alloc();
}




void initwindow()
{
	cachedwindow = 0;

#define screeninfo 0x2000
	struct fbinfo* fb = (struct fbinfo*)0x2000;

	screen = (u32*)(u64)(fb->buf);
	format = fb->fmt;
}
void freewindow()
{
}
