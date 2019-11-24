#include "libuser.h"




void windowread()
{
}
void windowwrite(struct supply* src)
{
	int x,y;
	u8* ibuf = src->buf;
	u8* obuf = (void*)0xa0000;
	for(x=0;x<320*200;x++)obuf[x] = ibuf[x];
}
void windowlist()
{
}
void windowchoose()
{
}
void windowstop()
{
}
void windowstart()
{
}
void windowdelete(struct supply* win)
{
}
void windowcreate(struct supply* win)
{
	win->type = hex32('w','i','n',0);
	win->fmt = hex32('8','b','i','t');

	win->width = win->stride = 320;
	win->height = 200;

	win->len = 320*200;
	win->buf = (void*)(u64)0x2000000;
}




void initwindow()
{
}
void freewindow()
{
}
