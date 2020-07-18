#include "libuser.h"






static void printboard_dx11draw(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	dx11solid_prism4(ctx, 0x00ff00, vc, vr, vf, vt);
}
static void printboard_gl41draw(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41solid_prism4(ctx, 0x00ff00, vc, vr, vf, vt);
}




static void printboard_camread(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key)
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
	case _dx11full_:printboard_dx11draw(ent,slot, wor,geom, wnd,area);break;
	case _gl41full_:printboard_gl41draw(ent,slot, wor,geom, wnd,area);break;
	}
}
int printboard_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot = stack[sp-1].pfoot;
	struct entity* wnd;struct style* area;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(wnd->fmt){
	default:printboard_camread(ent,foot, stack,sp, arg,key);break;
	}
	return 0;
}
int printboard_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
