#include "libuser.h"
#define _bgra8880_ hex64('b', 'g', 'r', 'a', '8', '8', '8', '0')
#define _rgba8880_ hex64('r', 'g', 'b', 'a', '8', '8', '8', '0')
#define _bgra8888_ hex64('b', 'g', 'r', 'a', '8', '8', '8', '8')
#define _rgba8888_ hex64('r', 'g', 'b', 'a', '8', '8', '8', '8')
void stdout_setwindow(void* node);
void getscreen(void** _buf, u64* _fmt, int* _w, int* _h, int* _fbw, int* _fbh);
//
void* supply_alloc();
void* supply_recycle(void*);
//
int wndmgr_take(void*,void*, void*,int, p64,int, void*,int);
int wndmgr_give(void*,void*, void*,int, p64,int, void*,int);




//
void* cachedwindow = 0;
//
static void* lfb = 0;
static u64 fmt;
static int w = 0;
static int h = 0;
static int fbw = 0;
static int fbh = 0;
void window_update(_obj* wnd,void* test, int x0,int y0, int xn,int yn)
{
	if(0 == lfb)return;
	if(x0 < 0)x0 = 0;
	if(y0 < 0)y0 = 0;
	if(xn > w)xn = w;
	if(yn > h)yn = h;
//say("%d,%d,%d,%d\n",x0,y0,xn,yn);
	int bpp;
	switch(wnd->vfmt){
	case _bgra8888_:
	case _rgba8888_:
		bpp = 4;
		break;
	case _bgra8880_:
	case _rgba8880_:
		bpp = 3;
		break;
	default:
		return;
	}

	int x,y;
	u32* ibuf = wnd->rgbanode.buf;
	u32* obuf = lfb;
	for(y=y0;y<yn;y++){
		obuf = lfb + y*fbw + x0*bpp;
		for(x=x0;x<xn;x++){
			*obuf = ibuf[y*w + x];
			obuf = (void*)obuf + bpp;

			if((x == w-1) && (y == h-1))break;
		}
	}
}




void window_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == lfb)return;
	u64 t0 = timeread_us();

	wndmgr_take(wnd,foot, stack,sp, arg,key, buf,len);

	u64 t1 = timeread_us();
	wnd->whdf.iw0 = t1 - t0;

	//10fps? update whole window
	window_update(wnd,0, 0,0, w,h);

	u64 t2 = timeread_us();
	wnd->whdf.iwn = t2 - t1;
}
void window_give(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(foot){
		//update area
		int w = wnd->whdf.width;
		int h = wnd->whdf.height + (key<<4);
		drawsolid_rect((void*)wnd,0, 0,h, w,h+16);
		drawstring((void*)wnd,0xff00ff, 0,h, buf,len);
		window_update(wnd,0, 0,h, w,h+16);
	}
	else{
		//printmemory(buf, 32);
		wndmgr_give(wnd,0, stack,sp, 0,0, buf,len);

		//only update mouse area
		int x = wnd->whdf.ix0;
		int y = wnd->whdf.iy0;
		//say("x=%d,y=%d\n",x,y);
		window_update(wnd,0, x-16,y-16, x+16,y+16);
	}
}
void window_detach()
{
}
void window_attach()
{
}




void window_read(_obj* wnd)
{
}
void window_write(_obj* wnd)
{
}
void window_delete(_obj* wnd)
{
}
void window_create(_obj* wnd)
{
	getscreen(&lfb, &fmt, &w, &h, &fbw, &fbh);
	say("lfb=%p,fmt=%.8s, w=%d,h=%d, fbw=0x%x,fbh=0x%x\n", lfb,&fmt, w,h, fbw,fbh);

	//wnd data
	wnd->hfmt = _rgba_;
	wnd->vfmt = fmt;

	wnd->whdf.width = w;
	wnd->whdf.height = h;

	wnd->whdf.fbwidth = w*4;
	//wnd->fbheight = 0;

	wnd->rgbanode.buf = (void*)0x4000000;

	//early console
	stdout_setwindow(wnd);

	cachedwindow = wnd;
}




void* window_alloc()
{
	if(cachedwindow)return cachedwindow;
	return supply_alloc();
}




void initwindow()
{
	cachedwindow = 0;
}
void freewindow()
{
}
