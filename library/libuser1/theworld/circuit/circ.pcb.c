#include "libuser.h"
#define mil (2.54*0.001*0.01)
#define oz  (2.54*0.001*0.01*1.35)
void line2crft(
	float* v0, float* v1, float* vn,	//knwon
	float* vc, float* vr, float* vf)	//wanted
{
	int j;
	for(j=0;j<3;j++){
		vc[j] = (v0[j]+v1[j])/2 + vn[j];
		vr[j] = (v1[j]-v0[j])/2;
	}
	vf[0] =-vn[2]*vr[1];	//vt[1]*vr[2] - vt[2]*vr[1];
	vf[1] = vn[2]*vr[0];	//vt[2]*vr[0] - vt[0]*vr[2];
	vf[2] = 0.0;	//vt[0]*vr[1] - vt[1]*vr[0];
	vec3_setlen(vf, mil*50);
}



/*
static void printboard_dx11draw(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	dx11opaque_prism4(ctx, 0x2000ff00, vc, vr, vf, vt);

	int j;
	vec3 t0,t1;
	vec3 tc,tr,tf,tt;

	for(j=0;j<3;j++){
		t0[j] = vc[j] + vt[j];
		t1[j] = vc[j] + vt[j] + vr[j]/2;
	}
	tt[0] = 0.0;
	tt[1] = 0.0;
	tt[2] = oz/2;
	line2crft(t0,t1,tt, tc,tr,tf);
	dx11solid_prism4(ctx, 0xff0000, tc,tr,tf,tt);

	for(j=0;j<3;j++){
		t0[j] = vc[j] - vt[j];
		t1[j] = vc[j] - vt[j] + vf[j]/2;
	}
	tt[0] = 0.0;
	tt[1] = 0.0;
	tt[2] =-oz/2;
	line2crft(t0,t1,tt, tc,tr,tf);
	dx11solid_prism4(ctx, 0x0000ff, tc,tr,tf,tt);
}*/
static void printboard_gl41draw(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41opaque_prism4(ctx, 0x2000ff00, vc, vr, vf, vt);

	int j;
	vec3 t0,t1;
	vec3 tc,tr,tf,tt;

	for(j=0;j<3;j++){
		t0[j] = vc[j] + vt[j];
		t1[j] = vc[j] + vt[j] + vr[j]/2;
	}
	tt[0] = 0.0;
	tt[1] = 0.0;
	tt[2] = oz/2;
	line2crft(t0,t1,tt, tc,tr,tf);
	gl41solid_prism4(ctx, 0xff0000, tc,tr,tf,tt);

	for(j=0;j<3;j++){
		t0[j] = vc[j] - vt[j];
		t1[j] = vc[j] - vt[j] + vf[j]/2;
	}
	tt[0] = 0.0;
	tt[1] = 0.0;
	tt[2] =-oz/2;
	line2crft(t0,t1,tt, tc,tr,tf);
	gl41solid_prism4(ctx, 0x0000ff, tc,tr,tf,tt);
}




static void printboard_world_camera_window(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(0 == stack)return;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->fmt){
	case _dx11full_:
	case _mt20full_:
	case _gl41full_:
	case _vk12full_:
		printboard_gl41draw(ent,slot, wor,geom, wnd,area);
		break;
	}
}
int printboard_taking(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->fmt){
	case _rgba_:
		break;
	case _gl41full_:
		break;
	default:
		printboard_world_camera_window(ent,slot, stack,sp, arg,key);
		break;
	}
	return 0;
}
int printboard_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int printboard_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int printboard_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@printboard_linkup: %.4s\n", &self->flag);
	return 0;
}




int printboard_search(struct entity* scene)
{
	return 0;
}
int printboard_modify(struct entity* scene)
{
	return 0;
}
int printboard_delete(struct entity* scene)
{
	return 0;
}
int printboard_create(struct entity* scene, void* arg, int argc, u8** argv)
{
	return 0;
}
