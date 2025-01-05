#include "libuser.h"




static void station_draw_gl41(
	_obj* act, struct style* part,		//self
	_obj* win, struct style* geom,		//world,mygeom
	_obj* ctx, struct style* none)		//gldata
{
	struct fmotion* physic = &geom->actual;
	float* q = physic->angular_x;

	vec3 vr;
	vr[0] = (1.0 - (q[1]*q[1] + q[2]*q[2]) * 2.0);
	vr[1] = (2.0 * (q[0]*q[1] + q[2]*q[3]));
	vr[2] = (2.0 * (q[0]*q[2] - q[1]*q[3]));
	vec3_setlen(vr, vec3_getlen(geom->fshape.vr));

	vec3 vf;
	vf[0] = (2.0 * (q[0]*q[1] - q[2]*q[3]));
	vf[1] = (1.0 - (q[0]*q[0] + q[2]*q[2]) * 2.0);
	vf[2] = (2.0 * (q[1]*q[2] + q[0]*q[3]));
	vec3_setlen(vf, vec3_getlen(geom->fshape.vf));

	vec3 vt;
	vt[0] = (2.0 * (q[0]*q[2] + q[1]*q[3]));
	vt[1] = (2.0 * (q[1]*q[2] - q[0]*q[3]));
	vt[2] = (1.0 - (q[0]*q[0] + q[1]*q[1]) * 2.0);
	vec3_setlen(vt, vec3_getlen(geom->fshape.vt));

	vec3 vc;
	vc[0] = physic->displace_x[0];
	vc[1] = physic->displace_x[1];
	vc[2] = physic->displace_x[2];
/*
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
*/
	//logtoall("(%f,%f,%f)(%f,%f,%f)(%f,%f,%f)(%f,%f,%f)\n", vc[0],vc[1],vc[2], vr[0],vr[1],vr[2], vf[0],vf[1],vf[2], vt[0],vt[1],vt[2]);
	gl41line_prism4(ctx, 0xffff00, vc, vr, vf, vt);

    int j;
    vec4 tc,tr,tf,tt;

	//core
	float lent3 = vec3_getlen(vt)/3;
	for(j=0;j<3;j++){
		tr[j] = vr[j];
		tf[j] = vf[j];
		tt[j] = vt[j];
	}
	vec3_setlen(tr, lent3);
	vec3_setlen(tf, lent3);
	vec3_setlen(tt, lent3);
	gl41solid_sphere(ctx, 0xffffff, vc, tr, tf, tt);

    //right
    for(j=0;j<3;j++){
        tr[j] =-vf[j];
        tf[j] =-vt[j];
        tt[j] = vr[j] * 0.46;
        tc[j] = vc[j]+vr[j]-tt[j];
    }
    vec3_setlen(tr, 0.005);
    vec3_setlen(tf, 0.005);
    gl41solid_cylinder(ctx, 0xffffff, tc,tr,tf,tt);
	//right.solarpanel
    for(j=0;j<3;j++){
        tr[j] = vr[j]*0.1;
        tf[j] = vf[j]*0.45;
        tc[j] = vc[j] +vr[j]-tr[j] +vf[j]-tf[j];
    }
	gl41solid_rect(ctx, 0xffffff, tc,tr,tf);
    for(j=0;j<3;j++){
        tc[j] = vc[j] +vr[j]-tr[j] -vf[j]+tf[j];
    }
	gl41solid_rect(ctx, 0xffffff, tc,tr,tf);

    //left
    for(j=0;j<3;j++){
        tr[j] =-vf[j];
        tf[j] =-vt[j];
        tt[j] = vr[j] * 0.46;
        tc[j] = vc[j]-vr[j]+tt[j];
    }
    vec3_setlen(tr, 0.005);
    vec3_setlen(tf, 0.005);
    gl41solid_cylinder(ctx, 0xffffff, tc,tr,tf,tt);
	//left.solarpanel
    for(j=0;j<3;j++){
        tr[j] = vr[j]*0.1;
        tf[j] = vf[j]*0.45;
        tc[j] = vc[j] -vr[j]+tr[j] +vf[j]-tf[j];
    }
	gl41solid_rect(ctx, 0xffffff, tc,tr,tf);
    for(j=0;j<3;j++){
        tc[j] = vc[j] -vr[j]+tr[j] -vf[j]+tf[j];
    }
	gl41solid_rect(ctx, 0xffffff, tc,tr,tf);

    //to north
    for(j=0;j<3;j++){
        tr[j] = vr[j];
        tf[j] =-vt[j];
        tt[j] = vf[j] * 0.46;
        tc[j] = vc[j]+vf[j]-tt[j];
    }
    vec3_setlen(tr, 0.005);
    vec3_setlen(tf, 0.005);
    gl41solid_cylinder(ctx, 0xffffff, tc,tr,tf,tt);

    //to earth
    for(j=0;j<3;j++){
        tr[j] = vr[j];
        tf[j] = vf[j];
        tt[j] = vt[j]*0.40;
        tc[j] = vc[j]-vt[j]+tt[j];
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
