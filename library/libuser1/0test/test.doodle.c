#include "actor.h"



//
static int x1=-99,y1=10;
static int x2=25,y2=-77;
static int px=77,py=88;




void doodle_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	int w = win->w;
	int h = win->h;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;

	//rect
	drawsolid_rect(win, 0x00ff00, 
		cx-90, cy-90, cx-10, cy-10);
	drawline_rect(win, 0xff00ff,
		cx+50, cy-30, cx+60, cy-50);

	//circle
	drawsolid_circle(win, 0x0000ff,
		cx-50, cy+40, 40);
	drawline_circle(win, 0xff0000,
		cx+70, cy+80, 40);

	//bezier
	drawline(win, 0x00ffff,
		cx+x1, cy+y1, px, py);
	drawline(win, 0x00ffff,
		cx+x2, cy+y2, px, py);
	drawline_bezier(win, 0xffff,
		cx+x1, cy+y1, cx+x2, cy+y2, px, py);
}
static void doodle_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;

	carvesolid_icosahedron(
		win, 0xffffff,
		cx, cy, 0.0,
		ww/2, 0.0, 0.0,
		0.0, ww/2, 0.0,
		0.0, 0.0, ww/2
	);
}
static void doodle_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void doodle_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void doodle_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	say("doodle(%x,%x,%x)\n", win, sty, act, com);
}
static void doodle_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)doodle_read_cli(win, sty, act, com);
	else if(fmt == _tui_)doodle_read_tui(win, sty, act, com);
	else if(fmt == _html_)doodle_read_html(win, sty, act, com);
	else if(fmt == _vbo_)doodle_read_vbo(win, sty, act, com);
	else doodle_read_pixel(win, sty, act, com);
}
static void doodle_write(
	struct actor* act, struct compo* com,
	struct event* ev)
{
	u64 what = ev->what;
	u64 why = ev->why;
	if(what == 0x2d70)
	{
		px = why & 0xffff;
		py = (why >> 16) & 0xffff;
	}
}
static void doodle_list()
{
}
static void doodle_change()
{
}
static void doodle_stop()
{
}
static void doodle_start()
{
}
static void doodle_delete()
{
}
static void doodle_create()
{
}




void doodle_register(struct actor* p)
{
	p->type = hex32('t', 'e', 's', 't');
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