#include "actor.h"
static u8 data[7][7];




static void pegged_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x, y, cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->cx;
		cy = sty->cy;
		ww = sty->rx;
		hh = sty->fy;
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}

	for(y=0;y<7;y++)
	{
		for(x=0;x<7;x++)
		{
			if(data[y][x] == 0)continue;

			drawline_rect(
				win, 0x808080,
				cx+(2*x-7)*ww/7,
				cy+(2*y-7)*hh/7,
				cx+(2*x-5)*ww/7,
				cy+(2*y-5)*hh/7
			);
			drawsolid_circle(
				win, 0xffffff,
				cx+(x-3)*ww*2/7,
				cy+(y-3)*hh*2/7,
				ww/14
			);
		}
	}
}
static void pegged_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	float cx = sty->cx;
	float cy = sty->cy;
	float cz = sty->cz;
	float ww = sty->rx;
	float hh = sty->fy;
	float dd = sty->uz;

	for(y=0;y<7;y++)
	{
		for(x=0;x<7;x++)
		{
			if(data[y][x] == 0)continue;

			carveline_rect(
				win, 0x808080,
				cx+(2*x-6)*ww/7, cy+(2*y-6)*hh/7, 0.0,
				ww/7, 0.0, 0.0,
				0.0, hh/7, 0.0
			);
			carvesolid_sphere(
				win, 0xffffff,
				cx+(x-3)*ww*2/7, cy+(y-3)*hh*2/7, 0.0,
				ww/14, 0.0, 0.0,
				0.0, ww/14, 0.0,
				0.0, 0.0, ww/14
			);
		}
	}
}
static void pegged_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void pegged_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;

	//<head>
	htmlprintf(win, 1,
		".pegbg{width:50%%;height:50%%;float:left;background-color:#000;text-align:center;}\n"
		".pegfg{width:14%%;height:14%%;float:left;background-color:#ccc;margin:0.1%%;}\n"
	);

	//<body>
	htmlprintf(win, 2, "<div class=\"pegbg\">\n");
	for(y=0;y<7;y++)
	{
		for(x=0;x<7;x++)
		{
			htmlprintf(win, 2,
				"<div class=\"pegfg\">%d</div>\n",
				data[y][x]
			);
		}//forx
	}//fory
	htmlprintf(win, 2, "</div>\n");
}
static void pegged_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void pegged_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void pegged_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)pegged_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)pegged_read_tui(win, sty, act, pin);
	else if(fmt == _html_)pegged_read_html(win, sty, act, pin);
	else if(fmt == _json_)pegged_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)pegged_read_vbo(win, sty, act, pin);
	else pegged_read_pixel(win, sty, act, pin);
}
static void pegged_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void pegged_list()
{
}
static void pegged_change()
{
}
static void pegged_stop(struct actor* act, struct pinid* pin)
{
}
static void pegged_start(struct actor* act, struct pinid* pin)
{
	int x,y;
	for(y=0;y<7;y++)
	{
		for(x=0;x<7;x++)
		{
			if((x>=2)&&(x<=4))data[y][x] = 1;
			else if((y>=2)&&(y<=4))data[y][x] = 1;
			else data[y][x] = 0;
		}
	}
}
static void pegged_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void pegged_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = data;
	if(_copy_ == act->type)act->buf = memorycreate(49);
}




void pegged_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('p', 'e', 'g', 'g', 'e', 'd', 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)pegged_create;
	p->ondelete = (void*)pegged_delete;
	p->onstart  = (void*)pegged_start;
	p->onstop   = (void*)pegged_stop;
	p->onlist   = (void*)pegged_list;
	p->onchoose = (void*)pegged_change;
	p->onread   = (void*)pegged_read;
	p->onwrite  = (void*)pegged_write;
}
