#include "libuser.h"




void pixel_clearcolor(struct supply* win)
{
	int x;
	int w = win->width;
	int h = win->height;
	int s = win->fbwidth>>2;
	int len = s*h;
	u32* buf = (u32*)(win->rgbabuf);

	for(x=0;x<len;x++)buf[x] = 0xff000000;
}
void pixel_cleardepth(struct supply* wnd)
{
	int x,y,w,h,stride;
	float* depth = wnd->depthbuf;
	if(0 == depth){depth = wnd->depthbuf = memorycreate(2048*2048*4, 0);}

	stride = wnd->fbwidth>>2;
	for(y=0;y<wnd->height;y++){
		for(x=0;x<wnd->width;x++)depth[y*stride+x] = 1000.0;
	}
}