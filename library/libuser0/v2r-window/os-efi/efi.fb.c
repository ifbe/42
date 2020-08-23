#include "libuser.h"
#define _bgra8880_ hex64('b', 'g', 'r', 'a', '8', '8', '8', '0')
#define _rgba8880_ hex64('r', 'g', 'b', 'a', '8', '8', '8', '0')
#define _bgra8888_ hex64('b', 'g', 'r', 'a', '8', '8', '8', '8')
#define _rgba8888_ hex64('r', 'g', 'b', 'a', '8', '8', '8', '8')
void getscreen(void** _buf, u64* _fmt, int* _w, int* _h, int* _fbw, int* _fbh);
//
int rgbanode_read(void*,int, void*,int, void*,int, void*,int);
int rgbanode_write(void*,int, void*,int, void*,int, void*,int);




static void* lfb = 0;
static u64 fmt;
static int w = 0;
static int h = 0;
static int fbw = 0;
static int fbh = 0;




void windowread(struct supply* wnd,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == lfb)return;
	rgbanode_read(wnd,foot, stack,sp, arg,key, buf,len);

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

	int j;
	u32* ibuf = wnd->rgbabuf;
	u32* obuf = lfb;
	for(j=0;j<w*h-1;j++)
	{
		*obuf = ibuf[j];
		obuf = (void*)obuf + bpp;
	}
}
void windowwrite(struct supply* wnd,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	//printmemory(buf, 32);
	rgbanode_write(wnd,0, stack,sp, 0,0, buf,len);
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
	say("lfb=%p,fmt=%.8s, w=%d,h=%d, fbw=%d,fbh=%d\n", lfb,&fmt, w,h, fbw,fbh);

	//wnd data
	wnd->fmt = _rgba_;
	wnd->vfmt = fmt;

	wnd->width = w;
	wnd->height = h;

	wnd->fbwidth = w*4;
	//wnd->fbheight = 0;

	wnd->rgbabuf = (void*)0x2000000;
}




void initwindow()
{
}
void freewindow()
{
}
