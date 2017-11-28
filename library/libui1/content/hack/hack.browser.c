#include "actor.h"
int netmgr_read();
int netmgr_write(void*);




static struct actor* pl;
static int count = 0;
//
static u64 fd = 0;
static u8* srcbuf = 0;
static u8* dstbuf = 0;
static int len = 0;




static void browser_read_html(struct arena* win, struct actor* act, struct style* sty)
{
	int j;
	u8* p = (u8*)(win->buf);
	for(j=0;j<len;j++)p[j] = dstbuf[j];
}
static void browser_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	drawstring(win, 0xffffff, 0, 0, pl->priv, 0);
	drawstring(win, 0x0000ff, 0, 16, dstbuf, 0);
}
static void browser_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void browser_read_tui(struct arena* win, struct actor* act, struct style* sty)
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
static void browser_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("browser(%x,%x,%x)\n",win,act,sty);
}
static void browser_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == __cli__)browser_read_cli(win, act, sty);
	else if(fmt == __tui__)browser_read_tui(win, act, sty);
	else if(fmt == __html__)browser_read_html(win, act, sty);
	else if(fmt == __vbo__)browser_read_vbo(win, act, sty);
	else browser_read_pixel(win, act, sty);
}
static void browser_write(struct event* ev)
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




static void browser_list()
{
}
static void browser_change()
{
}
static void browser_start()
{
}
static void browser_stop()
{
}
void browser_create(void* base,void* addr)
{
	srcbuf = base-0x100000;
	dstbuf = base+0x300000;
	pl = addr;

	pl->type = hex32('h', 'a', 'c', 'k');
	pl->name = hex64('b', 'r', 'o', 'w', 's', 'e', 'r', 0);

	pl->start = (void*)browser_start;
	pl->stop = (void*)browser_stop;
	pl->list = (void*)browser_list;
	pl->choose = (void*)browser_change;
	pl->read = (void*)browser_read;
	pl->write = (void*)browser_write;
}
void browser_delete()
{
}
