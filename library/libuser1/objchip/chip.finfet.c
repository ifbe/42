#include "libuser.h"




static void finfet_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void finfet_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void finfet_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
    int y;
    vec3 tc,tr,tf,tu;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;

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
static void finfet_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void finfet_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void finfet_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void finfet_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void finfet_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)finfet_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)finfet_read_tui(win, sty, act, pin);
	else if(fmt == _html_)finfet_read_html(win, sty, act, pin);
	else if(fmt == _json_)finfet_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)finfet_read_vbo2d(win, sty, act, pin);
		else finfet_read_vbo3d(win, sty, act, pin);
	}
	else finfet_read_pixel(win, sty, act, pin);
}
static void finfet_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void finfet_cread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void finfet_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void finfet_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void finfet_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
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
	p->onstart  = (void*)finfet_start;
	p->onstop   = (void*)finfet_stop;
	p->onget    = (void*)finfet_cread;
	p->onpost   = (void*)finfet_cwrite;
	p->onread   = (void*)finfet_sread;
	p->onwrite  = (void*)finfet_swrite;
}
