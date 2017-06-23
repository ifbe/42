#include "actor.h"
void drawhexadecimal(
	void*, u64 in, int size,
	int x, int y, u32 fg, u32 bg);




static int flag=0;




static void pure_read_pixel(struct arena* win)
{
	int x,y,w,h;
	u32 color;
	u32* buf = (u32*)(win->buf);
	w = win->w;
	h = win->h;

	color=0xff000000;
	if((flag&0x1) == 0x1)color |= 0xff;
	if((flag&0x2) == 0x2)color |= 0xff00;
	if((flag&0x4) == 0x4)color |= 0xff0000;

	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			buf[w*y + x] = color;
		}
	}

	drawhexadecimal(
		win, color & 0xffffff, 4,
		0, 0, 0x87654321, 0xfedcba98);
}
static void pure_read_html(struct arena* win)
{
	u32* buf = (u32*)(win->buf);
	pure_read_pixel(win);
	buf[0]=0;
}
static void pure_read_text(struct arena* win)
{
}
static void pure_read(struct arena* win)
{
	u64 fmt = win->fmt;
	//say("(@2048.read)temp=%x\n",temp);

	//text
	if(fmt == 0x74786574)
	{
		pure_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		pure_read_html(win);
	}

	//pixel
	else
	{
		pure_read_pixel(win);
	}
}
static void pure_write(struct event* ev)
{
	u64 what = ev->what;
	if(what==0x2d70 | what==0x64626b)
	{
		flag = (flag+1)&0x7;
	}
}




static void pure_list()
{
}
static void pure_into()
{
}
static void pure_start()
{
}
static void pure_stop()
{
}
void pure_create(void* uibuf,void* addr)
{
	struct actor* p = addr;
	p->type = hexof('t','e','s','t',0,0,0,0);
	p->name = hexof('p','u','r','e',0,0,0,0);

	p->start = (void*)pure_start;
	p->stop = (void*)pure_stop;
	p->list = (void*)pure_list;
	p->choose = (void*)pure_into;
	p->read = (void*)pure_read;
	p->write = (void*)pure_write;
}
void pure_delete()
{
}

