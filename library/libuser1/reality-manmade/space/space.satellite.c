#include "libuser.h"
#define longitude whdf.fx0
#define latitude whdf.fy0
#define altitude whdf.fz0




static void satellite_draw_gl41(
	_obj* act, struct style* part,		//self
	_obj* win, struct style* geom,		//world,mygeom
	_obj* ctx, struct style* none)		//gldata
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	//gl41line_sphere(ctx, 0xffffff, vc,vr,vf,vt);

	vec3 tc,tt,tr,tf;
	int j;
	float a0,c0,s0;
	float az,cz,sz;
	a0 = (act->longitude+180) * PI / 180;
	c0 = getcos(a0);
	s0 = getsin(a0);
	az = act->latitude * PI / 180;
	cz = getcos(az);
	sz = getsin(az);
	for(j=0;j<3;j++){
		tt[j] = (vr[j]*c0 + vf[j]*s0)*cz + vt[j]*sz;
		tc[j] = vc[j] + tt[j];
		tr[j] = -vr[j]*s0 + vf[j]*c0;
	}
	gl41line(ctx, 0xff00ff, vc, tc);

	float len = vec3_getlen(vr)/100;
	vec3_cross(tf, tt, tr);
	vec3_setlen(tr, len);
	vec3_setlen(tf, len);
	gl41solid_rect(ctx, 0xffff00, tc, tr, tf);
}




static void satellite_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void satellite_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void satellite_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void satellite_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void satellite_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	say("satellite(%x,%x,%x)\n",win,act,sty);
}
static void satellite_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	struct event* ev, int len)
{
}




static void satellite_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	satellite_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void satellite_wnd(_obj* ent,void* foot, _syn* stack,int sp)
{
}




static void satellite_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		satellite_wnd(ent,foot, stack,sp);
		break;
	default:
		satellite_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void satellite_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	float* f = buf;
	say("@satellite_write: %f,%f,%f,%f\n", f[0],f[1],f[2],f[3]);

	ent->longitude= f[0];
	ent->latitude = f[1];
	ent->altitude = f[2];
}
static void satellite_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void satellite_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@satellite_attach\n");
}




static void satellite_search(_obj* act)
{
}
static void satellite_modify(_obj* act)
{
}
static void satellite_delete(_obj* act)
{
	if(0 == act)return;
}
static void satellite_create(_obj* act, void* str)
{
	if(0 == act)return;

	act->longitude = 0.0;
	act->latitude = 0.0;
	act->altitude = 0.0;
}




void satellite_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('s','a','t','e','l','l','i', 0);

	p->oncreate = (void*)satellite_create;
	p->ondelete = (void*)satellite_delete;
	p->onreader = (void*)satellite_search;
	p->onwriter = (void*)satellite_modify;

	p->onattach = (void*)satellite_attach;
	p->ondetach = (void*)satellite_detach;
	p->ontaking = (void*)satellite_taking;
	p->ongiving = (void*)satellite_giving;
}
