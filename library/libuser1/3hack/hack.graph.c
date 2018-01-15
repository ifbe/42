#include "actor.h"
#define PI 3.141592653
void* samepinnextchip(void*);




static void starry_read_pixel_r(struct arena* win, struct arena* haha,
	int cx, int cy, int ww, int hh, float sa, float da)
{
	int x,y,j,k;
	u32 color;
	float ff;
	struct relation* rel;
	struct relation* tmp;
	struct actor* act;

	k = 0;
	rel = haha->irel;
	while(1)
	{
		if(rel == 0)break;
		rel = samepinnextchip(rel);
		k++;
	}
	//say("k=%x\n",k);

	j = 0;
	k *= 2;
	rel = haha->irel;
	while(1)
	{
		if(rel == 0)break;
//say("%x,%llx\n",rel,rel->selftype);

		ff = (da-sa) / k;
		x = cx + ww * cosine(sa + (2*j+1) * ff);
		y = cy + hh * sine(sa + (2*j+1) * ff);

		if(rel->selftype == hex32('w','i','n',0))
		{
			color = 0x0000ff;
		}
		else
		{
			color = 0x00ff00;
		}

		act = (void*)(rel->selfchip);
		drawline(
			win, color,
			cx, cy,
			x, y
		);
		drawicon_1(
			win, color,
			x - 32, y - 12,
			x + 32, y + 12,
			(void*)(&act->name), 8
		);

		if(color == 0x0000ff)
		{
			starry_read_pixel_r(
				win, (void*)act,
				x, y,
				ww/2, hh/2,
				sa+(2*j)*ff, sa+(2*j+2)*ff
			);
		}

		tmp = samepinnextchip(rel);
		if(tmp == 0)break;

		j += 1;
		rel = tmp;
	}
}
static void starry_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int ww = (win->w) * (sty->wantw) / 0x20000;
	int hh = (win->h) * (sty->wanth) / 0x20000;

	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy+hh);

	drawicon_1(
		win, 0xff,
		cx-32, cy-12,
		cx+32, cy+12,
		(void*)"root", 0
	);
	starry_read_pixel_r(
		win, win,
		cx, cy,
		ww/2, hh/2,
		-PI, PI
	);
}




static void graph_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void graph_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int cx = (sty->cx) * (win->w) / 0x10000;
	int cy = (sty->cy) * (win->h) / 0x10000;
	int ww = (sty->wantw) * (win->w) / 0x20000;
	int hh = (sty->wanth) * (win->h) / 0x20000;
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);
	//starry_read_pixel(win, act, sty);
}
static void graph_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void graph_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void graph_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
}
static void graph_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == __cli__)graph_read_cli(win, act, sty);
	else if(fmt == __tui__)graph_read_tui(win, act, sty);
	else if(fmt == __html__)graph_read_html(win, act, sty);
	else if(fmt == __vbo__)graph_read_vbo(win, act, sty);
	else graph_read_pixel(win, act, sty);
}
static void graph_write(struct event* ev)
{
}




static void graph_list()
{
}
static void graph_change()
{
}
static void graph_start()
{
}
static void graph_stop()
{
}
void graph_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('h', 'a', 'c', 'k');
	p->name = hex64('g', 'r', 'a', 'p', 'h', 0, 0, 0);

	p->start = (void*)graph_start;
	p->stop = (void*)graph_stop;
	p->list = (void*)graph_list;
	p->choose = (void*)graph_change;
	p->read = (void*)graph_read;
	p->write = (void*)graph_write;
}
void graph_delete()
{
}
