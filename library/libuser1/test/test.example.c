#include "actor.h"
u32 getrandom();




static void example_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 bg,fg;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;

	bg = getrandom()&0xffffff;
	fg = (~bg)&0xffffff;
	drawsolid_rect(win, bg, cx-ww, cy-hh, cx+ww, cy+hh);
	drawhexadecimal(win, fg, cx, cy, bg);
}
static void example_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float cx = sty->cx;
	float cy = sty->cy;
	float cz = sty->cz;
	float rx = sty->rx;
	float ry = sty->ry;
	float rz = sty->rz;
	float fx = sty->fx;
	float fy = sty->fy;
	float fz = sty->fz;
	float ux = sty->ux;
	float uy = sty->uy;
	float uz = sty->uz;

	carvepoint_sphere(
		win, 0xffffff,
		cx-rx/2-fx/2+ux/2, cy-ry/2-fy/2+uy/2, cz-rz/2-fz/2+uz/2,
		rx/2, ry/2, rz/2,
		fx/2, fy/2, fz/2,
		ux/2, uy/2, uz/2
	);
	carveline_sphere(
		win, 0xffffff,
		cx+rx/2-fx/2+ux/2, cy+ry/2-fy/2+uy/2, cz+rz/2-fz/2+uz/2,
		rx/2, ry/2, rz/2,
		fx/2, fy/2, fz/2,
		ux/2, uy/2, uz/2
	);
	carvesolid_sphere(
		win, 0xff00ff,
		cx-rx/2+fx/2+ux/2, cy-ry/2+fy/2+uy/2, cz-rz/2+fz/2+uz/2,
		rx/2, ry/2, rz/2,
		fx/2, fy/2, fz/2,
		ux/2, uy/2, uz/2
	);

	carvepoint_sphere(
		win, 0xffffff,
		cx-rx/2-fx/2+ux*3/2, cy-ry/2-fy/2+uy*3/2, cz-rz/2-fz/2+uz*3/2,
		rx/2, ry/2, rz/2,
		fx/2, fy/2, fz/2,
		ux/2, uy/2, uz/2
	);
	carveline_sphere(
		win, 0xffffff,
		cx+rx/2-fx/2+ux*3/2, cy+ry/2-fy/2+uy*3/2, cz+rz/2-fz/2+uz*3/2,
		rx/2, ry/2, rz/2,
		fx/2, fy/2, fz/2,
		ux/2, uy/2, uz/2
	);
	carvesolid_sphere(
		win, 0x87cefa,
		cx-rx/2+fx/2+ux*3/2, cy-ry/2+fy/2+uy*3/2, cz-rz/2+fz/2+uz*3/2,
		rx/2, ry/2, rz/2,
		fx/2, fy/2, fz/2,
		ux/2, uy/2, uz/2
	);
}
static void example_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void example_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"example\" style=\"width:50%%;height:100px;float:left;background-color:#82a977;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void example_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void example_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void example_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)example_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)example_read_tui(win, sty, act, pin);
	else if(fmt == _html_)example_read_html(win, sty, act, pin);
	else if(fmt == _json_)example_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)example_read_vbo(win, sty, act, pin);
	else example_read_pixel(win, sty, act, pin);
}
static void example_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void example_list()
{
}
static void example_change()
{
}
static void example_stop(struct actor* act, struct pinid* pin)
{
}
static void example_start(struct actor* act, struct pinid* pin)
{
}
static void example_delete(struct actor* act)
{
	if(0 == act)return;
}
static void example_create(struct actor* act)
{
	if(0 == act)return;
}




void example_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('e', 'x', 'a', 'm', 'p', 'l', 'e', 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)example_create;
	p->ondelete = (void*)example_delete;
	p->onstart  = (void*)example_start;
	p->onstop   = (void*)example_stop;
	p->onlist   = (void*)example_list;
	p->onchoose = (void*)example_change;
	p->onread   = (void*)example_read;
	p->onwrite  = (void*)example_write;
}
