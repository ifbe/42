#include "libuser.h"




static void spotlight_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void spotlight_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	vec3 tt;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;

	tt[0] = - vf[0];
	tt[1] = - vf[1];
	tt[2] = - vf[2];
	carvesolid_cone(win, 0xffff00, vc, vr, tt);
}
static void spotlight_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void spotlight_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void spotlight_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void spotlight_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void spotlight_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)spotlight_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)spotlight_read_tui(win, sty, act, pin);
	else if(fmt == _html_)spotlight_read_html(win, sty, act, pin);
	else if(fmt == _json_)spotlight_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)spotlight_read_vbo(win, sty, act, pin);
	else spotlight_read_pixel(win, sty, act, pin);
}
static void spotlight_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void spotlight_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void spotlight_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void spotlight_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void spotlight_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void spotlight_delete(struct actor* act)
{
}
static void spotlight_create(struct actor* act, void* str)
{
}




void spotlight_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('s','p','o','t','l', 'i', 't', 0);

	p->oncreate = (void*)spotlight_create;
	p->ondelete = (void*)spotlight_delete;
	p->onstart  = (void*)spotlight_start;
	p->onstop   = (void*)spotlight_stop;
	p->oncread  = (void*)spotlight_cread;
	p->oncwrite = (void*)spotlight_cwrite;
	p->onsread  = (void*)spotlight_sread;
	p->onswrite = (void*)spotlight_swrite;
}
