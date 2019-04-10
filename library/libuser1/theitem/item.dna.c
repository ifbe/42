#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void dna_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void dna_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid2d_rect(win, 0x404040, vc, vr ,vf);
}
static void dna_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int z;
	float a,c,s;
	float A,C,S;
	vec3 tc,tr,tf,tu;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carveline_prism4(win, 0xffffff, vc, vr ,vf, vu);

	tf[0] = vf[0] / 16;
	tf[1] = vf[1] / 16;
	tf[2] = vf[2] / 16;
	tu[0] = vu[0] / 16;
	tu[1] = vu[1] / 16;
	tu[2] = vu[2] / 16;
	for(z=-6;z<=6;z++)
	{
		a = 2*PI*z/6;
		c = cosine(a)/2;
		s = sine(a)/2;

		tc[0] = vc[0] + vr[0]*c + vf[0]*s + vu[0]*z/6.0;
		tc[1] = vc[1] + vr[1]*c + vf[1]*s + vu[1]*z/6.0;
		tc[2] = vc[2] + vr[2]*c + vf[2]*s + vu[2]*z/6.0;
		tr[0] = vr[0] / 16;
		tr[1] = vr[1] / 16;
		tr[2] = vr[2] / 16;
		carvesolid_sphere(win, 0xff0000, tc, tr, tf, tu);

		tc[0] = vc[0] - vr[0]*c - vf[0]*s + vu[0]*z/6.0;
		tc[1] = vc[1] - vr[1]*c - vf[1]*s + vu[1]*z/6.0;
		tc[2] = vc[2] - vr[2]*c - vf[2]*s + vu[2]*z/6.0;
		carvesolid_sphere(win, 0x0000ff, tc, tr, tf, tu);
	}

	for(z=-6;z<=5;z++)
	{
		a = 2*PI*z/6;
		c = cosine(a)/2;
		s = sine(a)/2;
		A = 2*PI*(z+1)/6;
		C = cosine(A)/2;
		S = sine(A)/2;

		tc[0] = vc[0] + vr[0]*c + vf[0]*s + vu[0]*z/6.0;
		tc[1] = vc[1] + vr[1]*c + vf[1]*s + vu[1]*z/6.0;
		tc[2] = vc[2] + vr[2]*c + vf[2]*s + vu[2]*z/6.0;
		tr[0] = vc[0] + vr[0]*C + vf[0]*S + vu[0]*(z+1)/6.0;
		tr[1] = vc[1] + vr[1]*C + vf[1]*S + vu[1]*(z+1)/6.0;
		tr[2] = vc[2] + vr[2]*C + vf[2]*S + vu[2]*(z+1)/6.0;
		carveline(win, 0xff0000, tc, tr);

		tc[0] = vc[0] - vr[0]*c - vf[0]*s + vu[0]*z/6.0;
		tc[1] = vc[1] - vr[1]*c - vf[1]*s + vu[1]*z/6.0;
		tc[2] = vc[2] - vr[2]*c - vf[2]*s + vu[2]*z/6.0;
		tr[0] = vc[0] - vr[0]*C - vf[0]*S + vu[0]*(z+1)/6.0;
		tr[1] = vc[1] - vr[1]*C - vf[1]*S + vu[1]*(z+1)/6.0;
		tr[2] = vc[2] - vr[2]*C - vf[2]*S + vu[2]*(z+1)/6.0;
		carveline(win, 0x0000ff, tc, tr);
	}
}
static void dna_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void dna_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void dna_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void dna_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void dna_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)dna_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)dna_read_tui(win, sty, act, pin);
	else if(fmt == _html_)dna_read_html(win, sty, act, pin);
	else if(fmt == _json_)dna_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)dna_read_vbo2d(win, sty, act, pin);
		else dna_read_vbo3d(win, sty, act, pin);
	}
	else dna_read_pixel(win, sty, act, pin);
}
static void dna_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void dna_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void dna_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void dna_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void dna_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void dna_delete(struct actor* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void dna_create(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256);
}




void dna_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('d', 'n', 'a', 0);

	p->oncreate = (void*)dna_create;
	p->ondelete = (void*)dna_delete;
	p->onstart  = (void*)dna_start;
	p->onstop   = (void*)dna_stop;
	p->oncread  = (void*)dna_cread;
	p->oncwrite = (void*)dna_cwrite;
	p->onsread  = (void*)dna_sread;
	p->onswrite = (void*)dna_swrite;
}
