#include "libuser.h"




static void tree_search(struct entity* act)
{
}
static void tree_modify(struct entity* act)
{
}
static void tree_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void tree_create(struct entity* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




static void tree_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
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

	drawsolid_rect(win, 0x008000, cx-ww, cy-hh*1/4, cx+ww, cy);
	drawsolid_rect(win, 0x008000, cx-ww*3/4, cy-hh/2, cx+ww*3/4, cy-hh/4);
	drawsolid_rect(win, 0x008000, cx-ww/2, cy-hh*3/4, cx+ww/2, cy-hh/2);
	drawsolid_rect(win, 0x008000, cx-ww/4, cy-hh, cx+ww/4, cy-hh*3/4);
}/*
static void tree_draw_d(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	vec3 tc, tr, tf, tu, f;

	float* vc = sty->fs.vc;
	float* vr = sty->fs.vr;
	float* vf = sty->fs.vf;
	float* vu = sty->fs.vt;
	//gl41solid_rect(win, 0x6a4b23, vc, vr, vf);

	tc[0] = vc[0]-vf[0]/2;
	tc[1] = vc[1]-vf[1]/2;
	tc[2] = vc[2]-vf[2]/2;
	tr[0] = vr[0]/16;
	tr[1] = vr[1]/16;
	tr[2] = vr[2]/16;
	tf[0] = vf[0]/2;
	tf[1] = vf[1]/2;
	tf[2] = vf[2]/2;
	gl41solid2d_rect(win, 0x404040, tc, tr, tf);

	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;

	tc[0] = vc[0]+vf[0]*1/8;
	tc[1] = vc[1]+vf[1]*1/8;
	tc[2] = vc[2]+vf[2]*1/8;
	tr[0] = vr[0]*4/4;
	tr[1] = vr[1]*4/4;
	tr[2] = vr[2]*4/4;
	gl41solid2d_rect(win, 0x00ff00, tc, tr, tf);

	tc[0] = vc[0]+vf[0]*3/8;
	tc[1] = vc[1]+vf[1]*3/8;
	tc[2] = vc[2]+vf[2]*3/8;
	tr[0] = vr[0]*3/4;
	tr[1] = vr[1]*3/4;
	tr[2] = vr[2]*3/4;
	gl41solid2d_rect(win, 0x00ff00, tc, tr, tf);

	tc[0] = vc[0]+vf[0]*5/8;
	tc[1] = vc[1]+vf[1]*5/8;
	tc[2] = vc[2]+vf[2]*5/8;
	tr[0] = vr[0]*2/4;
	tr[1] = vr[1]*2/4;
	tr[2] = vr[2]*2/4;
	gl41solid2d_rect(win, 0x00ff00, tc, tr, tf);

	tc[0] = vc[0]+vf[0]*7/8;
	tc[1] = vc[1]+vf[1]*7/8;
	tc[2] = vc[2]+vf[2]*7/8;
	tr[0] = vr[0]*1/4;
	tr[1] = vr[1]*1/4;
	tr[2] = vr[2]*1/4;
	gl41solid_prism4(win, 0x00ff00, tc, tr, tf, tu);
}*/
static void tree_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	vec3 tc, tr, tf, tu, f;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	//gl41solid_rect(ctx, 0x6a4b23, vc, vr, vf);
/*
	tc[0] = vc[0]-vu[0];
	tc[1] = vc[1]-vu[1];
	tc[2] = vc[2]-vu[2];
	gl41solid_cone(ctx, 0x6a4b23, tc, vr, vu);
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
	gl41solid_prism4(ctx, 0x404040, tc, tr, tf, tu);

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
	gl41solid_prism4(ctx, 0x00c000, tc, tr, tf, tu);

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
	gl41solid_prism4(ctx, 0x00c000, tc, tr, tf, tu);

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
	gl41solid_prism4(ctx, 0x00c000, tc, tr, tf, tu);

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
	gl41solid_prism4(ctx, 0x00c000, tc, tr, tf, tu);
}
static void tree_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tree_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tree_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tree_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void tree_wrl_cam_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	tree_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void tree_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].flag){
	}

	//caller defined behavior
	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->fmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	default:
		tree_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void tree_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void tree_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void tree_linkup(struct halfrel* self, struct halfrel* peer)
{
}




void tree_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('t', 'r', 'e', 'e');

	p->oncreate = (void*)tree_create;
	p->ondelete = (void*)tree_delete;
	p->onsearch = (void*)tree_search;
	p->onmodify = (void*)tree_modify;

	p->onlinkup = (void*)tree_linkup;
	p->ondiscon = (void*)tree_discon;
	p->ontaking = (void*)tree_taking;
	p->ongiving = (void*)tree_giving;
}
