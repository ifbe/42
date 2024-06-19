#include "libuser.h"




static void station_draw_gl41(
	_obj* act, struct style* part,		//self
	_obj* win, struct style* geom,		//world,mygeom
	_obj* ctx, struct style* none)		//gldata
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_prism4(ctx, 0xffff00, vc, vr, vf, vt);

    vec4 tc,tr,tf,tt;
    int j;

    //right
    for(j=0;j<3;j++){
        tc[j] = vc[j]+vr[j]/2;
        tr[j] =-vf[j];
        tf[j] =-vt[j];
        tt[j] = vr[j] * 0.49;
    }
    vec3_setlen(tr, 0.005);
    vec3_setlen(tf, 0.005);
    gl41solid_cylinder(ctx, 0xffffff, tc,tr,tf,tt);

    //left
    for(j=0;j<3;j++){
        tc[j] = vc[j]-vr[j]/2;
    }
    gl41solid_cylinder(ctx, 0xffffff, tc,tr,tf,tt);

    //to north
    for(j=0;j<3;j++){
        tc[j] = vc[j]+vf[j]/2;
        tr[j] = vr[j];
        tf[j] =-vt[j];
        tt[j] = vf[j] * 0.49;
    }
    vec3_setlen(tr, 0.005);
    vec3_setlen(tf, 0.005);
    gl41solid_cylinder(ctx, 0xffffff, tc,tr,tf,tt);

    //to earth
    for(j=0;j<3;j++){
        tc[j] = vc[j]-vt[j]/2;
        tr[j] = vr[j];
        tf[j] = vf[j];
        tt[j] =-vt[j]*0.49;
    }
    vec3_setlen(tr, 0.005);
    vec3_setlen(tf, 0.005);
    gl41solid_cylinder(ctx, 0xffffff, tc,tr,tf,tt);

}




static void station_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void station_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void station_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void station_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void station_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	logtoall("station(%x,%x,%x)\n",win,act,sty);
}
static void station_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	struct event* ev, int len)
{
}




static void station_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	station_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void station_read_bywnd(_obj* ent,void* foot, _syn* stack,int sp)
{
}




static void station_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
	case _render_:
		station_read_bywnd(ent,foot, stack,sp);
		break;
	default:
		station_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void station_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void station_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void station_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@station_attach\n");
}




static void station_search(_obj* act)
{
}
static void station_modify(_obj* act)
{
}
static void station_delete(_obj* act)
{
	if(0 == act)return;
}
static void station_create(_obj* act, void* str)
{
	if(0 == act)return;
}




void station_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('s','t','a','t','i','o','n', 0);

	p->oncreate = (void*)station_create;
	p->ondelete = (void*)station_delete;
	p->onreader = (void*)station_search;
	p->onwriter = (void*)station_modify;

	p->onattach = (void*)station_attach;
	p->ondetach = (void*)station_detach;
	p->ontaking = (void*)station_taking;
	p->ongiving = (void*)station_giving;
}
