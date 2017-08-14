#include "actor.h"
void rect(void*,
	int x0, int y0,
	int x1, int y1,
	u32 bc, u32 fc);




void navi_read(struct arena* win)
{
	int w = win->w;
	int h = win->h;
	rect(win, 0, h*15/16, w/4, h, 0x000000, 0xff0000);
	rect(win, w/4, h*15/16, w/2, h, 0x0000ff, 0xff00ff);
	rect(win, w/2, h*15/16, w*3/4, h, 0x00ff00, 0xffff00);
	rect(win, w*3/4, h*15/16, w, h, 0x00ffff, 0xffffff);
}
void navi_write()
{
}
void navi_create()
{
}
void navi_delete()
{
}
