#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




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
static void tree_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	vec3 tc, tr, tf, tu, f;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid_rect(win, 0x6a4b23, vc, vr, vf);

	tc[0] = vc[0]+vu[0]/2;
	tc[1] = vc[1]+vu[1]/2;
	tc[2] = vc[2]+vu[2]/2;
	tr[0] = vr[0]/8;
	tr[1] = vr[1]/8;
	tr[2] = vr[2]/8;
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;
	tu[0] = vu[0]/2;
	tu[1] = vu[1]/2;
	tu[2] = vu[2]/2;
	carvesolid_prism4(win, 0x404040, tc, tr, tf, tu);

	tc[0] = vc[0]+vu[0]*9/8;
	tc[1] = vc[1]+vu[1]*9/8;
	tc[2] = vc[2]+vu[2]*9/8;
	tr[0] = vr[0]*7/8;
	tr[1] = vr[1]*7/8;
	tr[2] = vr[2]*7/8,
	tf[0] = vf[0]*7/8;
	tf[1] = vf[1]*7/8;
	tf[2] = vf[2]*7/8;
	tu[0] = vu[0]/8;
	tu[1] = vu[1]/8;
	tu[2] = vu[2]/8;
	carvesolid_prism4(win, 0x00ff00, tc, tr, tf, tu);

	tc[0] = vc[0]+vu[0]*11/8;
	tc[1] = vc[1]+vu[1]*11/8;
	tc[2] = vc[2]+vu[2]*11/8;
	tr[0] = vr[0]*5/8;
	tr[1] = vr[1]*5/8;
	tr[2] = vr[2]*5/8;
	tf[0] = vf[0]*5/8;
	tf[1] = vf[1]*5/8;
	tf[2] = vf[2]*5/8;
	tu[0] = vu[0]/8;
	tu[1] = vu[1]/8;
	tu[2] = vu[2]/8;
	carvesolid_prism4(win, 0x00ff00, tc, tr, tf, tu);

	tc[0] = vc[0]+vu[0]*13/8;
	tc[1] = vc[1]+vu[1]*13/8;
	tc[2] = vc[2]+vu[2]*13/8;
	tr[0] = vr[0]*3/8;
	tr[1] = vr[1]*3/8;
	tr[2] = vr[2]*3/8;
	tf[0] = vf[0]*3/8;
	tf[1] = vf[1]*3/8;
	tf[2] = vf[2]*3/8;
	tu[0] = vu[0]/8;
	tu[1] = vu[1]/8;
	tu[2] = vu[2]/8;
	carvesolid_prism4(win, 0x00ff00, tc, tr, tf, tu);

	tc[0] = vc[0]+vu[0]*15/8;
	tc[1] = vc[1]+vu[1]*15/8;
	tc[2] = vc[2]+vu[2]*15/8;
	tr[0] = vr[0]/8;
	tr[1] = vr[1]/8;
	tr[2] = vr[2]/8;
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;
	tu[0] = vu[0]/8;
	tu[1] = vu[1]/8;
	tu[2] = vu[2]/8;
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
static void tree_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)tree_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)tree_read_tui(win, sty, act, pin);
	else if(fmt == _html_)tree_read_html(win, sty, act, pin);
	else if(fmt == _json_)tree_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)tree_read_vbo(win, sty, act, pin);
	else tree_read_pixel(win, sty, act, pin);
}
static void tree_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void tree_get()
{
}
static void tree_post()
{
}
static void tree_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void tree_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
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
	p->name = hex32('t', 'r', 'e', 'e');

	p->oncreate = (void*)tree_create;
	p->ondelete = (void*)tree_delete;
	p->onstart  = (void*)tree_start;
	p->onstop   = (void*)tree_stop;
	p->onget    = (void*)tree_get;
	p->onpost   = (void*)tree_post;
	p->onread   = (void*)tree_read;
	p->onwrite  = (void*)tree_write;
}
