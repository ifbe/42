#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void motor_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void motor_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	float a,c,s;
	vec3 v1,v2,v3;
	vec3 tc,tr,tf,tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41line_circle(ctx, 0xffffff, vc, vr, vf);

	tr[0] = vr[0]/2;
	tr[1] = vr[1]/2;
	tr[2] = vr[2]/2;
	tf[0] = vf[0]/2;
	tf[1] = vf[1]/2;
	tf[2] = vf[2]/2;
	tu[0] = vu[0]/2;
	tu[1] = vu[1]/2;
	tu[2] = vu[2]/2;
	gl41line_circle(ctx, 0xffffff, vc, tr, tf);

	tr[0] = vr[0]/4;
	tr[1] = vr[1]/4;
	tr[2] = vr[2]/4;
	tc[0] = vc[0] - vf[0]/2;
	tc[1] = vc[1] - vf[1]/2;
	tc[2] = vc[2] - vf[2]/2;
	gl41solid_rect(ctx, 0x0000ff, tc, tr, tu);
	tc[0] = vc[0] + vf[0]/2;
	tc[1] = vc[1] + vf[1]/2;
	tc[2] = vc[2] + vf[2]/2;
	gl41solid_rect(ctx, 0x0000ff, tc, tr, tu);

	tr[0] = vr[0]/8 + vf[0]*1.732/8;
	tr[1] = vr[1]/8 + vf[1]*1.732/8;
	tr[2] = vr[2]/8 + vf[2]*1.732/8;
	tc[0] = vc[0] + vr[0]*1.732/4 - vf[0]*0.25;
	tc[1] = vc[1] + vr[1]*1.732/4 - vf[1]*0.25;
	tc[2] = vc[2] + vr[2]*1.732/4 - vf[2]*0.25;
	gl41solid_rect(ctx, 0x00ff00, tc, tr, tu);
	tc[0] = vc[0] - vr[0]*1.732/4 + vf[0]*0.25;
	tc[1] = vc[1] - vr[1]*1.732/4 + vf[1]*0.25;
	tc[2] = vc[2] - vr[2]*1.732/4 + vf[2]*0.25;
	gl41solid_rect(ctx, 0x00ff00, tc, tr, tu);

	tr[0] = vr[0]/8 - vf[0]*1.732/8;
	tr[1] = vr[1]/8 - vf[1]*1.732/8;
	tr[2] = vr[2]/8 - vf[2]*1.732/8;
	tc[0] = vc[0] - vr[0]*1.732/4 - vf[0]*0.25;
	tc[1] = vc[1] - vr[1]*1.732/4 - vf[1]*0.25;
	tc[2] = vc[2] - vr[2]*1.732/4 - vf[2]*0.25;
	gl41solid_rect(ctx, 0xff0000, tc, tr, tu);
	tc[0] = vc[0] + vr[0]*1.732/4 + vf[0]*0.25;
	tc[1] = vc[1] + vr[1]*1.732/4 + vf[1]*0.25;
	tc[2] = vc[2] + vr[2]*1.732/4 + vf[2]*0.25;
	gl41solid_rect(ctx, 0xff0000, tc, tr, tu);




	a = timeread() / 1000000.0;

	s = getsin(a);
	v1[0] = vc[0] + (vf[0]/2)*s;
	v1[1] = vc[1] + (vf[1]/2)*s;
	v1[2] = vc[2] + (vf[2]/2)*s;
	gl41line(ctx, 0x0000ff, vc, v1);

	s = getsin(a+PI*2/3);
	v2[0] = vc[0] + (vr[0]*1.732/4 - vf[0]*0.25)*s;
	v2[1] = vc[1] + (vr[1]*1.732/4 - vf[1]*0.25)*s;
	v2[2] = vc[2] + (vr[2]*1.732/4 - vf[2]*0.25)*s;
	gl41line(ctx, 0x00ff00, vc, v2);

	s = getsin(a+PI*4/3);
	v3[0] = vc[0] - (vr[0]*1.732/4 + vf[0]*0.25)*s;
	v3[1] = vc[1] - (vr[1]*1.732/4 + vf[1]*0.25)*s;
	v3[2] = vc[2] - (vr[2]*1.732/4 + vf[2]*0.25)*s;
	gl41line(ctx, 0xff0000, vc, v3);

	tc[0] = v1[0] + v2[0] + v3[0] - vc[0]*2;
	tc[1] = v1[1] + v2[1] + v3[1] - vc[1]*2;
	tc[2] = v1[2] + v2[2] + v3[2] - vc[2]*2;
	gl41line(ctx, 0xffffff, vc, tc);

}
static void motor_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void motor_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void motor_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void motor_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void motor_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	motor_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void motor_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
		break;
	default:
		motor_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void motor_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void motor_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void motor_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void motor_search(_obj* act)
{
}
static void motor_modify(_obj* act)
{
}
static void motor_delete(_obj* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void motor_create(_obj* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void motor_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('m', 'o', 't', 'o', 'r', 0, 0, 0);

	p->oncreate = (void*)motor_create;
	p->ondelete = (void*)motor_delete;
	p->onsearch = (void*)motor_search;
	p->onmodify = (void*)motor_modify;

	p->onattach = (void*)motor_attach;
	p->ondetach = (void*)motor_detach;
	p->ontaking = (void*)motor_taking;
	p->ongiving = (void*)motor_giving;
}
