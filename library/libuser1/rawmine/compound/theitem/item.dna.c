#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void dna_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void dna_draw_vbo2d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	carvesolid2d_rect(win, 0x404040, vc, vr ,vf);
}
static void dna_draw_vbo3d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int z;
	float a,c,s;
	float A,C,S;
	vec3 tc,tr,tf,tu;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
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
static void dna_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void dna_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void dna_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void dna_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void dna_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)dna_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)dna_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)dna_draw_html(act, pin, win, sty);
	else if(fmt == _json_)dna_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)dna_draw_vbo2d(act, pin, win, sty);
		//else dna_draw_vbo3d(act, pin, win, sty);
	}
	else dna_draw_pixel(act, pin, win, sty);
}




static void dna_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//dna_draw(act, pin, win, sty);
}
static void dna_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void dna_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void dna_start(struct halfrel* self, struct halfrel* peer)
{
}




static void dna_search(struct actor* act)
{
}
static void dna_modify(struct actor* act)
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
	p->onsearch = (void*)dna_search;
	p->onmodify = (void*)dna_modify;

	p->onstart = (void*)dna_start;
	p->onstop  = (void*)dna_stop;
	p->onread  = (void*)dna_read;
	p->onwrite = (void*)dna_write;
}
