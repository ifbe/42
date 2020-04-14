#include "libuser.h"
#define _in_ hex16('i','n')
#define sin34 0.5591929034707469
#define cos34 0.8290375725550416
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);




static void* eventrts_find(struct entity* ent)
{
	int ret;
	struct halfrel* out[4];

	ret = relationsearch(ent, _evto_, &out[0], &out[1]);
	if(ret <= 0)return 0;

	struct entity* cam = out[1]->pchip;
	if(0 == cam)return 0;

	ret = relationsearch(cam, _in_, &out[0], &out[1]);
	if(ret <= 0)return 0;

	struct fstyle* geom = out[1]->pfoot;
	return geom;
}
static void eventrts_fixgeom(struct fstyle* geom)
{
	geom->vr[0] = 1.0;
	geom->vr[1] = 0.0;
	geom->vr[2] = 0.0;

	geom->vf[0] = 0.0;
	geom->vf[1] = sin34;
	geom->vf[2] =-cos34;

	geom->vt[0] = 0.0;
	geom->vt[1] = cos34;
	geom->vt[2] = sin34;
}




int eventrts_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	return 0;
}
int eventrts_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	struct fstyle* geom = eventrts_find(ent);
	if(0 == geom)return 0;

	struct event* ev = buf;
	if(_char_ == ev->what){
		switch(ev->why){
			case 'a':geom->vc[0] -= 100;break;
			case 'd':geom->vc[0] += 100;break;
			case 's':geom->vc[1] -= 100;break;
			case 'w':geom->vc[1] += 100;break;
			default:return 0;
		}
	}

	if(0x4070 == ev->what){
		//short* t = (void*)ev;
		//say("%d,%d\n", t[0],t[1]);

		vec3 v;
		gl41data_convert(stack[sp-4].pchip, stack[sp-4].pfoot, ev, v);
		say("%f,%f\n", v[0],v[1]);

		if(v[0] < 0.01)geom->vc[0] -= 10.0;
		if(v[0] > 0.99)geom->vc[0] += 10.0;
		if(v[1] < 0.01)geom->vc[1] -= 10.0;
		if(v[1] > 0.99)geom->vc[1] += 10.0;
	}

	eventrts_fixgeom(geom);
	return 0;
}
int eventrts_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@eventrts_discon\n");
	return 0;
}
int eventrts_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@eventrts_linkup\n");
	return 0;
}




int eventrts_search(struct entity* win)
{
	return 0;
}
int eventrts_modify(struct entity* win)
{
	return 0;
}
int eventrts_delete(struct entity* win)
{
	return 0;
}
int eventrts_create(struct entity* act, void* flag)
{
	return 0;
}
