#include "libuser.h"
void* supply_alloc();
void* supply_recycle(void*);
int wndmgr_take(void*,void*, void*,int, void*,int, void*,int);
int wndmgr_give(void*,void*, void*,int, void*,int, void*,int);




static u32* screen = 0;
static u32 format = 4;




void window_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
void window_give(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
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
void window_delete(_obj* w)
{
}
void window_create(_obj* wnd)
{
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
