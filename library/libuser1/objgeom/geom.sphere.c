#include "libuser.h"




static void sphere_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void sphere_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid2d_sphere(win, 0xc0c0c0, vc, vr, vf, vu);
}
static void sphere_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid_sphere(win, 0xc0c0c0, vc, vr, vf, vu);
}
static void sphere_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void sphere_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void sphere_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void sphere_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void sphere_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)sphere_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)sphere_read_tui(win, sty, act, pin);
	else if(fmt == _html_)sphere_read_html(win, sty, act, pin);
	else if(fmt == _json_)sphere_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)sphere_read_vbo2d(win, sty, act, pin);
		else sphere_read_vbo3d(win, sty, act, pin);
	}
	else sphere_read_pixel(win, sty, act, pin);
}




static void sphere_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void sphere_post(u8* buf, int len)
{
}
static void sphere_get(u8* buf, int len)
{
}
static void sphere_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void sphere_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void sphere_delete(struct actor* act, u8* buf)
{
}
static void sphere_create(struct actor* act, u8* buf)
{
}




void sphere_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('s','p','h','e','r','e', 0, 0);

	p->oncreate = (void*)sphere_create;
	p->ondelete = (void*)sphere_delete;
	p->onstart  = (void*)sphere_start;
	p->onstop   = (void*)sphere_stop;
	p->onget    = (void*)sphere_get;
	p->onpost   = (void*)sphere_post;
	p->onread   = (void*)sphere_read;
	p->onwrite  = (void*)sphere_write;
}
