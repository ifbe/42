#include "libuser.h"




static void ladder_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void ladder_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void ladder_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void ladder_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void ladder_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void ladder_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_prism4(wnd, 0xffffff, vc, vr, vf, vt);
}




static void ladder_wrl_cam_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	ladder_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void ladder_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
	case _gl41full_:
		break;
	default:
		ladder_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void ladder_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void ladder_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void ladder_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void ladder_search(struct entity* act)
{
}
static void ladder_modify(struct entity* act)
{
}
static void ladder_delete(struct entity* act)
{
}
static void ladder_create(struct entity* act)
{
}




void ladder_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('l','a','d','d','e','r', 0, 0);

	p->oncreate = (void*)ladder_create;
	p->ondelete = (void*)ladder_delete;
	p->onsearch = (void*)ladder_search;
	p->onmodify = (void*)ladder_modify;

	p->onlinkup = (void*)ladder_linkup;
	p->ondiscon = (void*)ladder_discon;
	p->ontaking = (void*)ladder_taking;
	p->ongiving = (void*)ladder_giving;
}
