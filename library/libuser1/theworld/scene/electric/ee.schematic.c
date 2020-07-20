#include "libuser.h"






static void schematic_dx11draw(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	dx11opaque_rect(ctx, 0x20ffff00, vc, vr, vf);
}
static void schematic_gl41draw(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41opaque_rect(ctx, 0x20ffff00, vc, vr, vf);

	vec3 tc,tr,tf;
	float x,y,z=0.01;
	tr[0] = 0.0005;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 0.0005;
	tf[2] = 0.0;
	for(y=-0.1;y<0.1;y+=0.01){
		for(x=-0.1;x<0.1;x+=0.01){
			tc[0] = vc[0] + x;
			tc[1] = vc[1] + y;
			tc[2] = vc[2] + z;
			gl41solid_rect(ctx, 0, tc,tr,tf);
		}
	}
}




static void schematic_camread(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(0 == stack)return;
	if('v' != key)return;

	slot = stack[sp-1].pfoot;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->fmt){
	case _dx11full_:schematic_dx11draw(ent,slot, wor,geom, wnd,area);break;
	case _gl41full_:schematic_gl41draw(ent,slot, wor,geom, wnd,area);break;
	}
}
int schematic_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot = stack[sp-1].pfoot;
	struct entity* wnd;struct style* area;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(wnd->fmt){
	default:schematic_camread(ent,foot, stack,sp, arg,key);break;
	}
	return 0;
}
int schematic_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int schematic_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int schematic_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@schematic_linkup: %.4s\n", &self->flag);
	return 0;
}




int schematic_search(struct entity* scene)
{
	return 0;
}
int schematic_modify(struct entity* scene)
{
	return 0;
}
int schematic_delete(struct entity* scene)
{
	return 0;
}
int schematic_create(struct entity* scene, void* arg, int argc, u8** argv)
{
	return 0;
}
