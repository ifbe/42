#include "libuser.h"




static void rocket_forgl41_actual(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;

    vec4 tc,tt;
    int j;
    for(j=0;j<3;j++){
        tc[j] = vc[j]+vt[j]/2;
        tt[j] = vt[j]/2;
    }
    gl41solid_cylinder(ctx, 0xffffff, tc,vr,vf,tt);

    for(j=0;j<3;j++){
        tc[j] = vc[j]-vt[j];
    }
    gl41solid_cone(ctx, 0xffff00, tc,vr,vt);
}
static void rocket_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void rocket_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void rocket_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void rocket_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void rocket_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void rocket_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor = stack[sp-2].pchip;
	struct style* geom = stack[sp-2].pfoot;
	_obj* wnd = stack[sp-6].pchip;
	struct style* area = stack[sp-6].pfoot;
	rocket_forgl41_actual(ent,slot, wor,geom, wnd,area);
}




static void rocket_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
		break;
	default:
		rocket_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void rocket_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void rocket_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void rocket_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void rocket_modify(_obj* act)
{
}
static void rocket_search(_obj* act)
{
}
static void rocket_delete(_obj* act)
{
}
static void rocket_create(_obj* act)
{
}




void rocket_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('r', 'o', 'c', 'k', 'e', 't', 0, 0);

	p->oncreate = (void*)rocket_create;
	p->ondelete = (void*)rocket_delete;
	p->onreader = (void*)rocket_search;
	p->onwriter = (void*)rocket_modify;

	p->onattach = (void*)rocket_attach;
	p->ondetach = (void*)rocket_detach;
	p->ontaking = (void*)rocket_taking;
	p->ongiving = (void*)rocket_giving;
}
