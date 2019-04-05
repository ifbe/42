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
	vec3 tr,tf;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	float n = (win->height) * vf[1] / (win->width) / vr[0];
	if(n < 1.0)
	{
		tr[0] = vr[0] * n;
		tr[1] = vr[1] * n;
		tr[2] = vr[2] * n;
		tf[0] = vf[0];
		tf[1] = vf[1];
		tf[2] = vf[2];
	}
	else
	{
		tr[0] = vr[0];
		tr[1] = vr[1];
		tr[2] = vr[2];
		tf[0] = vf[0] / n;
		tf[1] = vf[1] / n;
		tf[2] = vf[2] / n;
	}
	carveopaque2d_rect(win, 0xffffff, vc, tr, tf);
}
static void cylinder_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	vec3 tc,tr,tf,tu;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	//carveline_rect(win, 0xffffff, vc, vr, vf);
	//carveopaque_cylinder(win, 0xffffff, vc, vr, vf, vu);

	tc[0] = vc[0] + vu[0]/2;
	tc[1] = vc[1] + vu[1]/2;
	tc[2] = vc[2] + vu[2]/2;
	tr[0] = vr[0]/2;
	tr[1] = vr[1]/2;
	tr[2] = vr[2]/2;
	tf[0] = vf[0]/2;
	tf[1] = vf[1]/2;
	tf[2] = vf[2]/2;
	tu[0] = vu[0]/2;
	tu[1] = vu[1]/2;
	tu[2] = vu[2]/2;
	carvesolid_cylinder(win, 0xc0c0c0, tc, tr, tf, tu);
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
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void cylinder_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
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
	p->fmt = hex64('c','y','l','i','n','d','e','r');

	p->oncreate = (void*)cylinder_create;
	p->ondelete = (void*)cylinder_delete;
	p->onstart  = (void*)cylinder_start;
	p->onstop   = (void*)cylinder_stop;
	p->onget    = (void*)cylinder_get;
	p->onpost   = (void*)cylinder_post;
	p->onread   = (void*)cylinder_read;
	p->onwrite  = (void*)cylinder_write;
}
