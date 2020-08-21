#include "libuser.h"
int rgbanode_read(void*,int, void*,int, void*,int, void*,int);
int rgbanode_write(void*,int, void*,int, void*,int, void*,int);




struct fbinfo{
	u32 buf;
	u32 pad0;

	u32 fmt;
	u32 pad1;

	u16 w;
	u16 zerow;
	u32 padd2;

	u16 h;
	u16 zeroh;
	u32 padd3;
};
static u32* screen = 0;
static u32 format = 4;




void windowread(struct supply* wnd,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("wnd=%p,stack=%p\n",wnd,stack);
	rgbanode_read(wnd,foot, stack,sp, arg,key, buf,len);

	int j;
	u32* ibuf = wnd->rgbabuf;
	u32* obuf = screen;
	for(j=0;j<1024*768;j++)
	{
		obuf = (void*)obuf + format;
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
	struct fbinfo* fb = (struct fbinfo*)0x2000;

	screen = (u32*)(u64)(fb->buf);
	format = fb->fmt;
}
void freewindow()
{
}
