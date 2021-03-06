#include "libuser.h"




static void gaafet_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void gaafet_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void gaafet_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void gaafet_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void gaafet_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void gaafet_draw_gl41(
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




static void gaafet_wrl_cam_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	gaafet_draw_gl41(ent, slot, wor,geom, wnd,area);
}
static void gaafet_wrl_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
}
static void gaafet_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
}




static void gaafet_taking(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].flag){
	}

	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//caller defined behavior
	switch(caller->fmt){
	case _rgba_:
		break;
	case _gl41list_:
		gaafet_wnd(ent,slot, stack,sp);break;
		break;
	default:
		gaafet_wrl_cam_wnd(ent,slot, stack,sp);
	}
}
static void gaafet_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void gaafet_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void gaafet_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void gaafet_search(struct entity* act, u8* buf)
{
}
static void gaafet_modify(struct entity* act, u8* buf)
{
}
static void gaafet_delete(struct entity* act, u8* buf)
{
}
static void gaafet_create(struct entity* act, void* arg, int argc, u8** argv)
{
	act->ix0 = 0;	//D
	act->iy0 = 0;	//S
	act->iz0 = 0;	//G
	act->iw0 = 0;	//B
}




void gaafet_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('g','a','a','f','e','t', 0, 0);

	p->oncreate = (void*)gaafet_create;
	p->ondelete = (void*)gaafet_delete;
	p->onsearch = (void*)gaafet_search;
	p->onmodify = (void*)gaafet_modify;

	p->onlinkup = (void*)gaafet_linkup;
	p->ondiscon = (void*)gaafet_discon;
	p->ontaking = (void*)gaafet_taking;
	p->ongiving = (void*)gaafet_giving;
}
