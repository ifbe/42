#include "libuser.h"
#define _tar_ hex32('t','a','r', 0)
int invmvp(float* v, struct fstyle* sty);
int obb_ray(struct fstyle* sty, vec3 ray[], vec3 out[]);
int gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);
int relation_search(void* item, u32 foottype, struct halfrel* self[], struct halfrel* peer[]);




static int clickray_send(struct entity* handler, vec3 ray[])
{
	int ret;
	struct halfrel* self[2];
	struct halfrel* peer[2];
	ret = relation_search(handler, _tar_, self, peer);
//say("clickray_send:%d\n", ret);
	if(ret <= 0)return 0;

	vec3 out[2];
	struct style* sty;
	struct entity* scene = peer[0]->pchip;
	struct relation* rel = scene->orel0;
	while(1){
		if(0 == rel)break;
		sty = rel->psrcfoot;
		ret = obb_ray(&sty->fs, ray, out);
		if(ret){
			entitywrite((void*)(rel->dst), (void*)(rel->src), sty, 0, ray, 0);
			return 1;
		}
		rel = samesrcnextdst(rel);
	}

	say("clickray_send: miss\n");
	return 0;
}
static int clickray_convert(
	struct entity* cam, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* wnd, struct style* area,
	struct event* ev, float* xyz)
{
	//screen to ndc
	gl41data_convert(wnd, area, ev, xyz);
	xyz[0] = 2*xyz[0] - 1.0;
	xyz[1] = 2*xyz[1] - 1.0;
	xyz[2] = 0.999999;
	//say("%f,%f\n", dr[0],dr[1]);

	//ndc to world
	invmvp(xyz, &geom->frus);
	cam->fx0 = xyz[0];
	cam->fy0 = xyz[1];
	cam->fz0 = xyz[2];
	//say("%f,%f,%f\n", xyz[0],xyz[1],xyz[2]);
	return 1;
}




int clickray_search(struct entity* act, int flag, struct halfrel* self[], struct halfrel* peer[])
{
	return 0;
}
int clickray_modify(struct entity* act)
{
	return 0;
}
int clickray_delete(struct entity* act)
{
	return 0;
}
int clickray_create(struct entity* act, void* flag)
{
	act->ix0 = 0;
	act->iy0 = 0;
	act->iz0 = 0;
	act->iw0 = 0;
	return 0;
}




int clickray_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	return 0;
}
int clickray_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, struct event* ev, int len)
{
	struct entity* ent = self->pchip;
	if(0 == ent)return 0;

	if(0x2d70 == ev->what){		//mouse up
		ent->iw0 = 0;
		return 0;
	}
	if(0x2b70 == ev->what){		//mouse down
		ent->iw0 = 1;
	}
	if('p' == (ev->what&0xff)){
		if(0 == ent->iw0)return 0;

		int ret;
		vec3 ray[2];
		struct entity* wrd = stack[rsp-1]->pchip;
		struct style* geom = stack[rsp-1]->pfoot;
		struct entity* act = stack[rsp-2]->pchip;
		struct style* slot = stack[rsp-2]->pfoot;
		//struct entity* cam = stack[rsp-3]->pchip;
		//struct style* gl41 = stack[rsp-3]->pfoot;
		struct entity* wnd = stack[rsp-4]->pchip;
		struct style* area = stack[rsp-4]->pfoot;
		ret = clickray_convert(act,slot, wrd,geom, wnd,area, ev,ray[1]);
		if(ret <= 0)return 0;

		ray[0][0] = geom->fs.vc[0];
		ray[0][1] = geom->fs.vc[1];
		ray[0][2] = geom->fs.vc[2];
		clickray_send(self->pchip, ray);
	}
	return 0;
}
int clickray_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int clickray_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}