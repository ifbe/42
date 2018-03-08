#include "actor.h"
#define width 32
#define height 32
u32 getrandom();
static u8 data[height][width];




static void circuit_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 c;
	int x,y;
	int xxx,yyy;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			drawline_rect(
				win, 0x404040,
				cx-ww + (x+x+0)*ww/width,
				cy-hh + (y+y+0)*hh/height,
				cx-ww + (x+x+2)*ww/width,
				cy-hh + (y+y+2)*hh/height
			);

			if(1 == data[y][x])c = 0xffffff;
			else if(2 == data[y][x])c = 0xffff00;
			else if(4 == data[y][x])c = 0xff00ff;
			else if(8 == data[y][x])c = 0xff0000;
			else if(16 == data[y][x])c = 0xffff;
			else if(32 == data[y][x])c = 0xff00;
			else if(64 == data[y][x])c = 0xff;
			else continue;

			drawsolid_rect(
				win, c,
				cx-ww + (x+x+0)*ww/width,
				cy-hh + (y+y+0)*hh/height,
				cx-ww + (x+x+2)*ww/width,
				cy-hh + (y+y+2)*hh/height
			);
		}
	}
}
static void circuit_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 c;
	int x,y;
	float xxx, yyy;
	float cx = sty->cx;
	float cy = sty->cy;
	float cz = sty->cz;
	float ww = sty->rx;
	float hh = sty->fy;
	float dd = sty->uz;

	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			xxx = cx-ww + (x+x+1)*ww/width;
			yyy = cy-hh + (y+y+1)*hh/height;
			carveline_rect(
				win, 0x404040,
				xxx, yyy, 0.0,
				ww/width, 0.0, 0.0,
				0.0, hh/height, 0.0
			);

			if(1 == data[y][x])c = 0xffffff;
			else if(2 == data[y][x])c = 0xffff00;
			else if(4 == data[y][x])c = 0xff00ff;
			else if(8 == data[y][x])c = 0xff0000;
			else if(16 == data[y][x])c = 0xffff;
			else if(32 == data[y][x])c = 0xff00;
			else if(64 == data[y][x])c = 0xff;
			else continue;

			carvesolid_prism4(
				win, c,
				xxx, yyy, ww/width/2,
				ww/width, 0.0, 0.0,
				0.0, hh/height, 0.0,
				0.0, 0.0, ww/width/2
			);
		}
	}
}
static void circuit_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void circuit_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void circuit_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("circuit(%x,%x,%x)\n",win,act,sty);
}
static void circuit_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)circuit_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)circuit_read_tui(win, sty, act, pin);
	else if(fmt == _html_)circuit_read_html(win, sty, act, pin);
	else if(fmt == _vbo_)circuit_read_vbo(win, sty, act, pin);
	else circuit_read_pixel(win, sty, act, pin);
}
static void circuit_write(
	struct actor* act, struct pinid* pin,
	struct event* ev)
{
}
static void circuit_list()
{
}
static void circuit_change()
{
}
static void circuit_stop(struct actor* act, struct pinid* pin)
{
}
static void circuit_start(struct actor* act, struct pinid* pin)
{
	int x,y;
	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			data[y][x] = 0;
		}
	}

	for(x=0;x<7;x++)
	{
		data[getrandom()%height][getrandom()%width] = 1<<x;
		data[getrandom()%height][getrandom()%width] = 1<<x;
	}
}
static void circuit_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)stopmemory(act->buf);
}
static void circuit_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = data;
	if(_copy_ == act->type)act->buf = startmemory(width*height);
}




void circuit_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('c', 'i', 'r', 'c', 'u', 'i', 't', 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)circuit_create;
	p->ondelete = (void*)circuit_delete;
	p->onstart  = (void*)circuit_start;
	p->onstop   = (void*)circuit_stop;
	p->onlist   = (void*)circuit_list;
	p->onchoose = (void*)circuit_change;
	p->onread   = (void*)circuit_read;
	p->onwrite  = (void*)circuit_write;
}
