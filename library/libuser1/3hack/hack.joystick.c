#include "actor.h"




static void joystick_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int ww = (win->w) * (sty->wantw) / 0x20000;
	int hh = (win->h) * (sty->wanth) / 0x20000;
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
static void joystick_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void joystick_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void joystick_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void joystick_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("joystick(%x,%x,%x)\n",win,act,sty);
}
static void joystick_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == __cli__)joystick_read_cli(win, act, sty);
	else if(fmt == __tui__)joystick_read_tui(win, act, sty);
	else if(fmt == __html__)joystick_read_html(win, act, sty);
	else if(fmt == __vbo__)joystick_read_vbo(win, act, sty);
	else joystick_read_pixel(win, act, sty);
}
static void joystick_write(u64* who, u64* what, u64* how)
{
}
static void joystick_list()
{
}
static void joystick_change()
{
}
static void joystick_start()
{
}
static void joystick_stop()
{
}
void joystick_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('h', 'a', 'c', 'k');
	p->name = hex64('j', 'o', 'y', 's', 't', 'i', 'c', 'k');

	p->start = (void*)joystick_start;
	p->stop = (void*)joystick_stop;
	p->list = (void*)joystick_list;
	p->choose = (void*)joystick_change;
	p->read = (void*)joystick_read;
	p->write = (void*)joystick_write;
}
void joystick_delete()
{
}
