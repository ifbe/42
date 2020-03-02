#include "libuser.h"
#define sin34 0.5591929034707469
#define cos34 0.8290375725550416
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);




static void eventrts_fixgeom(struct fstyle* cam)
{
	cam->vr[0] = 1.0;
	cam->vr[1] = 0.0;
	cam->vr[2] = 0.0;

	cam->vf[0] = 0.0;
	cam->vf[1] = sin34;
	cam->vf[2] =-cos34;

	cam->vt[0] = 0.0;
	cam->vt[1] = cos34;
	cam->vt[2] = sin34;
}




int eventrts_read(struct halfrel* self, struct halfrel* peer, void* arg, int rsp, void* buf, int len)
{
	return 0;
}
int eventrts_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, struct event* ev, int len)
{
	struct entity* ent = self->pchip;
	if(0 == ent)return 0;

	struct fstyle* cam = stack[rsp-1]->pfoot;
	if(0 == cam)return 0;

	if(_char_ == ev->what){
		switch(ev->why){
			case 'a':cam->vc[0] -= 100;break;
			case 'd':cam->vc[0] += 100;break;
			case 's':cam->vc[1] -= 100;break;
			case 'w':cam->vc[1] += 100;break;
			default:return 0;
		}
	}
	if(0x4070 == ev->what){
		//short* t = (void*)ev;
		//say("%d,%d\n", t[0],t[1]);

		vec3 v;
		gl41data_convert(stack[rsp-4]->pchip, stack[rsp-4]->pfoot, ev, v);
		say("%f,%f\n", v[0],v[1]);

		if(v[0] < 0.01)cam->vc[0] -= 10.0;
		if(v[0] > 0.99)cam->vc[0] += 10.0;
		if(v[1] < 0.01)cam->vc[1] -= 10.0;
		if(v[1] > 0.99)cam->vc[1] += 10.0;
	}

	eventrts_fixgeom(cam);
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
