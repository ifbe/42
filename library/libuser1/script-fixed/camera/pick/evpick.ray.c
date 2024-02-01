#include "libuser.h"
#define _tar_ hex32('t','a','r', 0)
int areauv2worldxyz(float* v, struct fstyle* sty);
int obb_ray(struct fstyle* sty, vec3 ray[], vec3 out[]);
int gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v);




static int clickray_convert(_obj* cam, struct style* xxx, float* xyz, vec3 ray[])
{
	struct relation* rel;
	_obj* world;
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
	areauv2worldxyz(xyz, &geom->frus);
	//logtoall("%f,%f,%f\n", xyz[0],xyz[1],xyz[2]);

	//put it into cam.rayslot
	cam->whdf.fx0 = xyz[0];
	cam->whdf.fy0 = xyz[1];
	cam->whdf.fz0 = xyz[2];

xyz2ray:
	ray[0][0] = geom->fs.vc[0];
	ray[0][1] = geom->fs.vc[1];
	ray[0][2] = geom->fs.vc[2];
	ray[1][0] = xyz[0] - ray[0][0];
	ray[1][1] = xyz[1] - ray[0][1];
	ray[1][2] = xyz[2] - ray[0][2];
	return 1;
}
static int clickray_intersect(_obj* handler,void* foot,
	struct halfrel* stack,int sp, vec3 ray[], vec3 out[])
{
	int ret;
	struct halfrel* totar[2];
	ret = relationsearch(handler, _tar_, &totar[0], &totar[1]);
	if(ret <= 0)return 0;
//logtoall("(%f,%f,%f)->(%f,%f,%f)\n", ray[0][0],ray[0][1],ray[0][2], ray[1][0],ray[1][1],ray[1][2]);

	_obj* scene = totar[1]->pchip;
	if(0 == scene)return 0;

	struct relation* rel = scene->orel0;
	while(1){
		if(0 == rel)break;

		ret = obb_ray(rel->psrcfoot, ray, out);
		if(ret){
			//tell that world
			stack[sp+0].pchip = totar[0]->pchip;
			stack[sp+0].pfoot = totar[0]->pfoot;
			stack[sp+0].foottype = totar[0]->foottype;
			stack[sp+1].pchip = totar[1]->pchip;
			stack[sp+1].pfoot = totar[1]->pfoot;
			stack[sp+1].foottype = totar[1]->foottype;
			entity_giveby(stack[sp+1].pchip, stack[sp+1].pfoot, stack,sp+2, (p64)rel, 0, ray, 0);
			return 1;
		}
		rel = samesrcnextdst(rel);
	}

	logtoall("clickray_send: miss\n");
	entity_giveby(stack[sp+1].pchip, stack[sp+1].pfoot, stack,sp+2, 0, 0, ray, 0);
	return 0;
}




int clickray_search(_obj* act, int flag, struct halfrel* self[], struct halfrel* peer[])
{
	return 0;
}
int clickray_modify(_obj* act)
{
	return 0;
}
int clickray_delete(_obj* act)
{
	return 0;
}
int clickray_create(_obj* act, void* flag)
{
	act->whdf.ix0 = 0;
	act->whdf.iy0 = 0;
	act->whdf.iz0 = 0;
	act->whdf.iw0 = 0;
	return 0;
}




int clickray_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
	return 0;
}
int clickray_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
//[-4,-3]: wnd,area -> cam,togl
//[-2,-1]: cam,evto -> this,bycam
	_obj* cam = stack[sp-2].pchip;
	struct style* xxxx = stack[sp-2].pfoot;
	_obj* wnd = stack[sp-4].pchip;
	struct style* area = stack[sp-4].pfoot;
logtoall("%.8s->%.8s->%.8s\n",&wnd->type, &cam->type, &ent->type);
/*	switch(wnd->type){
	case _gl41list_:
	case _dx11list_:
	case _mt20list_:
	}
*/
	struct event* ev = buf;
	if(0x2d70 == ev->what){		//mouse up
		ent->whdf.iw0 = 0;
		return 0;
	}
	if(0x2b70 == ev->what){		//mouse down
		ent->whdf.iw0 = 1;
	}
	if('p' == (ev->what&0xff)){
		if(0 == ent->whdf.iw0)return 0;

		int ret;
		vec3 xyz;
		vec3 ray[2];
		vec3 out[2];

		//screen to ndc
		ret = gl41data_convert(wnd, area, ev, xyz);
		//logtoall("%f,%f\n",xyz[0],xyz[1]);

		//ndc to ray
		ret = clickray_convert(cam, xxxx, xyz, ray);
		if(ret <= 0)return 0;

		//intersect test
		ret = clickray_intersect(ent,foot, stack,sp, ray,out);
		if(ret <= 0)return 0;
	}
	return 0;
}
int clickray_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int clickray_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
