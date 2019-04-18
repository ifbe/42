#include "libuser.h"




static void dirlight_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void dirlight_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float x,y;
	vec3 ta, tb;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	for(y=-1.0;y<1.01;y+=0.1)
	{
		for(x=-1.0;x<1.01;x+=0.1)
		{
			ta[0] = vc[0] + x*vr[0] + y*vu[0];
			ta[1] = vc[1] + x*vr[1] + y*vu[1];
			ta[2] = vc[2] + x*vr[2] + y*vu[2];
			tb[0] = ta[0] - vf[0];
			tb[1] = ta[1] - vf[1];
			tb[2] = ta[2] - vf[2];
			carveline(win, 0xffff00, ta, tb);
		}
	}
}
static void dirlight_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void dirlight_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void dirlight_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void dirlight_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void dirlight_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)dirlight_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)dirlight_read_tui(win, sty, act, pin);
	else if(fmt == _html_)dirlight_read_html(win, sty, act, pin);
	else if(fmt == _json_)dirlight_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)dirlight_read_vbo(win, sty, act, pin);
	else dirlight_read_pixel(win, sty, act, pin);
}
static void dirlight_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void dirlight_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void dirlight_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void dirlight_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void dirlight_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void dirlight_delete(struct actor* act)
{
}
static void dirlight_create(struct actor* act, void* str)
{
}




void dirlight_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('d','i','r','l', 'i', 't', 0, 0);

	p->oncreate = (void*)dirlight_create;
	p->ondelete = (void*)dirlight_delete;
	p->onstart  = (void*)dirlight_start;
	p->onstop   = (void*)dirlight_stop;
	p->oncread  = (void*)dirlight_cread;
	p->oncwrite = (void*)dirlight_cwrite;
	p->onsread  = (void*)dirlight_sread;
	p->onswrite = (void*)dirlight_swrite;
}
