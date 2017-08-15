#include "actor.h"
void drawstring(
	void*, void*, int size,
	int x, int y, u32 fg, u32 bg);
void backgroundcolor(
	void*, u32);
int netmgr_read();
int netmgr_write(void*);




static struct actor* pl;
static int count = 0;
//
static u64 fd = 0;
static u8* srcbuf = 0;
static u8* dstbuf = 0;
static int len = 0;




static void browse_read_text(struct arena* win)
{
	u8* p = (u8*)(win->buf);
	int w = win->w;
	int h = win->h;
	int x,y;

	//
	for(x=0;x<w*h*4;x++)p[x] = 0;
	for(x=0;x<w;x++)p[x<<2] = pl->priv[x];

	//
	y = w;
	for(x=0;x<len;x++)
	{
		if(dstbuf[x] == 0xa)y += w-(y%w);
		else
		{
			p[y<<2] = dstbuf[x];
			y++;
			if(y>w*h)break;
		}
	}
}
static void browse_read_html(struct arena* win)
{
	int j;
	u8* p = (u8*)(win->buf);
	for(j=0;j<len;j++)p[j] = dstbuf[j];
}
static void browse_read_pixel(struct arena* win)
{
	backgroundcolor(win, 0);
	drawstring(
		win, pl->priv, 1,
		0, 0, 0xffffffff, 0);
	drawstring(
		win, dstbuf, 1,
		0, 16, 0xffffffff, 0);
}
static void browse_read(struct arena* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		browse_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		browse_read_html(win);
	}

	//pixel
	else
	{
		browse_read_pixel(win);
	}
}
static void browse_write(struct event* ev)
{
#define kbd 0x72616863
#define http 0x70747468
	u64 type = ev->what;
	u64 key = ev->why;
	if(type == kbd)
	{
		if(key == 0xd)
		{
			netmgr_write(pl->priv);
			for(;count>=0;count--)pl->priv[count] = 0;
			count = 0;
		}
		else if(key == 0x8)
		{
			if(count>0)count--;
			pl->priv[count] = 0;
		}
		else
		{
			pl->priv[count] = key&0xff;
			if(count<0xbf)count++;
		}
	}
	else if(type == http)
	{
		int src=0,dst=0;
		fd = ev->where;

		for(src=0;src<key;src++)
		{
			if(srcbuf[src] == 0xd)continue;

			dstbuf[dst] = srcbuf[src];
			dst++;
		}
		len = dst;
	}
}




static void browse_list()
{
}
static void browse_change()
{
}
static void browse_start()
{
}
static void browse_stop()
{
}
void browse_create(void* base,void* addr)
{
	srcbuf = base-0x100000;
	dstbuf = base+0x300000;
	pl = addr;

	pl->type = hex32('h', 'a', 'c', 'k');
	pl->name = hex64('b', 'r', 'o', 'w', 's', 'e', 0, 0);

	pl->start = (void*)browse_start;
	pl->stop = (void*)browse_stop;
	pl->list = (void*)browse_list;
	pl->choose = (void*)browse_change;
	pl->read = (void*)browse_read;
	pl->write = (void*)browse_write;
}
void browse_delete()
{
}
