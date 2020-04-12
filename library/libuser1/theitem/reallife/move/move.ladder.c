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
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	gl41line_prism4(wnd, 0xffffff, vc, vr, vf, vt);
}
static void ladder_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* scn;struct style* geom;
	struct entity* wnd;struct style* area;

	if(stack && ('v'==key)){
		slot = stack[sp-1].pfoot;
		scn = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		ladder_draw_gl41(ent,slot, scn,geom, wnd,area);
	}
}




static void ladder_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	ladder_read_bycam(ent,foot, stack,sp, arg,key, buf,len);
}
static void ladder_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
	p->onread  = (void*)ladder_read;
	p->onwrite = (void*)ladder_write;
}
