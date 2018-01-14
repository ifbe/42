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
static void starry_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void starry_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void starry_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void starry_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("starry(%x,%x,%x)\n",win,act,sty);
}
static void starry_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == __cli__)starry_read_cli(win, act, sty);
	else if(fmt == __tui__)starry_read_tui(win, act, sty);
	else if(fmt == __html__)starry_read_html(win, act, sty);
	else if(fmt == __vbo__)starry_read_vbo(win, act, sty);
	else starry_read_pixel(win, act, sty);
}
static void starry_write(struct event* ev)
{
}




static void starry_list()
{
}
static void starry_into()
{
}
static void starry_start()
{
}
static void starry_stop()
{
}
void starry_create(void* base,void* addr)
{
	struct actor* act = addr;
	//node = (struct starry*)(base+0x300000);

	act->type = hex32('h', 'a', 'c', 'k');
	act->name = hex64('s', 't', 'a', 'r', 'r', 'y', 0, 0);

	act->start = (void*)starry_start;
	act->stop = (void*)starry_stop;
	act->list = (void*)starry_list;
	act->choose = (void*)starry_into;
	act->read = (void*)starry_read;
	act->write = (void*)starry_write;
}
void starry_delete()
{
}
