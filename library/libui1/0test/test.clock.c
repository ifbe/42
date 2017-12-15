#include "actor.h"
u64 getdate();




static void clock_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void clock_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	u32 c[7]={0xff,0xff00,0xffff,0xff0000,0xff00ff,0xffff00,0xffffff};
	int j;
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int w = (win->w) * (sty->wantw) / 0x20000;
	int h = (win->h) * (sty->wanth) / 0x20000;
	u64 date = getdate();
	u8* p = (u8*)&date;
	for(j=6;j>=0;j--)
	{
		drawdecimal(win, c[j], cx+64-(j*24), cy-8, p[j]);
	}
}
static void clock_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void clock_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void clock_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	u64 date = getdate();
	u8* p = (u8*)&date;

	say(	"_%02d_%02d_%02d_%02d_%02d_%02d_%02d\n",
		p[6],p[5],p[4],p[3],p[2],p[1],p[0]);
}
static void clock_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == __cli__)clock_read_cli(win, act, sty);
	else if(fmt == __tui__)clock_read_tui(win, act, sty);
	else if(fmt == __html__)clock_read_html(win, act, sty);
	else if(fmt == __vbo__)clock_read_vbo(win, act, sty);
	else clock_read_pixel(win, act, sty);
}
static void clock_write(struct event* ev)
{
}




static void clock_list()
{
}
static void clock_change()
{
}
static void clock_start()
{
}
static void clock_stop()
{
}
void clock_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'e', 's', 't');
	p->name = hex64('c', 'l', 'o', 'c', 'k', 0, 0, 0);

	p->start = (void*)clock_start;
	p->stop = (void*)clock_stop;
	p->list = (void*)clock_list;
	p->choose = (void*)clock_change;
	p->read = (void*)clock_read;
	p->write = (void*)clock_write;
}
void clock_delete()
{
}
