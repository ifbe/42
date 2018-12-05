#include "libuser.h"
static u32* screen = 0;




void windowread()
{
}
void windowwrite(struct arena* win)
{
	int j;
	u32* ibuf = win->buf;
	u32* obuf = screen;
	for(j=0;j<1024*768;j++)
	{
		obuf = (void*)obuf + 3;
		*obuf = ibuf[j];
	}
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
void windowdelete(struct arena* w)
{
}
void windowcreate(struct arena* w)
{
	w->type = hex32('w','i','n',0);
	w->fmt = hex64('b','g','r','a','8','8','8','8');

	w->width = w->stride = 1024;
	w->height = 768;

	w->buf = (void*)0x2000000;
}




void initwindow()
{
#define screeninfo 0x2000
	screen = *(u32*)screeninfo;
}
void freewindow()
{
}
