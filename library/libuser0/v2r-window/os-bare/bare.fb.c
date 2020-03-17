#include "libuser.h"
static u32* screen = 0;




void windowread()
{
}
void windowwrite(struct supply* win)
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
void windowdelete(struct supply* w)
{
}
void windowcreate(struct supply* w)
{
	w->fmt = hex64('b','g','r','a','8','8','8','8');
	w->vfmt = 0;

	w->width = 1024;
	w->height = 768;

	w->fbwidth = 1024*4;
	//w->fbheight = 0;

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
