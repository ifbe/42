#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void tree_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx = sty->cx;
	int cy = sty->cy;
	int ww = sty->rx;
	int hh = sty->fy;

	drawline(win, 0x6a4b23, cx-ww, cy+hh, cx+ww, cy+hh);
	drawsolid_rect(win, 0x404040, cx-ww/4, cy, cx+ww/4, cy+hh);

	drawsolid_rect(win, 0x00ff00, cx-ww, cy-hh*1/4, cx+ww, cy);
	drawsolid_rect(win, 0x00ff00, cx-ww*3/4, cy-hh/2, cx+ww*3/4, cy-hh/4);
	drawsolid_rect(win, 0x00ff00, cx-ww/2, cy-hh*3/4, cx+ww/2, cy-hh/2);
	drawsolid_rect(win, 0x00ff00, cx-ww/4, cy-hh, cx+ww/4, cy-hh*3/4);
}
static void tree_read_vbo(
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

	//ground
	carvesolid_rect(
		win, 0x6a4b23,
		cx, cy, cz,
		rx, ry, rz,
		fx, fy, rz
	);
	carvesolid_prism4(
		win, 0x404040,
		cx+ux/2, cy+uy/2, cz+uz/2,
		rx/8, ry/8, rz/8,
		fx/8, fy/8, fz/8,
		ux/2, uy/2, uz/2
	);

	carvesolid_prism4(
		win, 0x00ff00,
		cx+ux*9/8, cy+uy*9/8, cz+uz*9/8,
		rx*7/8, ry*7/8, rz*7/8,
		fx*7/8, fy*7/8, fz*7/8,
		ux/8, uy/8, uz/8
	);
	carvesolid_prism4(
		win, 0x00ff00,
		cx+ux*11/8, cy+uy*11/8, cz+uz*11/8,
		rx*5/8, ry*5/8, rz*5/8,
		fx*5/8, fy*5/8, fz*5/8,
		ux/8, uy/8, uz/8
	);
	carvesolid_prism4(
		win, 0x00ff00,
		cx+ux*13/8, cy+uy*13/8, cz+uz*13/8,
		rx*3/8, ry*3/8, rz*3/8,
		fx*3/8, fy*3/8, fz*3/8,
		ux/8, uy/8, uz/8
	);
	carvesolid_prism4(
		win, 0x00ff00,
		cx+ux*15/8, cy+uy*15/8, cz+uz*15/8,
		rx/8, ry/8, rz/8,
		fx/8, fy/8, fz/8,
		ux/8, uy/8, uz/8
	);
}
static void tree_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void tree_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void tree_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void tree_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void tree_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)tree_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)tree_read_tui(win, sty, act, pin);
	else if(fmt == _html_)tree_read_html(win, sty, act, pin);
	else if(fmt == _json_)tree_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)tree_read_vbo(win, sty, act, pin);
	else tree_read_pixel(win, sty, act, pin);
}
static void tree_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void tree_list()
{
}
static void tree_change()
{
}
static void tree_stop(struct actor* act, struct pinid* pin)
{
}
static void tree_start(struct actor* act, struct pinid* pin)
{
}
static void tree_delete(struct actor* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void tree_create(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256);
}




void tree_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex32('t', 'r', 'e', 'e');
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)tree_create;
	p->ondelete = (void*)tree_delete;
	p->onstart  = (void*)tree_start;
	p->onstop   = (void*)tree_stop;
	p->onlist   = (void*)tree_list;
	p->onchoose = (void*)tree_change;
	p->onread   = (void*)tree_read;
	p->onwrite  = (void*)tree_write;
}
