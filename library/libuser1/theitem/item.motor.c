#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void motor_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void motor_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float a,c,s;
	vec3 v1,v2,v3;
	vec3 tc,tr,tf,tu;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carveline_circle(win, 0xffffff, vc, vr, vf);

	tr[0] = vr[0]/2;
	tr[1] = vr[1]/2;
	tr[2] = vr[2]/2;
	tf[0] = vf[0]/2;
	tf[1] = vf[1]/2;
	tf[2] = vf[2]/2;
	tu[0] = vu[0]/2;
	tu[1] = vu[1]/2;
	tu[2] = vu[2]/2;
	carveline_circle(win, 0xffffff, vc, tr, tf);

	tr[0] = vr[0]/4;
	tr[1] = vr[1]/4;
	tr[2] = vr[2]/4;
	tc[0] = vc[0] - vf[0]/2;
	tc[1] = vc[1] - vf[1]/2;
	tc[2] = vc[2] - vf[2]/2;
	carvesolid_rect(win, 0x0000ff, tc, tr, tu);
	tc[0] = vc[0] + vf[0]/2;
	tc[1] = vc[1] + vf[1]/2;
	tc[2] = vc[2] + vf[2]/2;
	carvesolid_rect(win, 0x0000ff, tc, tr, tu);

	tr[0] = vr[0]/8 + vf[0]*1.732/8;
	tr[1] = vr[1]/8 + vf[1]*1.732/8;
	tr[2] = vr[2]/8 + vf[2]*1.732/8;
	tc[0] = vc[0] + vr[0]*1.732/4 - vf[0]*0.25;
	tc[1] = vc[1] + vr[1]*1.732/4 - vf[1]*0.25;
	tc[2] = vc[2] + vr[2]*1.732/4 - vf[2]*0.25;
	carvesolid_rect(win, 0x00ff00, tc, tr, tu);
	tc[0] = vc[0] - vr[0]*1.732/4 + vf[0]*0.25;
	tc[1] = vc[1] - vr[1]*1.732/4 + vf[1]*0.25;
	tc[2] = vc[2] - vr[2]*1.732/4 + vf[2]*0.25;
	carvesolid_rect(win, 0x00ff00, tc, tr, tu);

	tr[0] = vr[0]/8 - vf[0]*1.732/8;
	tr[1] = vr[1]/8 - vf[1]*1.732/8;
	tr[2] = vr[2]/8 - vf[2]*1.732/8;
	tc[0] = vc[0] - vr[0]*1.732/4 - vf[0]*0.25;
	tc[1] = vc[1] - vr[1]*1.732/4 - vf[1]*0.25;
	tc[2] = vc[2] - vr[2]*1.732/4 - vf[2]*0.25;
	carvesolid_rect(win, 0xff0000, tc, tr, tu);
	tc[0] = vc[0] + vr[0]*1.732/4 + vf[0]*0.25;
	tc[1] = vc[1] + vr[1]*1.732/4 + vf[1]*0.25;
	tc[2] = vc[2] + vr[2]*1.732/4 + vf[2]*0.25;
	carvesolid_rect(win, 0xff0000, tc, tr, tu);




	a = timeread() / 1000000.0;

	s = sine(a);
	v1[0] = vc[0] + (vf[0]/2)*s;
	v1[1] = vc[1] + (vf[1]/2)*s;
	v1[2] = vc[2] + (vf[2]/2)*s;
	carveline(win, 0x0000ff, vc, v1);

	s = sine(a+PI*2/3);
	v2[0] = vc[0] + (vr[0]*1.732/4 - vf[0]*0.25)*s;
	v2[1] = vc[1] + (vr[1]*1.732/4 - vf[1]*0.25)*s;
	v2[2] = vc[2] + (vr[2]*1.732/4 - vf[2]*0.25)*s;
	carveline(win, 0x00ff00, vc, v2);

	s = sine(a+PI*4/3);
	v3[0] = vc[0] - (vr[0]*1.732/4 + vf[0]*0.25)*s;
	v3[1] = vc[1] - (vr[1]*1.732/4 + vf[1]*0.25)*s;
	v3[2] = vc[2] - (vr[2]*1.732/4 + vf[2]*0.25)*s;
	carveline(win, 0xff0000, vc, v3);

	tc[0] = v1[0] + v2[0] + v3[0] - vc[0]*2;
	tc[1] = v1[1] + v2[1] + v3[1] - vc[1]*2;
	tc[2] = v1[2] + v2[2] + v3[2] - vc[2]*2;
	carveline(win, 0xffffff, vc, tc);

}
static void motor_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void motor_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void motor_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void motor_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void motor_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)motor_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)motor_read_tui(win, sty, act, pin);
	else if(fmt == _html_)motor_read_html(win, sty, act, pin);
	else if(fmt == _json_)motor_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)motor_read_vbo(win, sty, act, pin);
	else motor_read_pixel(win, sty, act, pin);
}
static void motor_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void motor_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void motor_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void motor_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void motor_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void motor_delete(struct actor* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void motor_create(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256);
}




void motor_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('m', 'o', 't', 'o', 'r', 0, 0, 0);

	p->oncreate = (void*)motor_create;
	p->ondelete = (void*)motor_delete;
	p->onstart  = (void*)motor_start;
	p->onstop   = (void*)motor_stop;
	p->oncread  = (void*)motor_cread;
	p->oncwrite = (void*)motor_cwrite;
	p->onsread  = (void*)motor_sread;
	p->onswrite = (void*)motor_swrite;
}
