#include "libuser.h"




static void stair_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void stair_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int j;
	vec3 tc, tr, tf, tu;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carveline2d_rect(win, 0xffffff, vc, vr, vf);

	tr[0] = vr[0]/8;
	tr[1] = vr[1]/8;
	tr[2] = vr[2]/8;
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;
	for(j=-7;j<8;j+=2)
	{
		tc[0] = vc[0] + vr[0]*j/8 + vf[0]*j/8;
		tc[1] = vc[1] + vr[1]*j/8 + vf[1]*j/8;
		tc[2] = vc[2] + vr[2]*j/8 + vf[2]*j/8;
		carvesolid2d_rect(win, 0x808080, tc, tr, tf);
	}
}
static void stair_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int z;
	vec3 tc, tr, tf, tu;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carveline_prism4(win, 0xffffff, vc, vr, vf, vu);

	tr[0] = vr[0]/2;
	tr[1] = vr[1]/2;
	tr[2] = vr[2]/2;
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;
	tu[0] = vu[0]/16;
	tu[1] = vu[1]/16;
	tu[2] = vu[2]/16;

	for(z=1;z<16;z+=2)
	{
		tc[0] = vc[0] + vr[0]/2 + vf[0]*(z-8)/8 + vu[0]*z/16;
		tc[1] = vc[1] + vr[1]/2 + vf[1]*(z-8)/8 + vu[1]*z/16;
		tc[2] = vc[2] + vr[2]/2 + vf[2]*(z-8)/8 + vu[2]*z/16;
		carvesolid_prism4(win, 0x808080, tc, tr, tf, tu);
	}
	for(z=1;z<16;z+=2)
	{
		tc[0] = vc[0] - vr[0]/2 + vf[0]*(z-8)/8 - vu[0]*z/16;
		tc[1] = vc[1] - vr[1]/2 + vf[1]*(z-8)/8 - vu[1]*z/16;
		tc[2] = vc[2] - vr[2]/2 + vf[2]*(z-8)/8 - vu[2]*z/16;
		carvesolid_prism4(win, 0x808080, tc, tr, tf, tu);
	}
}
static void stair_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void stair_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void stair_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void stair_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void stair_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)stair_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)stair_read_tui(win, sty, act, pin);
	else if(fmt == _html_)stair_read_html(win, sty, act, pin);
	else if(fmt == _json_)stair_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)stair_read_vbo2d(win, sty, act, pin);
		else stair_read_vbo3d(win, sty, act, pin);
	}
	else stair_read_pixel(win, sty, act, pin);
}
static void stair_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void stair_cread()
{
}
static void stair_cwrite()
{
}
static void stair_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void stair_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void stair_delete(struct actor* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void stair_create(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256);
}




void stair_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 't', 'a', 'i', 'r', 0, 0, 0);

	p->oncreate = (void*)stair_create;
	p->ondelete = (void*)stair_delete;
	p->onstart  = (void*)stair_start;
	p->onstop   = (void*)stair_stop;
	p->onget    = (void*)stair_cread;
	p->onpost   = (void*)stair_cwrite;
	p->onread   = (void*)stair_sread;
	p->onwrite  = (void*)stair_swrite;
}
