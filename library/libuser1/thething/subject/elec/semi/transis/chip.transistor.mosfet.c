#include "libuser.h"




static void mosfet_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mosfet_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mosfet_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mosfet_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mosfet_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void mosfet_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_rect(wnd, 0xffffff, vc,vr,vf);
}
static void mosfet_wrl_cam_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	mosfet_draw_gl41(ent, slot, wor,geom, wnd,area);
}
static void mosfet_wrl_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
}
static void mosfet_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
}




static void mosfet_taking(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//foot defined behavior
	switch(stack[sp-1].flag){
	}

	//caller defined behavior
	switch(caller->fmt){
	case _rgba_:
		break;
	case _gl41full_:
		mosfet_wnd(ent,slot, stack,sp);break;
		break;
	default:
		mosfet_wrl_cam_wnd(ent,slot, stack,sp);
	}
}
static void mosfet_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void mosfet_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void mosfet_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void mosfet_search(struct entity* act, u8* buf)
{
}
static void mosfet_modify(struct entity* act, u8* buf)
{
}
static void mosfet_delete(struct entity* act, u8* buf)
{
}
static void mosfet_create(struct entity* act, void* arg, int argc, u8** argv)
{
	act->ix0 = 0;	//D
	act->iy0 = 0;	//S
	act->iz0 = 0;	//G
	act->iw0 = 0;	//B
}




void mosfet_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('m','o','s','f','e','t', 0, 0);

	p->oncreate = (void*)mosfet_create;
	p->ondelete = (void*)mosfet_delete;
	p->onsearch = (void*)mosfet_search;
	p->onmodify = (void*)mosfet_modify;

	p->onlinkup = (void*)mosfet_linkup;
	p->ondiscon = (void*)mosfet_discon;
	p->ontaking = (void*)mosfet_taking;
	p->ongiving = (void*)mosfet_giving;
}
