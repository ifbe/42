#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void house_read_pixel(
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
	drawline(win, 0x404040, cx-ww, cy-hh/3, cx, cy-hh);
	drawline(win, 0x404040, cx+ww, cy-hh/3, cx, cy-hh);
	drawline_rect(win, 0x404040, cx-ww, cy-hh/3, cx+ww, cy+hh);
	drawline_rect(win, 0x404040, cx-ww/3, cy+hh/3, cx+ww/3, cy+hh);
	drawline_rect(win, 0x404040, cx-ww*5/6, cy-hh*1/6, cx-ww*3/6, cy+hh*1/6);
	drawline_rect(win, 0x404040, cx+ww*3/6, cy-hh*1/6, cx+ww*5/6, cy+hh*1/6);
}
static void house_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	vec3 tc, tr, tf, tu, f;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid_rect(win, 0x404040, vc, vr, vf);

	//left
	tc[0] = vc[0]-vr[0]+vu[0]*3/4;
	tc[1] = vc[1]-vr[1]+vu[1]*3/4;
	tc[2] = vc[2]-vr[2]+vu[2]*3/4;
	tu[0] = vu[0]*3/4;
	tu[1] = vu[1]*3/4;
	tu[2] = vu[2]*3/4;
	carvesolid_rect(win, 0xc0c0c0, tc, vf, tu);

	tr[0] = vc[0]-vr[0]+vu[0]*2;
	tr[1] = vc[1]-vr[1]+vu[1]*2;
	tr[2] = vc[2]-vr[2]+vu[2]*2;
	tf[0] = vc[0]-vr[0]-vf[0]+vu[0]*3/2;
	tf[1] = vc[1]-vr[1]-vf[1]+vu[1]*3/2;
	tf[2] = vc[2]-vr[2]-vf[2]+vu[2]*3/2;
	tu[0] = vc[0]-vr[0]+vf[0]+vu[0]*3/2;
	tu[1] = vc[1]-vr[1]+vf[1]+vu[1]*3/2;
	tu[2] = vc[2]-vr[2]+vf[2]+vu[2]*3/2;
	carvesolid_triangle(win, 0xc0c0c0, tr, tf, tu);

	//right
	tc[0] = vc[0]+vr[0]+vu[0]*3/4;
	tc[1] = vc[1]+vr[1]*vu[1]*3/4;
	tc[2] = vc[2]+vr[2]+vu[2]*3/4;
	tr[0] = -vf[0];
	tr[1] = -vf[1];
	tr[2] = -vf[2];
	tf[0] = vu[0]*3/4;
	tf[1] = vu[1]*3/4;
	tf[2] = vu[2]*3/4;
	carvesolid_rect(win, 0xc0c0c0, tc, tr, tf);

	tr[0] = vc[0]+vr[0]+vu[0]*2;
	tr[1] = vc[1]+vr[1]+vu[1]*2;
	tr[2] = vc[2]+vr[2]+vu[2]*2;
	tf[0] = vc[0]+vr[0]-vf[0]+vu[0]*3/2;
	tf[1] = vc[1]+vr[1]-vf[1]+vu[1]*3/2;
	tf[2] = vc[2]+vr[2]-vf[2]+vu[2]*3/2;
	tu[0] = vc[0]+vr[0]+vf[0]+vu[0]*3/2;
	tu[1] = vc[1]+vr[1]+vf[1]+vu[1]*3/2;
	tu[2] = vc[2]+vr[2]+vf[2]+vu[2]*3/2;
	carvesolid_triangle(win, 0xc0c0c0, tr, tf, tu);

	//back
	tc[0] = vc[0]+vf[0]+vu[0]*3/4;
	tc[1] = vc[1]+vf[1]+vu[1]*3/4;
	tc[2] = vc[2]+vf[2]+vu[2]*3/4;
	tf[0] = vu[0]*3/4;
	tf[1] = vu[1]*3/4;
	tf[2] = vu[2]*3/4;
	carvesolid_rect(win, 0xc0c0c0, tc, vr, tf);

	tc[0] = vc[0]+vf[0]/2+vu[0]*7/4;
	tc[1] = vc[1]+vf[1]/2+vu[1]*7/4;
	tc[2] = vc[2]+vf[2]/2+vu[2]*7/4;
	tr[0] = -vr[0];
	tr[1] = -vr[1];
	tr[2] = -vr[2];
	tf[0] = vu[0]/4-vf[0]/2;
	tf[1] = vu[1]/4-vf[1]/2;
	tf[2] = vu[2]/4-vf[2]/2;
	carvesolid_rect(win, 0xffffff, tc, tr, tf);

	//front
	tc[0] = vc[0]-vf[0]+vu[0]*3/4;
	tc[1] = vc[1]-vf[1]+vu[1]*3/4;
	tc[2] = vc[2]-vf[2]+vu[2]*3/4;
	tf[0] = vu[0]*3/4;
	tf[1] = vu[1]*3/4;
	tf[2] = vu[2]*3/4;
	carvesolid_rect(win, 0xc0c0c0, tc, vr, tf);

	tc[0] = vc[0]-vf[0]/2+vu[0]*7/4;
	tc[1] = vc[1]-vf[1]/2+vu[1]*7/4;
	tc[2] = vc[2]-vf[2]/2+vu[2]*7/4;
	tf[0] = vu[0]/4+vf[0]/2;
	tf[1] = vu[1]/4+vf[1]/2;
	tf[2] = vu[2]/4+vf[2]/2;
	carvesolid_rect(win, 0xffffff, tc, vr, tf);
}
static void house_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void house_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void house_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void house_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void house_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)house_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)house_read_tui(win, sty, act, pin);
	else if(fmt == _html_)house_read_html(win, sty, act, pin);
	else if(fmt == _json_)house_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)house_read_vbo(win, sty, act, pin);
	else house_read_pixel(win, sty, act, pin);
}
static void house_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void house_list()
{
}
static void house_change()
{
}
static void house_stop(struct actor* act, struct pinid* pin)
{
}
static void house_start(struct actor* act, struct pinid* pin)
{
}
static void house_delete(struct actor* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void house_create(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256);
}




void house_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('h', 'o', 'u', 's', 'e', 0, 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)house_create;
	p->ondelete = (void*)house_delete;
	p->onstart  = (void*)house_start;
	p->onstop   = (void*)house_stop;
	p->onlist   = (void*)house_list;
	p->onchoose = (void*)house_change;
	p->onread   = (void*)house_read;
	p->onwrite  = (void*)house_write;
}
