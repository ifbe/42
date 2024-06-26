#include "libuser.h"




static void graphene_search(_obj* act, u8* buf)
{
}
static void graphene_modify(_obj* act, u8* buf)
{
}
static void graphene_delete(_obj* act, u8* buf)
{
}
static void graphene_create(_obj* act, u8* buf)
{
}




static void graphene_draw_gl41(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_prism4(ctx, 0x444444, vc, vr, vf, vt);

	for(j=0;j<3;j++){
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
		tt[j] = vt[j]/4;
	}
	gl41solid_sphere(ctx, 0x808080, vc, tr, tf, tt);
}
static void graphene_draw_pixel(
	_obj* act, struct style* pin,
	_obj* wnd, struct style* sty)
{
}
static void graphene_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void graphene_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void graphene_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void graphene_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void graphene_read_byworld_bycam_bywnd(_obj* ent,void* foot, _syn* stack,int sp)
{
	struct style* slot;
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	slot = stack[sp-1].pfoot;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	graphene_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void graphene_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* caller = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
	case _render_:
		break;
	default:
		graphene_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void graphene_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void graphene_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void graphene_attach(struct halfrel* self, struct halfrel* peer)
{
}




void graphene_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('g','r','a','p','h','e','n','e');

	p->oncreate = (void*)graphene_create;
	p->ondelete = (void*)graphene_delete;
	p->onreader = (void*)graphene_search;
	p->onwriter = (void*)graphene_modify;

	p->onattach = (void*)graphene_attach;
	p->ondetach = (void*)graphene_detach;
	p->ontaking = (void*)graphene_taking;
	p->ongiving = (void*)graphene_giving;
}
