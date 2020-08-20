#include "libuser.h"
int rgbanode_read(void*,int, void*,int, void*,int, void*,int);
int rgbanode_write(void*,int, void*,int, void*,int, void*,int);




static u32* screen = 0;




void windowread(struct supply* wnd,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("wnd=%p,stack=%p\n",wnd,stack);
	rgbanode_read(wnd,foot, stack,sp, arg,key, buf,len);

	int j;
	u32* ibuf = wnd->rgbabuf;
	u32* obuf = screen;
	for(j=0;j<1024*768;j++)
	{
		obuf = (void*)obuf + 3;
		*obuf = ibuf[j];
	}
}
void windowwrite(struct supply* wnd,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	//printmemory(buf, 0x20);
	rgbanode_write(wnd,0, stack,sp, 0,0, buf,len);
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
void windowcreate(struct supply* wnd)
{
	wnd->fmt = _rgba_;
	wnd->vfmt = hex64('b', 'g', 'r', 'a', '8', '8', '8', '8');

	wnd->width = 1024;
	wnd->height = 768;

	wnd->fbwidth = 1024*4;
	//wnd->fbheight = 0;

	wnd->rgbabuf = (void*)0x2000000;
}




void initwindow()
{
#define screeninfo 0x2000
	u64 memory = *(u32*)screeninfo;
	screen = (u32*)memory;
}
void freewindow()
{
}
