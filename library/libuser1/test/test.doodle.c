#include "actor.h"
double arctan2(double,double);
double cosine(double);
double sine(double);



static int px=0;
static int py=0;
static u8 buffer[16];




void doodle_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float c,s,f;
	int x0,y0,x1,y1;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;

	//circle
	drawsolid_circle(win, 0x00ffff, cx, cy, ww);
	drawsolid_circle(win, 0x404040, cx+ww/2, cy, ww/2);
	drawsolid_circle(win, 0x404040, cx-ww/2, cy, ww/2);

	f = arctan2(py-cy, px-cx+(ww/2));
	c = cosine(f);
	s = sine(f);
	x0 = (int)(c*ww/4) + (cx-ww/2);
	y0 = (int)(s*ww/4) + cy;

	f = arctan2(py-cy, px-cx-(ww/2));
	c = cosine(f);
	s = sine(f);
	x1 = (int)(c*ww/4) + (cx+ww/2);
	y1 = (int)(s*ww/4) + cy;

	drawsolid_circle(win, 0xff0000, x0, y0, ww/4);
	drawsolid_circle(win, 0xff0000, x1, y1, ww/4);

	drawline(win, 0xffffff, x0, y0, px, py);
	drawline(win, 0xffffff, x1, y1, px, py);

	drawbezier(win, 0xffffff, x0, y0, x1, y1, px, py);
}
static void doodle_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float f;
	float x0,y0,x1,y1;
	float cx = sty->cx;
	float cy = sty->cy;
	float cz = sty->cz;
	float ww = sty->rx;
	float hh = sty->fy;
	float dd = sty->uz;

	carvesolid_circle(
		win, 0x00ffff,
		cx, cy, ww/128,
		ww, 0.0, 0.0,
		0.0, hh, 0.0
	);
	carvesolid_circle(
		win, 0x404040,
		cx-ww/2, cy, ww/64,
		ww/2, 0.0, 0.0,
		0.0, hh/2, 0.0
	);
	carvesolid_circle(
		win, 0x404040,
		cx+ww/2, cy, ww/64,
		ww/2, 0.0, 0.0,
		0.0, hh/2, 0.0
	);

	f = arctan2(py-cy, px-cx+(ww/2));
	x0 = (cosine(f)*ww*1/4) + (cx-ww/2);
	y0 = (sine(f)*ww*1/4) + (cy+y0);
	f = arctan2(py-cy, px-cx-(ww/2));
	x1 = (cosine(f)*ww*1/4) + (cx+ww/2);
	y1 = (sine(f)*ww*1/4) + (cy+y1);

	carvesolid_circle(
		win, 0xff0000,
		x0, y0, ww/32,
		ww/4, 0.0, 0.0,
		0.0, hh/4, 0.0
	);
	carvesolid_circle(
		win, 0xff0000,
		x1, y1, ww/32,
		ww/4, 0.0, 0.0,
		0.0, hh/4, 0.0
	);

	carveline(
		win, 0xffffff,
		x0, y0, ww/32,
		px, py, ww/32
	);
	carveline(
		win, 0xffffff,
		x1, y1, ww/32,
		px, py, ww/32
	);
}
static void doodle_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void doodle_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"doodle\" style=\"width:50%%;height:100px;float:left;background-color:#28a779;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void doodle_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void doodle_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("doodle(%x,%x,%x)\n", win, sty, act, pin);
}
static void doodle_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)doodle_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)doodle_read_tui(win, sty, act, pin);
	else if(fmt == _html_)doodle_read_html(win, sty, act, pin);
	else if(fmt == _json_)doodle_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)doodle_read_vbo(win, sty, act, pin);
	else doodle_read_pixel(win, sty, act, pin);
}
static void doodle_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	u64 what = ev->what;
	u64 why = ev->why;
	if(0x4070 == what)
	{
		px = why&0xffff;
		py = (why>>16)&0xffff;
	}
}
static void doodle_list()
{
}
static void doodle_change()
{
}
static void doodle_stop(struct actor* act, struct pinid* pin)
{
}
static void doodle_start(struct actor* act, struct pinid* pin)
{
}
static void doodle_delete(struct actor* act)
{
}
static void doodle_create(struct actor* act)
{
	if(0 == act)return;
	act->buf = ((void*)act) + 0x100;
}




void doodle_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('d', 'o', 'o', 'd', 'l', 'e', 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)doodle_create;
	p->ondelete = (void*)doodle_delete;
	p->onstart  = (void*)doodle_start;
	p->onstop   = (void*)doodle_stop;
	p->onlist   = (void*)doodle_list;
	p->onchoose = (void*)doodle_change;
	p->onread   = (void*)doodle_read;
	p->onwrite  = (void*)doodle_write;
}
