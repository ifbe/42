#include "actor.h"




static int turn;
static u8 data[9];




static void ooxx_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy+hh);

	//heng
	drawline(win, 0xffffff, cx-ww, cy-hh/3, cx+ww, cy-hh/3);
	drawline(win, 0xffffff, cx-ww, cy+hh/3, cx+ww, cy+hh/3);

	//shu
	drawline(win, 0xffffff, cx-ww/3, cy-hh, cx-ww/3, cy+hh);
	drawline(win, 0xffffff, cx+ww/3, cy-hh, cx+ww/3, cy+hh);

	//ox
	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			if(data[3*y + x] == 'o')
			{
				drawline_circle(win, 0xff,
					cx+(x-1)*ww*2/3, cy+(y-1)*hh*2/3, ww/6
				);
			}
			else if(data[3*y + x] == 'x')
			{
				drawline(win, 0xff0000,
					cx+(4*x-5)*ww/6, cy+(4*y-5)*hh/6,
					cx+(4*x-3)*ww/6, cy+(4*y-3)*hh/6
				);
				drawline(win, 0xff0000,
					cx+(4*x-5)*ww/6, cy+(4*y-3)*hh/6,
					cx+(4*x-3)*ww/6, cy+(4*y-5)*hh/6
				);
			}
		}//forx
	}//fory
}
static void ooxx_read_vbo(
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
	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			carvesolid_sphere(
				win, 0xffffff,
				(cx-ww)+(2*x+1)*ww/3, (cy-hh)+(2*y+1)*hh/3, 0.0,
				ww/3, 0.0, 0.0,
				0.0, 0.0, ww/3
			);
		}
	}
}
static void ooxx_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void ooxx_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"ooxx\" style=\"width:100%%;height:100px;background-color:#111111;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");
	win->len = len;
}
static void ooxx_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void ooxx_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u8 ch;
	int x,y;
	say("ooxx(%x,%x,%x)\n",win,act,sty);

	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			ch = data[y*3 + x];
			if((ch!='o') && (ch!='x'))say("_	");
			else say("%c	",ch);
		}
		say("\n");
	}
}
static void ooxx_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)ooxx_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)ooxx_read_tui(win, sty, act, pin);
	else if(fmt == _html_)ooxx_read_html(win, sty, act, pin);
	else if(fmt == _json_)ooxx_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)ooxx_read_vbo(win, sty, act, pin);
	else ooxx_read_pixel(win, sty, act, pin);
}
void ooxx_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	char val;
	int x,y;
/*
	int width = haha->width;
	int height = haha->height;
	int min = (width<height) ? width:height;
*/
	int min = 512;
	if(ev->what == 0x2d70)
	{
		x=(ev->why) & 0xffff;
		if(x>min)return;

		y=( (ev->why) >> 16 ) & 0xffff;
		if(y>min)return;

		x = x*3/min;
		y = y*3/min;
say("%d,%d\n",x,y);
		if(data[y*3 + x] != 0)return;

		if((turn&0x1) == 0x1)val='o';
		else val='x';

		data[y*3 + x] = val;
		turn++;
	}
}
static void ooxx_list()
{
}
static void ooxx_choose()
{
}
static void ooxx_stop(struct actor* act, struct pinid* pin)
{
}
static void ooxx_start(struct actor* act, struct pinid* pin)
{
	int j;

	turn=0;
	for(j=0;j<9;j++)data[j]=0;
}
static void ooxx_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void ooxx_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = data;
	if(_copy_ == act->type)act->buf = memorycreate(16);
}




void ooxx_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex32('o', 'o', 'x', 'x');
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)ooxx_create;
	p->ondelete = (void*)ooxx_delete;
	p->onstart  = (void*)ooxx_start;
	p->onstop   = (void*)ooxx_stop;
	p->onlist   = (void*)ooxx_list;
	p->onchoose = (void*)ooxx_choose;
	p->onread   = (void*)ooxx_read;
	p->onwrite  = (void*)ooxx_write;
}
