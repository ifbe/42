#include "libuser.h"
#define _tar_ hex32('t','a','r', 0)
int worldxyzfromareauv(float* v, struct fstyle* sty);
int obb_ray(struct fstyle* sty, vec3 ray[], vec3 out[]);
int gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);




static int clickray_convert(struct entity* cam, struct style* xxx, float* xyz, vec3 ray[])
{
	struct relation* rel;
	struct entity* world;
	struct style* geom;

	rel = cam->irel0;
	while(1){
		if(0 == rel)return 0;
		world = rel->psrcchip;
		if( (_virtual_ == world->type) | (_scene3d_ == world->type) ){
			geom = rel->psrcfoot;
			goto found;
		}
		rel = samedstnextsrc(rel);
	}
	return 0;

found:
	//xyz(world) from xy(screen) and frustum
	worldxyzfromareauv(xyz, &geom->frus);
	//say("%f,%f,%f\n", xyz[0],xyz[1],xyz[2]);

	//put it into cam.rayslot
	cam->fx0 = xyz[0];
	cam->fy0 = xyz[1];
	cam->fz0 = xyz[2];

xyz2ray:
	ray[0][0] = geom->fs.vc[0];
	ray[0][1] = geom->fs.vc[1];
	ray[0][2] = geom->fs.vc[2];
	ray[1][0] = xyz[0] - ray[0][0];
	ray[1][1] = xyz[1] - ray[0][1];
	ray[1][2] = xyz[2] - ray[0][2];
	return 1;
}
static int clickray_intersect(struct entity* handler,int foot,
	struct halfrel* stack,int sp, vec3 ray[], vec3 out[])
{
	int ret;
	struct halfrel* junk[2];
	ret = relationsearch(handler, _tar_, &junk[0], &junk[1]);
	if(ret <= 0)return 0;
//say("%f,%f,%f->%f,%f,%f\n", ray[0][0],ray[0][1],ray[0][2], ray[1][0],ray[1][1],ray[1][2]);

	struct entity* scene = junk[1]->pchip;
	if(0 == scene)return 0;

	struct relation* rel = scene->orel0;
	while(1){
		if(0 == rel)break;

		ret = obb_ray(rel->psrcfoot, ray, out);
		if(ret){
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+0].flag = rel->srcflag;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			stack[sp+1].flag = rel->dstflag;
			return 1;
		}
		rel = samesrcnextdst(rel);
	}

	say("clickray_send: miss\n");
	return 0;
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




int clickray_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	return 0;
}
int clickray_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	struct event* ev = buf;
	if(0x2d70 == ev->what){		//mouse up
		ent->iw0 = 0;
		return 0;
	}
	if(0x2b70 == ev->what){		//mouse down
		ent->iw0 = 1;
	}
	if('p' == (ev->what&0xff)){
		if(0 == ent->iw0)return 0;

//[-4,-3]: wnd,area -> cam,togl
//[-2,-1]: cam,evto -> this,bycam
		int ret;
		vec3 xyz;
		vec3 ray[2];
		vec3 out[2];
		struct entity* cam = stack[sp-2].pchip;
		struct style* xxxx = stack[sp-2].pfoot;
		struct entity* wnd = stack[sp-4].pchip;
		struct style* area = stack[sp-4].pfoot;
//say("%.8s,%.8s,%.8s\n",&wnd->type, &act->type, &wrd->type);

		//screen to ndc
		ret = gl41data_convert(wnd, area, ev, xyz);
		//say("%f,%f\n",xyz[0],xyz[1]);

		//ndc to ray
		ret = clickray_convert(cam, xxxx, xyz, ray);
		if(ret <= 0)return 0;

		//intersect test
		ret = clickray_intersect(ent,foot, stack,sp, ray,out);
		if(ret <= 0)return 0;

		//send
		entitywrite(stack[sp+1].pchip, stack[sp+1].flag, stack,sp+2, stack[sp].pfoot, 0, ray, 0);
	}
	return 0;
}
int clickray_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int clickray_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
