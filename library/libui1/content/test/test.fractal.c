#include "actor.h"
void drawrect(
	void* win,
	int x1, int y1,
	int x2, int y2,
	u32 bc, u32 fc);




static void fractal_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void fractal_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int cx, cy, w, h;
	cx = (win->w) * (sty->cx) / 0x10000;
	cy = (win->h) * (sty->cy) / 0x10000;
	w = (win->w) * (sty->wantw) / 0x10000;
	h = (win->h) * (sty->wanth) / 0x10000;

	drawrect(win, cx-w/2, cy-h/2, cx+w/2, cy+h/2, 0xff0000, 0xff00);
}
static void fractal_read_text(struct arena* win, struct actor* act, struct style* sty)
{
}
static void fractal_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		fractal_read_text(win, act, sty);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		fractal_read_html(win, act, sty);
	}

	//pixel
	else
	{
		fractal_read_pixel(win, act, sty);
	}
}
static void fractal_write(struct event* ev)
{
}




static void fractal_list()
{
}
static void fractal_change()
{
}
static void fractal_start()
{
}
static void fractal_stop()
{
}
void fractal_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'e', 's', 't');
	p->name = hex64('f', 'r', 'a', 'c', 't', 'a', 'l', 0);

	p->start = (void*)fractal_start;
	p->stop = (void*)fractal_stop;
	p->list = (void*)fractal_list;
	p->choose = (void*)fractal_change;
	p->read = (void*)fractal_read;
	p->write = (void*)fractal_write;
}
void fractal_delete()
{
}