#include "libuser.h"




static void sphere_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx = sty->vc[0];
	int cy = sty->vc[1];
	int ww = sty->vr[0];
	int hh = sty->vf[1];
	if(ww > hh)ww = hh;
	drawsolid_circle(win, 0x808080, cx, cy, ww);
}
static void sphere_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	vec3 tr, tf;
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
	carvesolid2d_sphere(win, 0xc0c0c0, vc, tr, tf, vu);
}
static void sphere_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	vec3 tc,tr,tf,tu;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	//carveline_rect(win, 0xffffff, vc, vr, vf);
	//carveopaque_sphere(win, 0xffffff, vc, vr, vf, vu);

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
	carvesolid_sphere(win, 0xc0c0c0, tc, tr, tf, tu);
	//carveopaque_sphere(win, 0xc0c0c0, tc, tr, tf, tu);
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
static void sphere_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
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
static void sphere_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void sphere_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void sphere_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void sphere_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void sphere_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
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
	p->fmt = hex64('s','p','h','e','r','e', 0, 0);

	p->oncreate = (void*)sphere_create;
	p->ondelete = (void*)sphere_delete;
	p->onstart  = (void*)sphere_start;
	p->onstop   = (void*)sphere_stop;
	p->oncread  = (void*)sphere_cread;
	p->oncwrite = (void*)sphere_cwrite;
	p->onsread  = (void*)sphere_sread;
	p->onswrite = (void*)sphere_swrite;
}
