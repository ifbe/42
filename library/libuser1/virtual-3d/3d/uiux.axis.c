#include "libuser.h"
#define _tar_ hex32('t','a','r', 0)




static void axis3d_perobj(_obj* wnd, float* vc, float* vr, float* vf, float* vt)
{
	int j;
	vec3 tr,tf,tt;
	vec3 nr,nf,nt;

	float lr = vec3_getlen(vr);
	float lf = vec3_getlen(vf);
	float lt = vec3_getlen(vt);
	for(j=0;j<3;j++){
		tr[j] = vc[j] + vr[j];
		tf[j] = vc[j] + vf[j];
		tt[j] = vc[j] + vt[j];

		nr[j] = vr[j]*32/lr;
		nf[j] = vf[j]*32/lf;
		nt[j] = vt[j]*32/lt;
	}

	//right
	gl41line(wnd, 0xff0000, vc, tr);
	gl41float(wnd, 0xff0000, vc, nr, nf, lr);

	//front
	gl41line(wnd, 0x00ff00, vc, tf);
	gl41float(wnd, 0xff0000, vc, nf, nt, lf);

	//above
	gl41line(wnd, 0x0000ff, vc, tt);
	gl41float(wnd, 0xff0000, vc, nt, nr, lt);
}
void axis3d_draw_gl41(_obj* scene, _obj* wnd)
{
	//logtoall("@axis3d_read: %.8s\n", &scene->type);

	vec3 tc,tr,tf,tt;
	tc[0] = tc[1] = tc[2] = 0.0;
	tr[0] = 1000000.0;tr[1] = 0.0;tr[2] = 0.0;
	tf[0] = 0.0;tf[1] = 1000000.0;tf[2] = 0.0;
	tt[0] = 0.0;tt[1] = 0.0;tt[2] = 1000000.0;
	axis3d_perobj(wnd, tc, tr, tf, tt);

	struct fstyle* sty;
	struct relation* rel = scene->orel0;
	while(1){
		if(0 == rel)break;

		sty = rel->psrcfoot;
		if(sty)axis3d_perobj(wnd, sty->vc, sty->vr, sty->vf, sty->vt);

		rel = samesrcnextdst(rel);
	}
}




static void axis3d_read_bywnd(_obj* ent,void* slot, _obj* wnd,void* area)
{
	switch(wnd->vfmt){
	case _rgba8888_:
		break;
	case _gl41list_:
		break;
	}
}
int axis3d_read_bycam(_obj* ent,void* foot, struct halfrel* stack,int sp, p64 arg,int key)
{
	struct halfrel* aa[2];
	int ret = relationsearch(ent, _tar_, &aa[0], &aa[1]);
	if(ret <= 0)return 0;

	_obj* tar = aa[1]->pchip;
	if(0 == tar)return 0;

	_obj* wnd = stack[sp-6].pchip;
	if(0 == wnd)return 0;

	axis3d_draw_gl41(tar, wnd);
	return 0;
}




int axis3d_taking(_obj* ent,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return 0;

	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	switch(caller->type){
	case _wnd_:
	case _render_:
		axis3d_read_bywnd(ent,foot, caller,area);
		break;
	default:
		axis3d_read_bycam(ent,foot, stack,sp, arg,key);
		break;
	}
	return 0;
}
int axis3d_giving(_obj* ent,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	return 0;
}
int axis3d_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int axis3d_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@axis3d_attach\n");
	return 0;
}




int axis3d_search(_obj* scene)
{
	return 0;
}
int axis3d_modify(_obj* scene)
{
	return 0;
}
int axis3d_delete(_obj* scene)
{
	return 0;
}
int axis3d_create(_obj* scene, void* str)
{
	logtoall("@axis3d_create\n");
	return 0;
}
