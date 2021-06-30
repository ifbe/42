#include "libuser.h"
#define _tar_ hex32('t','a','r', 0)




static void axis3d_perobj(struct entity* wnd, float* vc, float* vr, float* vf, float* vt)
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
void axis3d_draw_gl41(struct entity* scene, struct entity* wnd)
{
	//say("@axis3d_read: %.8s\n", &scene->fmt);

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
int axis3d_read_bycam(struct entity* ent,void* foot, struct halfrel* stack,int sp, void* arg,int key)
{
	struct halfrel* aa[2];
	int ret = relationsearch(ent, _tar_, &aa[0], &aa[1]);
	if(ret <= 0)return 0;

	struct entity* tar = aa[1]->pchip;
	if(0 == tar)return 0;

	struct entity* wnd = stack[sp-6].pchip;
	if(0 == wnd)return 0;

	axis3d_draw_gl41(tar, wnd);
	return 0;
}




int axis3d_taking(struct entity* ent,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return 0;

	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//foot defined behavior
	switch(stack[sp-1].flag){
	}

	//caller defined behavior
	switch(caller->fmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	}

	return axis3d_read_bycam(ent,foot, stack,sp, arg,key);
}
int axis3d_giving(struct entity* ent,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int axis3d_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int axis3d_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@axis3d_linkup\n");
	return 0;
}




int axis3d_search(struct entity* scene)
{
	return 0;
}
int axis3d_modify(struct entity* scene)
{
	return 0;
}
int axis3d_delete(struct entity* scene)
{
	return 0;
}
int axis3d_create(struct entity* scene, void* str)
{
	say("@axis3d_create\n");
	return 0;
}
