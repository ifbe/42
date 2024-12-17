#include "libuser.h"




static void rocket_forgl41_actual(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* ctx, struct style* area)
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

	//float* vc = geom->fs.vc;
	//float* vr = geom->fs.vr;
	//float* vf = geom->fs.vf;
	//float* vt = geom->fs.vt;
	//logtoall("(%f,%f,%f)(%f,%f,%f)(%f,%f,%f)(%f,%f,%f)\n", vc[0],vc[1],vc[2], vr[0],vr[1],vr[2], vf[0],vf[1],vf[2], vt[0],vt[1],vt[2]);

    int j;
    vec4 tc,tt;
	for(j=0;j<3;j++){
		tc[j] = vc[j]-vf[j];
		tt[j] = tc[j]+vt[j];
	}
	gl41line(ctx, 0xff, tc,tt);

    for(j=0;j<3;j++){
        tc[j] = vc[j]+vt[j]/2;
        tt[j] = vt[j]/2;
    }
    gl41solid_cylinder(ctx, 0xffffff, tc,vr,vf,tt);

    for(j=0;j<3;j++){
        tc[j] = vc[j]-vt[j];
    }
    gl41solid_cone(ctx, 0xffff00, tc,vr,vt);


	struct forceinfo* fi = &geom->forceinfo;
	int k;
	for(k=0;k<fi->cntlast;k++){
		for(j=0;j<3;j++){
			tc[j] = fi->where[k][j];
			tt[j] = fi->where[k][j] + fi->force[k][j];
		}
		gl41line(ctx, 0xffffff, tc, tt);
	}
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
	case _render_:
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
