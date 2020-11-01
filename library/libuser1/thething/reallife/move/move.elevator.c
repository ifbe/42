#include "libuser.h"




static void elevator_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void elevator_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void elevator_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void elevator_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void elevator_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void elevator_draw_gl41(
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
static void elevator_read_bycam(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* scn;struct style* geom;
	struct entity* wnd;struct style* area;

	if(stack && ('v'==key)){
		slot = stack[sp-1].pfoot;
		scn = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		elevator_draw_gl41(ent,slot, scn,geom, wnd,area);
	}
}




static void elevator_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	elevator_read_bycam(ent,foot, stack,sp, arg,key, buf,len);
}
static void elevator_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void elevator_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void elevator_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void elevator_search(struct entity* act)
{
}
static void elevator_modify(struct entity* act)
{
}
static void elevator_delete(struct entity* act)
{
}
static void elevator_create(struct entity* act)
{
}




void elevator_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('e','l','e','v','a','t','o','r');

	p->oncreate = (void*)elevator_create;
	p->ondelete = (void*)elevator_delete;
	p->onsearch = (void*)elevator_search;
	p->onmodify = (void*)elevator_modify;

	p->onlinkup = (void*)elevator_linkup;
	p->ondiscon = (void*)elevator_discon;
	p->ontaking = (void*)elevator_taking;
	p->ongiving = (void*)elevator_giving;
}
