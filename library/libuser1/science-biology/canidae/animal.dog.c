#include "libuser.h"
#define _int_ hex32('i','n','t',0)




void robodog_calc(vec3 joint, vec3 vector, vec3 shaft, float rotate)
{
	quaternion_operation(vector, shaft, rotate);
	vector[0] += joint[0];
	vector[1] += joint[1];
	vector[2] += joint[2];
}
void gl41solid_jointandpole(_obj* ctx, u32 rgb, vec3 joint, vec3 target, vec3 shaft)
{
	int j;
	vec3 tc,tr,tf,tu;
	gl41line(ctx, rgb, joint, target);

	for(j=0;j<3;j++)tr[j] = target[j] - joint[j];
	vec3_setlen(tr, 100);

	vec3_cross(tf, tr, shaft);
	vec3_setlen(tf, 100);

	for(j=0;j<3;j++)tu[j] = shaft[j];
	vec3_setlen(tu, 10);

	gl41solid_cylinder(ctx, rgb, joint, tr,tf,tu);


	for(j=0;j<3;j++){
		tc[j] = (target[j]+joint[j])/2;
		tr[j] = (target[j]-joint[j])/2;
	}
	vec3_setlen(tf, 50);
	vec3_setlen(tu, 50);
	gl41solid_prism4(ctx, rgb, tc,tr,tf,tu);
}
void robodog_leg(_obj* ctx, float* f, int lr, vec3 joint, vec3 vector, vec3 shaft)
{
	int j;
	vec3 tc;

	//1:
	robodog_calc(joint, vector, shaft, lr*f[0]);
	gl41solid_jointandpole(ctx, 0x800000, joint, vector, shaft);

	//2: 
	for(j=0;j<3;j++){
		tc[j] = shaft[j];
		shaft[j] = (vector[j] - joint[j])*lr;
		joint[j] = vector[j];
	}
	vec3_cross(vector, shaft, tc);
	vec3_setlen(vector, 500);
	robodog_calc(joint, vector, shaft, f[1]);
	gl41solid_jointandpole(ctx, 0x008000, joint, vector, shaft);

	//3: shaft unchanged
	for(j=0;j<3;j++){
		tc[j] = joint[j];
		joint[j] = vector[j];
		vector[j] = tc[j] - vector[j];
	}
	robodog_calc(joint, vector, shaft, f[2]);
	gl41solid_jointandpole(ctx, 0x000080, joint, vector, shaft);
}




static void robodog_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void robodog_draw_gl41(
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* ctx, struct style* area)
{
	int j;
	float* f;
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_prism4(ctx, 0x404040, vc, vr, vf, vt);
	for(j=0;j<3;j++)tf[j] = vc[j]+vf[j];
	gl41line_arrow(ctx, 0xffffff, vc, tf, vt);

	//body
	for(j=0;j<3;j++){
		tt[j] = vt[j]/8;
		tr[j] = vr[j]*0.5;
		tc[j] = vc[j] +vt[j]-tt[j];
	}
	gl41solid_prism4(ctx, 0x202020, tc, tr, vf, tt);

	f = act->listptr.buf0;
	if(0 == f)return;

	//left, near
	for(j=0;j<3;j++){
		tf[j] = vf[j];
		tc[j] = vc[j] -vr[j]*0.5 -vf[j] +vt[j]*0.875;
		tr[j] = -vr[j]/4;
	}
	robodog_leg(ctx, &f[0], 1, tc,tr,tf);

	//right, near
	for(j=0;j<3;j++){
		tf[j] = vf[j];
		tc[j] = vc[j] +vr[j]*0.5 -vf[j] +vt[j]*0.875;
		tr[j] = vr[j]/4;
	}
	robodog_leg(ctx, &f[3],-1, tc,tr,tf);

	//left, front
	for(j=0;j<3;j++){
		tf[j] = vf[j];
		tc[j] = vc[j] -vr[j]*0.5 +vf[j] +vt[j]*0.875;
		tr[j] = -vr[j]/4;
	}
	robodog_leg(ctx, &f[6], 1, tc,tr,tf);

	//right, front
	for(j=0;j<3;j++){
		tf[j] = vf[j];
		tc[j] = vc[j] +vr[j]*0.5 +vf[j] +vt[j]*0.875;
		tr[j] = vr[j]/4;
	}
	robodog_leg(ctx, &f[9],-1, tc,tr,tf);
}
static void robodog_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void robodog_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void robodog_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void robodog_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




void robodog_write_float(_obj* act, float* src, int len)
{
	int j;
	float* dst = act->listptr.buf0;
	for(j=0;j<12;j++)dst[j] = src[j];
}
void robodog_write_int(_obj* act, int* src, int len)
{
	int j;
	float* dst = act->listptr.buf0;
	for(j=0;j<12;j++)dst[j] = src[j]*PI/50.0;
}




static void robodog_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	robodog_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void robodog_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		break;
	default:
		robodog_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void robodog_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	logtoall("robodog_write\n");

	switch(stack[sp-1].foottype){
	case _int_:
		robodog_write_int(ent,buf,len);
		break;
	default:
		robodog_write_float(ent,buf,len);
	}
}
static void robodog_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void robodog_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void robodog_modify(_obj* act)
{
}
static void robodog_search(_obj* act)
{
}
static void robodog_delete(_obj* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memoryfree(act->buf);
}
static void robodog_create(_obj* act, u8* arg, int argc, u8** argv)
{
	float* f;
	if(0 == act)return;

	f = act->listptr.buf0 = memoryalloc(0x1000, 0);
	f[ 0] = 0.0;
	f[ 1] = PI/12;
	f[ 2] = PI*0.7;
	f[ 3] = 0.0;
	f[ 4] = PI/12;
	f[ 5] = PI*0.7;
	f[ 6] = 0.0;
	f[ 7] = PI/12;
	f[ 8] = PI*0.7;
	f[ 9] = 0.0;
	f[10] = PI/12;
	f[11] = PI*0.7;
}




void robodog_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('r', 'o', 'b', 'o', 'd', 'o', 'g', 0);

	p->oncreate = (void*)robodog_create;
	p->ondelete = (void*)robodog_delete;
	p->onreader = (void*)robodog_search;
	p->onwriter = (void*)robodog_modify;

	p->onattach = (void*)robodog_attach;
	p->ondetach = (void*)robodog_detach;
	p->ontaking = (void*)robodog_taking;
	p->ongiving = (void*)robodog_giving;
}
