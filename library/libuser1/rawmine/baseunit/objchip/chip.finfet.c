#include "libuser.h"




static void finfet_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}/*
static void finfet_draw_vbo2d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}*/
static void finfet_draw_vbo3d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int y;
	vec3 tc,tr,tf,tu;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;

	//body.base
	tc[0] = vc[0] + vu[0]/8;
	tc[1] = vc[1] + vu[1]/8;
	tc[2] = vc[2] + vu[2]/8;
	tu[0] = vu[0] / 8;
	tu[1] = vu[1] / 8;
	tu[2] = vu[2] / 8;
	carvesolid_prism4(win, 0x404040, tc, vr, vf, tu);

	//body.fin
	tf[0] = vf[0] / 16;
	tf[1] = vf[1] / 16;
	tf[2] = vf[2] / 16;
	tu[0] = vu[0] / 4;
	tu[1] = vu[1] / 4;
	tu[2] = vu[2] / 4;
	for(y=-1;y<2;y++)
	{
		tc[0] = vc[0] + y*vf[0]/2 + vu[0]/2;
		tc[1] = vc[1] + y*vf[1]/2 + vu[1]/2;
		tc[2] = vc[2] + y*vf[2]/2 + vu[2]/2;
		carvesolid_prism4(win, 0x404040, tc, vr, tf, tu);
	}

	//oxide
	tf[0] = vf[0] * 3 / 16;
	tf[1] = vf[1] * 3 / 16;
	tf[2] = vf[2] * 3 / 16;
	tu[0] = vu[0] / 8;
	tu[1] = vu[1] / 8;
	tu[2] = vu[2] / 8;
	for(y=-3;y<4;y+=2)
	{
		tc[0] = vc[0] + y*vf[0]/4 + vu[0]*3/8;
		tc[1] = vc[1] + y*vf[1]/4 + vu[1]*3/8;
		tc[2] = vc[2] + y*vf[2]/4 + vu[2]*3/8;
		carvesolid_prism4(win, 0x0000ff, tc, vr, tf, tu);
	}

	//metal.upper
	tc[0] = vc[0] + vu[0]*7/8;
	tc[1] = vc[1] + vu[1]*7/8;
	tc[2] = vc[2] + vu[2]*7/8;
	tr[0] = vr[0]/4;
	tr[1] = vr[1]/4;
	tr[2] = vr[2]/4;
	tu[0] = vu[0]/8;
	tu[1] = vu[1]/8;
	tu[2] = vu[2]/8;
	carvesolid_prism4(win, 0xff0000, tc, tr, vf, tu);

	//metal.bottom
	tf[0] = vf[0] * 3 / 16;
	tf[1] = vf[1] * 3 / 16;
	tf[2] = vf[2] * 3 / 16;
	tu[0] = vu[0] / 8;
	tu[1] = vu[1] / 8;
	tu[2] = vu[2] / 8;
	for(y=-3;y<4;y+=2)
	{
		tc[0] = vc[0] + y*vf[0]/4 + vu[0]*5/8;
		tc[1] = vc[1] + y*vf[1]/4 + vu[1]*5/8;
		tc[2] = vc[2] + y*vf[2]/4 + vu[2]*5/8;
		carvesolid_prism4(win, 0xff0000, tc, tr, tf, tu);
	}
}
static void finfet_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void finfet_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void finfet_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void finfet_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void finfet_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)finfet_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)finfet_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)finfet_draw_html(act, pin, win, sty);
	else if(fmt == _json_)finfet_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)finfet_draw_vbo2d(act, pin, win, sty);
		//else finfet_draw_vbo3d(act, pin, win, sty);
	}
	else finfet_draw_pixel(act, pin, win, sty);
}




static void finfet_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//finfet_draw(act, pin, win, sty);
}
static void finfet_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void finfet_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void finfet_start(struct halfrel* self, struct halfrel* peer)
{
}




static void finfet_search(struct actor* act, u8* buf)
{
}
static void finfet_modify(struct actor* act, u8* buf)
{
}
static void finfet_delete(struct actor* act, u8* buf)
{
}
static void finfet_create(struct actor* act, u8* buf)
{
}




void finfet_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('f','i','n','f','e','t', 0, 0);

	p->oncreate = (void*)finfet_create;
	p->ondelete = (void*)finfet_delete;
	p->onsearch = (void*)finfet_search;
	p->onmodify = (void*)finfet_modify;

	p->onstart = (void*)finfet_start;
	p->onstop  = (void*)finfet_stop;
	p->onread  = (void*)finfet_read;
	p->onwrite = (void*)finfet_write;
}
