#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void tardis_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void tardis_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid2d_rect(win, 0xff, vc, vr ,vf);
}
static void tardis_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	vec3 tc,tr,tf,tu;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;

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
	carvesolid_prism4(win, 0x0000ff, tc, tr, tf, tu);
}
static void tardis_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void tardis_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void tardis_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void tardis_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void tardis_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)tardis_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)tardis_read_tui(win, sty, act, pin);
	else if(fmt == _html_)tardis_read_html(win, sty, act, pin);
	else if(fmt == _json_)tardis_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)tardis_read_vbo2d(win, sty, act, pin);
		else tardis_read_vbo3d(win, sty, act, pin);
	}
	else tardis_read_pixel(win, sty, act, pin);
}
static void tardis_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void tardis_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void tardis_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void tardis_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void tardis_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void tardis_delete(struct actor* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void tardis_create(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256);
}




void tardis_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'a', 'r', 'd', 'i', 's', 0, 0);

	p->oncreate = (void*)tardis_create;
	p->ondelete = (void*)tardis_delete;
	p->onstart  = (void*)tardis_start;
	p->onstop   = (void*)tardis_stop;
	p->oncread  = (void*)tardis_cread;
	p->oncwrite = (void*)tardis_cwrite;
	p->onsread  = (void*)tardis_sread;
	p->onswrite = (void*)tardis_swrite;
}
