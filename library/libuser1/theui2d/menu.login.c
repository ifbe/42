#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void login_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void login_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	vec3 tc,tr,tf;
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid2d_rect(win, 0x808080, vc, vr ,vf);

	tc[0] = vc[0] + vf[0]/2;
	tc[1] = vc[1] + vf[1]/2;
	tc[2] = vc[2] + vf[2]/2 - 0.1;
	tr[0] = vr[0] / 4;
	tr[1] = vr[1] / 4;
	tr[2] = vr[2] / 4;
	tf[0] = vf[0] / 4;
	tf[1] = vf[1] / 4;
	tf[2] = vf[2] / 4;
	carvestring2d_center(win, 0xffffff, tc, tr, tf, (void*)"welcome!", 8);

	tc[0] -= vf[0]/8;
	tc[1] -= vf[1]/8;
	tc[2] -= vf[2]/8;
	tf[0] = - vr[0]/16 - vf[0]/4;
	tf[1] = - vr[1]/16 - vf[1]/4;
	tf[2] = - vr[2]/16 - vf[2]-4;
	carvestring2d_center(win, 0xffffff, tc, tr, tf, (void*)"welcome!", 8);

	tc[0] = vc[0] - vf[0]/8;
	tc[1] = vc[1] - vf[1]/8;
	tc[2] = vc[2] - vf[2]/8 - 0.1;
	tr[0] = vr[0] / 4;
	tr[1] = vr[1] / 4;
	tr[2] = vr[2] / 4;
	tf[0] = vf[0] / 16;
	tf[1] = vf[1] / 16;
	tf[2] = vf[2] / 16;
	carvesolid2d_rect(win, 0x202020, tc, tr, tf);
	tc[2] -= 0.1;
	tr[0] /= 8;
	tr[1] /= 8;
	tr[1] /= 8;
	carvestring2d_center(win, 0xffffff, tc, tr, tf, (void*)"username", 8);

	tc[0] = vc[0] - vf[0]*3/8;
	tc[1] = vc[1] - vf[1]*3/8;
	tc[2] = vc[2] - vf[2]*3/8 - 0.1;
	tr[0] = vr[0] / 4;
	tr[1] = vr[1] / 4;
	tr[2] = vr[2] / 4;
	tf[0] = vf[0] / 16;
	tf[1] = vf[1] / 16;
	tf[2] = vf[2] / 16;
	carvesolid2d_rect(win, 0x202020, tc, tr, tf);
	tc[2] -= 0.1;
	tr[0] /= 8;
	tr[1] /= 8;
	tr[1] /= 8;
	carvestring2d_center(win, 0xffffff, tc, tr, tf, (void*)"password", 8);
}
static void login_read_vbo3d(
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
}
static void login_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void login_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void login_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void login_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void login_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)login_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)login_read_tui(win, sty, act, pin);
	else if(fmt == _html_)login_read_html(win, sty, act, pin);
	else if(fmt == _json_)login_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)login_read_vbo2d(win, sty, act, pin);
		else login_read_vbo3d(win, sty, act, pin);
	}
	else login_read_pixel(win, sty, act, pin);
}
static void login_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void login_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void login_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void login_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void login_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void login_delete(struct actor* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void login_create(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256);
}




void login_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('l', 'o', 'g', 'i', 'n', 0, 0, 0);

	p->oncreate = (void*)login_create;
	p->ondelete = (void*)login_delete;
	p->onstart  = (void*)login_start;
	p->onstop   = (void*)login_stop;
	p->oncread  = (void*)login_cread;
	p->oncwrite = (void*)login_cwrite;
	p->onsread  = (void*)login_sread;
	p->onswrite = (void*)login_swrite;
}
