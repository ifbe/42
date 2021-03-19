#include "libuser.h"




static void status_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;
	gl41solid_rect(ctx,0x808080, vc,vr,vf);
}
static void status_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void status_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void status_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void status_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void status_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void status_read_bycam(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	status_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void status_read_byuiux(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* uuu;struct style* area;
	struct entity* wnd;struct style* rect;
	uuu = stack[sp-2].pchip;area = stack[sp-2].pfoot;
	wnd = stack[sp-4].pchip;rect = stack[sp-4].pfoot;

	float x0,y0,xn,yn;
	x0 = area->fs.vc[0];
	y0 = area->fs.vc[1];
	xn = area->fs.vc[0]+area->fs.vq[0];
	yn = area->fs.vc[1]+area->fs.vq[1];
	//say("%f,%f,%f,%f\n",x0,y0,dx,dy);

	struct fstyle fs;
	fs.vc[0] = (x0+xn)/2;fs.vc[1] = (y0+yn)/2;fs.vc[2] = 0.5;
	fs.vr[0] = (xn-x0)/2;fs.vr[1] =       0.0;fs.vr[2] = 0.0;
	fs.vf[0] =       0.0;fs.vf[1] = (yn-y0)/2;fs.vf[2] = 0.0;
	fs.vt[0] =       0.0;fs.vt[1] =       0.0;fs.vt[2] =-0.5;

	status_draw_gl41(ent,slot, 0,(void*)&fs, wnd,rect);
}
static void status_read_bywnd(_ent* ent,struct style* slot, _ent* wnd,struct style* area)
{
}




static void status_taking(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct entity* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;
//say("fmt=%.8s\n", &sup->fmt);

	switch(wnd->fmt){
	case _gl41full_:
		status_read_bywnd(ent,slot, wnd,area);
		break;
	case _virtual_:
		status_read_byuiux(ent,slot, stack,sp);
		break;
	default:
		status_read_bycam(ent,slot, stack,sp, arg,key);
		break;
	}
}
static void status_giving(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void status_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void status_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void status_search(struct entity* act)
{
}
static void status_modify(struct entity* act)
{
}
static void status_delete(struct entity* act)
{
}
static void status_create(struct entity* act, u8* arg)
{
}




void status_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s','t','a','t','u','s', 0, 0);

	p->oncreate = (void*)status_create;
	p->ondelete = (void*)status_delete;
	p->onsearch = (void*)status_search;
	p->onmodify = (void*)status_modify;

	p->onlinkup = (void*)status_linkup;
	p->ondiscon = (void*)status_discon;
	p->ontaking = (void*)status_taking;
	p->ongiving = (void*)status_giving;
}
