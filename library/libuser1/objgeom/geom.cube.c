#include "libuser.h"




static void cube_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void cube_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void cube_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid_prism4(win, 0xc0c0c0, vc, vr, vf, vu);
}
static void cube_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void cube_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void cube_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void cube_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void cube_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)cube_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)cube_read_tui(win, sty, act, pin);
	else if(fmt == _html_)cube_read_html(win, sty, act, pin);
	else if(fmt == _json_)cube_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)cube_read_vbo2d(win, sty, act, pin);
		else cube_read_vbo3d(win, sty, act, pin);
	}
	else cube_read_pixel(win, sty, act, pin);
}




static void cube_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void cube_post(u8* buf, int len)
{
}
static void cube_get(u8* buf, int len)
{
}
static void cube_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void cube_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void cube_delete(struct actor* act, u8* buf)
{
}
static void cube_create(struct actor* act, u8* buf)
{
}




void cube_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex32('c','u','b','e');

	p->oncreate = (void*)cube_create;
	p->ondelete = (void*)cube_delete;
	p->onstart  = (void*)cube_start;
	p->onstop   = (void*)cube_stop;
	p->onget    = (void*)cube_get;
	p->onpost   = (void*)cube_post;
	p->onread   = (void*)cube_read;
	p->onwrite  = (void*)cube_write;
}
