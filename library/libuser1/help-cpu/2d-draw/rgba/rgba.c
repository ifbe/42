#include "libuser.h"




void pixel_clearcolor(_obj* win)
{
	int x;
	int w = win->whdf.width;
	int h = win->whdf.height;
	int s = win->whdf.fbwidth>>2;
	int len = s*h;
	u32* buf = (u32*)(win->rgbanode.buf);

	for(x=0;x<len;x++)buf[x] = 0xff000000;
}
void pixel_cleardepth(_obj* wnd)
{
	int x,y,w,h,stride;
	float* depth = wnd->rgbanode.depthbuf;
	if(0 == depth){depth = wnd->rgbanode.depthbuf = memoryalloc(2048*2048*4, 0);}

	stride = wnd->whdf.fbwidth>>2;
	for(y=0;y<wnd->whdf.height;y++){
		for(x=0;x<wnd->whdf.width;x++)depth[y*stride+x] = 1000*1000.0;
	}
}