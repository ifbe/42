#include "libuser.h"




static void tree_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
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
}/*
static void tree_draw_vbo2d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	vec3 tc, tr, tf, tu, f;
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
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
}*/
static void tree_draw_vbo3d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	vec3 tc, tr, tf, tu, f;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
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
static void tree_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void tree_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void tree_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void tree_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void tree_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)tree_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)tree_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)tree_draw_html(act, pin, win, sty);
	else if(fmt == _json_)tree_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)tree_draw_vbo2d(act, pin, win, sty);
		//else tree_draw_vbo3d(act, pin, win, sty);
	}
	else tree_draw_pixel(act, pin, win, sty);
}




static void tree_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//tree_draw(act, pin, win, sty);
}
static void tree_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void tree_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void tree_start(struct halfrel* self, struct halfrel* peer)
{
}




static void tree_search(struct actor* act)
{
}
static void tree_modify(struct actor* act)
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
	p->onsearch = (void*)tree_search;
	p->onmodify = (void*)tree_modify;

	p->onstart = (void*)tree_start;
	p->onstop  = (void*)tree_stop;
	p->onread  = (void*)tree_read;
	p->onwrite = (void*)tree_write;
}
