#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void tardis_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}/*
static void tardis_draw_vbo2d(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	carvesolid2d_rect(win, 0xff, vc, vr ,vf);
}*/
static void tardis_draw_vbo3d(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	vec3 tc,tr,tf,tu;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;

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
static void tardis_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tardis_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tardis_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tardis_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tardis_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)tardis_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)tardis_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)tardis_draw_html(act, pin, win, sty);
	else if(fmt == _json_)tardis_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)tardis_draw_vbo2d(act, pin, win, sty);
		//else tardis_draw_vbo3d(act, pin, win, sty);
	}
	else tardis_draw_pixel(act, pin, win, sty);
}




static void tardis_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//tardis_draw(act, pin, win, sty);
}
static void tardis_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void tardis_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void tardis_start(struct halfrel* self, struct halfrel* peer)
{
}




static void tardis_search(struct entity* act)
{
}
static void tardis_modify(struct entity* act)
{
}
static void tardis_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void tardis_create(struct entity* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void tardis_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'a', 'r', 'd', 'i', 's', 0, 0);

	p->oncreate = (void*)tardis_create;
	p->ondelete = (void*)tardis_delete;
	p->onsearch = (void*)tardis_search;
	p->onmodify = (void*)tardis_modify;

	p->onstart = (void*)tardis_start;
	p->onstop  = (void*)tardis_stop;
	p->onread  = (void*)tardis_read;
	p->onwrite = (void*)tardis_write;
}
