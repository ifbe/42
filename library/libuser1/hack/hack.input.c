#include "actor.h"




static void joystick_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	int radius = hh/8;

	//outer
	drawline_rect(win, 0xffffff, cx-ww, cy-hh, cx+ww, cy+hh);

	//select, start
	drawline_rect(win, 0xffffff,
		cx-(ww/8), cy+(hh*11/16),
		cx, cy+(hh*13/16)
	);
	drawline_rect(win, 0xffffff,
		cx, cy+(hh*11/16),
		cx+(ww/8), cy+(hh*13/16)
	);

	//left, up, down, right
	drawsolid_rect(win, 0x0000ff,
		cx-(ww*5/8)-radius, cy-(radius/2),
		cx-(ww*5/8)+radius, cy+(radius/2)
	);
	drawsolid_rect(win, 0xff0000,
		cx-(ww*4/8)-(radius/2), cy-(hh/4)-radius,
		cx-(ww*4/8)+(radius/2), cy-(hh/4)+radius
	);
	drawsolid_rect(win, 0x00ffff,
		cx-(ww*4/8)-(radius/2), cy+(hh/4)-radius,
		cx-(ww*4/8)+(radius/2), cy+(hh/4)+radius
	);
	drawsolid_rect(win, 0xffff00,
		cx-(ww*3/8)-radius, cy-(radius/2),
		cx-(ww*3/8)+radius, cy+(radius/2)
	);

	//y, x, b, a
	drawsolid_circle(win, 0xff00ff,
		cx+(ww*3/8), cy, radius);
	drawsolid_circle(win, 0xfedcba,
		cx+(ww*4/8), cy-(hh/4), radius);
	drawsolid_circle(win, 0xabcdef,
		cx+(ww*4/8), cy+(hh/4), radius);
	drawsolid_circle(win, 0xffff00,
		cx+(ww*5/8), cy, radius);
}
static void joystick_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void joystick_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void joystick_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void joystick_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("joystick(%x,%x,%x)\n",win,act,sty);
}




void keyboard_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y,c,l;
	int cx = sty->cx;
	int cy = sty->cy;
	int ww = sty->rx;
	int hh = sty->fy;

	for(y=0;y<6;y++)
	{
		for(x=0;x<16;x++)
		{
			drawicon_1(
				win, 0x40ffffff,
				(cx-ww+1)+(2*x*ww/16),
				(cy-hh+1)+(2*y*hh/6),
				(cx-ww-1)+((2*x+2)*ww/16),
				(cy-hh-1)+((2*y+2)*hh/6),
				(void*)".", l
			);
		}
	}
}
static void keyboard_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void keyboard_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void keyboard_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void keyboard_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("keyboard(%x,%x,%x)\n",win,act,sty);
}
static void input_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)keyboard_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)keyboard_read_tui(win, sty, act, pin);
	else if(fmt == _html_)keyboard_read_html(win, sty, act, pin);
	else if(fmt == _vbo_)keyboard_read_vbo(win, sty, act, pin);
	else keyboard_read_pixel(win, sty, act, pin);
}
static void input_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int x,y;
	//say("%x,%x\n",x,y);
}
static void input_list()
{
}
static void input_change()
{
}
static void input_stop(struct actor* act, struct pinid* pin)
{
}
static void input_start(struct actor* act, struct pinid* pin)
{
}
static void input_delete(struct actor* act)
{
	if(0 == act)return;
}
static void input_create(struct actor* act)
{
	if(0 == act)return;
}




void input_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('i', 'n', 'p', 'u', 't', 0, 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)input_create;
	p->ondelete = (void*)input_delete;
	p->onstart  = (void*)input_start;
	p->onstop   = (void*)input_stop;
	p->onlist   = (void*)input_list;
	p->onchoose = (void*)input_change;
	p->onread   = (void*)input_read;
	p->onwrite  = (void*)input_write;
}
