#include "libuser.h"




static void cylinder_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void cylinder_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void cylinder_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid_cylinder(win, 0xc0c0c0, vc, vr, vf, vu);
}
static void cylinder_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void cylinder_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void cylinder_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void cylinder_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void cylinder_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)cylinder_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)cylinder_read_tui(win, sty, act, pin);
	else if(fmt == _html_)cylinder_read_html(win, sty, act, pin);
	else if(fmt == _json_)cylinder_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)cylinder_read_vbo2d(win, sty, act, pin);
		else cylinder_read_vbo3d(win, sty, act, pin);
	}
	else cylinder_read_pixel(win, sty, act, pin);
}




static void cylinder_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void cylinder_post(u8* buf, int len)
{
}
static void cylinder_get(u8* buf, int len)
{
}
static void cylinder_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void cylinder_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void cylinder_delete(struct actor* act, u8* buf)
{
}
static void cylinder_create(struct actor* act, u8* buf)
{
}




void cylinder_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('c','y','l','i','n','d','e','r');

	p->oncreate = (void*)cylinder_create;
	p->ondelete = (void*)cylinder_delete;
	p->onstart  = (void*)cylinder_start;
	p->onstop   = (void*)cylinder_stop;
	p->onget    = (void*)cylinder_get;
	p->onpost   = (void*)cylinder_post;
	p->onread   = (void*)cylinder_read;
	p->onwrite  = (void*)cylinder_write;
}
