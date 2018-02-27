#include "actor.h"
static u16 symbol[16] = 
{
	'A','1',   '2','3',
	'4','5',   '6','7',
	'8','9',0x3031,'J',
	'Q','K',   '!'
};



static void poker_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	int j;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);

	for(j=0;j<108/4;j++)
	{
		drawsolid_rect(
			win, 0xc0c0c0,
			cx + (j-13)*ww/27 - (ww/4),
			cy - (j/2) + (hh/2),
			cx + (j-13)*ww/27 + (ww/4),
			cy - (j/2) + hh
		);
		drawline_rect(
			win, 0x404040,
			cx + (j-13)*ww/27 - (ww/4),
			cy - (j/2) + (hh/2),
			cx + (j-13)*ww/27 + (ww/4),
			cy - (j/2) + hh
		);
		drawascii(
			win, 0xffffff,
			cx + (j-13)*ww/27 - (ww/4),
			cy - (j/2) + (hh/2),
			symbol[j/2]
		);
	}
}
static void poker_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	int j;
	float cx = sty->i_cx;
	float cy = sty->i_cy;
	float cz = sty->i_cz;
	float ww = sty->i_rx;
	float hh = sty->i_fy;
	float dd = sty->i_uz;
	carvesolid_rect(
		win, 0x404040,
		cx, cy, cz,
		ww, 0.0, 0.0,
		0.0, hh, 0.0
	);

	for(j=0;j<13;j++)
	{
		carvesolid_rect(
			win, 0xc08040,
			(cx-ww/2)+(j*ww/16), cy-(hh*(48+j)/64), ww/16,
			ww/24, 0.0, 0.0,
			0.0, 0.0, ww/16
		);
	}
}
static void poker_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void poker_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void poker_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void poker_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)poker_read_cli(win, sty, act, com);
	else if(fmt == _tui_)poker_read_tui(win, sty, act, com);
	else if(fmt == _html_)poker_read_html(win, sty, act, com);
	else if(fmt == _vbo_)poker_read_vbo(win, sty, act, com);
	else poker_read_pixel(win, sty, act, com);
}
static void poker_write(
	struct actor* act, struct compo* com,
	struct event* ev)
{
}
static void poker_list()
{
}
static void poker_choose()
{
}
static void poker_stop()
{
}
static void poker_start()
{
}
static void poker_delete()
{
}
static void poker_create()
{
}




void poker_register(struct actor* p)
{
	p->type = hex32('g', 'a', 'm', 'e');
	p->name = hex64('p', 'o', 'k', 'e', 'r', 0, 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)poker_create;
	p->ondelete = (void*)poker_delete;
	p->onstart  = (void*)poker_start;
	p->onstop   = (void*)poker_stop;
	p->onlist   = (void*)poker_list;
	p->onchoose = (void*)poker_choose;
	p->onread   = (void*)poker_read;
	p->onwrite  = (void*)poker_write;
}