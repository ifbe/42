#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void house_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void house_read_vbo(
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
		win, 0x404040,
		cx, cy, cz,
		rx, ry, rz,
		fx, fy, rz
	);

	//left
	carvesolid_rect(
		win, 0xc0c0c0,
		cx-rx+ux*3/4, cy-ry+uy*3/4, cz-rz+uz*3/4,
		fx, fy, fz,
		ux*3/4, uy*3/4, uz*3/4
	);
	carvesolid_triangle(
		win, 0xc0c0c0,
		cx-rx+ux*2,      cy-ry+uy*2,      cz-rz+uz*2,
		cx-rx-fx+ux*3/2, cy-ry-fy+uy*3/2, cz-rz-fz+uz*3/2,
		cx-rx+fx+ux*3/2, cy-ry+fy+uy*3/2, cz-rz+fz+uz*3/2
	);

	//right
	carvesolid_rect(
		win, 0xc0c0c0,
		cx+rx+ux*3/4, cy+ry*uy*3/4, cz+rz+uz*3/4,
		-fx, -fy, -fz,
		ux*3/4, uy*3/4, uz*3/4
	);
	carvesolid_triangle(
		win, 0xc0c0c0,
		cx+rx+ux*2,      cy+ry+uy*2,      cz+rz+uz*2,
		cx+rx-fx+ux*3/2, cy+ry-fy+uy*3/2, cz+rz-fz+uz*3/2,
		cx+rx+fx+ux*3/2, cy+ry+fy+uy*3/2, cz+rz+fz+uz*3/2
	);

	//back
	carvesolid_rect(
		win, 0xc0c0c0,
		cx+fx+ux*3/4, cy+fy+uy*3/4, cz+fz+uz*3/4,
		rx, ry, rz,
		ux*3/4, uy*3/4, uz*3/4
	);
	carvesolid_rect(
		win, 0xffffff,
		cx+fx/2+ux*7/4, cy+fy/2+uy*7/4, cz+fz/2+uz*7/4,
		-rx, -ry, -rz,
		ux/4-fx/2, uy/4-fy/2, uz/4-fz/2
	);

	//front
	carvesolid_rect(
		win, 0xc0c0c0,
		cx-fx+ux*3/4, cy-fy+uy*3/4, cz-fz+uz*3/4,
		rx, ry, rz,
		ux*3/4, uy*3/4, uz*3/4
	);
	carvesolid_rect(
		win, 0xffffff,
		cx-fx/2+ux*7/4, cy-fy/2+uy*7/4, cz-fz/2+uz*7/4,
		rx, ry, rz,
		ux/4+fx/2, uy/4+fy/2, uz/4+fz/2
	);
}
static void house_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void house_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void house_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void house_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void house_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)house_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)house_read_tui(win, sty, act, pin);
	else if(fmt == _html_)house_read_html(win, sty, act, pin);
	else if(fmt == _json_)house_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)house_read_vbo(win, sty, act, pin);
	else house_read_pixel(win, sty, act, pin);
}
static void house_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void house_list()
{
}
static void house_change()
{
}
static void house_stop(struct actor* act, struct pinid* pin)
{
}
static void house_start(struct actor* act, struct pinid* pin)
{
}
static void house_delete(struct actor* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void house_create(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256);
}




void house_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('h', 'o', 'u', 's', 'e', 0, 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)house_create;
	p->ondelete = (void*)house_delete;
	p->onstart  = (void*)house_start;
	p->onstop   = (void*)house_stop;
	p->onlist   = (void*)house_list;
	p->onchoose = (void*)house_change;
	p->onread   = (void*)house_read;
	p->onwrite  = (void*)house_write;
}
