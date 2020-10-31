#include "libuser.h"
#define _bgra8880_ hex64('b', 'g', 'r', 'a', '8', '8', '8', '0')
#define _rgba8880_ hex64('r', 'g', 'b', 'a', '8', '8', '8', '0')
#define _bgra8888_ hex64('b', 'g', 'r', 'a', '8', '8', '8', '8')
#define _rgba8888_ hex64('r', 'g', 'b', 'a', '8', '8', '8', '8')
void getscreen(void** _buf, u64* _fmt, int* _w, int* _h, int* _fbw, int* _fbh);
//
int wndmgr_take(void*,void*, void*,int, void*,int, void*,int);
int wndmgr_give(void*,void*, void*,int, void*,int, void*,int);




static void* lfb = 0;
static u64 fmt;
static int w = 0;
static int h = 0;
static int fbw = 0;
static int fbh = 0;




void window_take(struct supply* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == lfb)return;
	wndmgr_take(wnd,foot, stack,sp, arg,key, buf,len);

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
	u32* ibuf = wnd->rgbabuf;
	u32* obuf = lfb;
	for(y=0;y<h;y++){
		obuf = lfb + y*fbw;
		for(x=0;x<w;x++){
			*obuf = ibuf[y*w + x];
			obuf = (void*)obuf + bpp;

			if((x == w-1) && (y == h-1))break;
		}
	}
}
void window_give(struct supply* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	//printmemory(buf, 32);
	wndmgr_give(wnd,0, stack,sp, 0,0, buf,len);
}
void windowlist()
{
}
void windowchoose()
{
}
void windowstart()
{
}
void windowstop()
{
}
void windowdelete(struct supply* wnd)
{
}
void windowcreate(struct supply* wnd)
{
	getscreen(&lfb, &fmt, &w, &h, &fbw, &fbh);
	say("lfb=%p,fmt=%.8s, w=%d,h=%d, fbw=0x%x,fbh=0x%x\n", lfb,&fmt, w,h, fbw,fbh);

	//wnd data
	wnd->fmt = _rgba_;
	wnd->vfmt = fmt;

	wnd->width = w;
	wnd->height = h;

	wnd->fbwidth = w*4;
	//wnd->fbheight = 0;

	wnd->rgbabuf = (void*)0x4000000;
}




void initwindow()
{
}
void freewindow()
{
}
