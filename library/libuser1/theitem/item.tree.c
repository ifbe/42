#include "libuser.h"




static void tree_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->vc[0];
		cy = sty->vc[1];
		ww = sty->vr[0];
		hh = sty->vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}

	drawline(win, 0x6a4b23, cx-ww, cy+hh, cx+ww, cy+hh);
	drawsolid_rect(win, 0x404040, cx-ww/4, cy, cx+ww/4, cy+hh);

	drawsolid_rect(win, 0x00ff00, cx-ww, cy-hh*1/4, cx+ww, cy);
	drawsolid_rect(win, 0x00ff00, cx-ww*3/4, cy-hh/2, cx+ww*3/4, cy-hh/4);
	drawsolid_rect(win, 0x00ff00, cx-ww/2, cy-hh*3/4, cx+ww/2, cy-hh/2);
	drawsolid_rect(win, 0x00ff00, cx-ww/4, cy-hh, cx+ww/4, cy-hh*3/4);
}
static void tree_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	vec3 tc, tr, tf, tu, f;
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	//carvesolid_rect(win, 0x6a4b23, vc, vr, vf);

	tc[0] = vc[0]-vf[0]/2;
	tc[1] = vc[1]-vf[1]/2;
	tc[2] = vc[2]-vf[2]/2;
	tr[0] = vr[0]/16;
	tr[1] = vr[1]/16;
	tr[2] = vr[2]/16;
	tf[0] = vf[0]/2;
	tf[1] = vf[1]/2;
	tf[2] = vf[2]/2;
	carvesolid2d_rect(win, 0x404040, tc, tr, tf);

	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;

	tc[0] = vc[0]+vf[0]*1/8;
	tc[1] = vc[1]+vf[1]*1/8;
	tc[2] = vc[2]+vf[2]*1/8;
	tr[0] = vr[0]*4/4;
	tr[1] = vr[1]*4/4;
	tr[2] = vr[2]*4/4;
	carvesolid2d_rect(win, 0x00ff00, tc, tr, tf);

	tc[0] = vc[0]+vf[0]*3/8;
	tc[1] = vc[1]+vf[1]*3/8;
	tc[2] = vc[2]+vf[2]*3/8;
	tr[0] = vr[0]*3/4;
	tr[1] = vr[1]*3/4;
	tr[2] = vr[2]*3/4;
	carvesolid2d_rect(win, 0x00ff00, tc, tr, tf);

	tc[0] = vc[0]+vf[0]*5/8;
	tc[1] = vc[1]+vf[1]*5/8;
	tc[2] = vc[2]+vf[2]*5/8;
	tr[0] = vr[0]*2/4;
	tr[1] = vr[1]*2/4;
	tr[2] = vr[2]*2/4;
	carvesolid2d_rect(win, 0x00ff00, tc, tr, tf);

	tc[0] = vc[0]+vf[0]*7/8;
	tc[1] = vc[1]+vf[1]*7/8;
	tc[2] = vc[2]+vf[2]*7/8;
	tr[0] = vr[0]*1/4;
	tr[1] = vr[1]*1/4;
	tr[2] = vr[2]*1/4;
	carvesolid_prism4(win, 0x00ff00, tc, tr, tf, tu);
}
static void tree_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	vec3 tc, tr, tf, tu, f;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	//carvesolid_rect(win, 0x6a4b23, vc, vr, vf);
/*
	tc[0] = vc[0]-vu[0];
	tc[1] = vc[1]-vu[1];
	tc[2] = vc[2]-vu[2];
	carvesolid_cone(win, 0x6a4b23, tc, vr, vu);
*/
	tc[0] = vc[0]+vu[0]/4;
	tc[1] = vc[1]+vu[1]/4;
	tc[2] = vc[2]+vu[2]/4;
	tr[0] = vr[0]/16;
	tr[1] = vr[1]/16;
	tr[2] = vr[2]/16;
	tf[0] = vf[0]/16;
	tf[1] = vf[1]/16;
	tf[2] = vf[2]/16;
	tu[0] = vu[0]/4;
	tu[1] = vu[1]/4;
	tu[2] = vu[2]/4;
	carvesolid_prism4(win, 0x404040, tc, tr, tf, tu);

	tc[0] = vc[0]+vu[0]*9/16;
	tc[1] = vc[1]+vu[1]*9/16;
	tc[2] = vc[2]+vu[2]*9/16;
	tr[0] = vr[0]*7/16;
	tr[1] = vr[1]*7/16;
	tr[2] = vr[2]*7/16,
	tf[0] = vf[0]*7/16;
	tf[1] = vf[1]*7/16;
	tf[2] = vf[2]*7/16;
	tu[0] = vu[0]/16;
	tu[1] = vu[1]/16;
	tu[2] = vu[2]/16;
	carvesolid_prism4(win, 0x00ff00, tc, tr, tf, tu);

	tc[0] = vc[0]+vu[0]*11/16;
	tc[1] = vc[1]+vu[1]*11/16;
	tc[2] = vc[2]+vu[2]*11/16;
	tr[0] = vr[0]*5/16;
	tr[1] = vr[1]*5/16;
	tr[2] = vr[2]*5/16;
	tf[0] = vf[0]*5/16;
	tf[1] = vf[1]*5/16;
	tf[2] = vf[2]*5/16;
	tu[0] = vu[0]/16;
	tu[1] = vu[1]/16;
	tu[2] = vu[2]/16;
	carvesolid_prism4(win, 0x00ff00, tc, tr, tf, tu);

	tc[0] = vc[0]+vu[0]*13/16;
	tc[1] = vc[1]+vu[1]*13/16;
	tc[2] = vc[2]+vu[2]*13/16;
	tr[0] = vr[0]*3/16;
	tr[1] = vr[1]*3/16;
	tr[2] = vr[2]*3/16;
	tf[0] = vf[0]*3/16;
	tf[1] = vf[1]*3/16;
	tf[2] = vf[2]*3/16;
	tu[0] = vu[0]/16;
	tu[1] = vu[1]/16;
	tu[2] = vu[2]/16;
	carvesolid_prism4(win, 0x00ff00, tc, tr, tf, tu);

	tc[0] = vc[0]+vu[0]*15/16;
	tc[1] = vc[1]+vu[1]*15/16;
	tc[2] = vc[2]+vu[2]*15/16;
	tr[0] = vr[0]/16;
	tr[1] = vr[1]/16;
	tr[2] = vr[2]/16;
	tf[0] = vf[0]/16;
	tf[1] = vf[1]/16;
	tf[2] = vf[2]/16;
	tu[0] = vu[0]/16;
	tu[1] = vu[1]/16;
	tu[2] = vu[2]/16;
	carvesolid_prism4(win, 0x00ff00, tc, tr, tf, tu);
}
static void tree_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void tree_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void tree_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void tree_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void tree_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)tree_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)tree_read_tui(win, sty, act, pin);
	else if(fmt == _html_)tree_read_html(win, sty, act, pin);
	else if(fmt == _json_)tree_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)tree_read_vbo2d(win, sty, act, pin);
		else tree_read_vbo3d(win, sty, act, pin);
	}
	else tree_read_pixel(win, sty, act, pin);
}
static void tree_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void tree_cread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void tree_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void tree_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void tree_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void tree_delete(struct actor* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void tree_create(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256);
}




void tree_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('t', 'r', 'e', 'e');

	p->oncreate = (void*)tree_create;
	p->ondelete = (void*)tree_delete;
	p->onstart  = (void*)tree_start;
	p->onstop   = (void*)tree_stop;
	p->onget    = (void*)tree_cread;
	p->onpost   = (void*)tree_cwrite;
	p->onread   = (void*)tree_sread;
	p->onwrite  = (void*)tree_swrite;
}
