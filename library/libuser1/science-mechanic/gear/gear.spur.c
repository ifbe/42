#include "libuser.h"
#define _gear_ hex32('g','e','a','r')
#define STAMP listu64.data0
int ray_plane(void* ray, void* plane, void* out);




static void spurgear_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	_obj* world;
	struct fstyle* obb = 0;
	//logtoall("freecam@%llx,%llx,%llx,%d\n",act,pin,buf,len);

	rel = act->irel0;
	while(1){
		if(0 == rel)return;
		world = (void*)(rel->srcchip);
		if(_virtual_ == world->type){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return;
		}
		rel = samedstnextsrc(rel);
	}
}
static void spurgear_modify(_obj* act, u8* buf)
{
}
static void spurgear_delete(_obj* act, u8* buf)
{
}
static void spurgear_create(_obj* act, void* arg, int argc, u8** argv)
{
    act->whdf.fx0 = 0.0;
    act->whdf.fy0 = 0.0;
    act->whdf.fz0 = 0.0;
	act->STAMP = 0;
}




static void spurgear_draw_gl41(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41solid_rotategear(ctx, 0x444444, vc, vr, vf, vt, 32, act->whdf.fz0);
}
static void spurgear_draw_pixel(
	_obj* act, struct style* pin,
	_obj* wnd, struct style* sty)
{
}
static void spurgear_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spurgear_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spurgear_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spurgear_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static int spurgear_recur(_syn* stack,int sp, _obj* gear)
{
	int j;
	for(j=0;j<sp;j+=2){
		if(gear == stack[j].pchip)return 1;
	}
	return 0;
}
static void spurgear_spread(_syn* stack, int sp, _obj* gear)
{
	struct relation* rel;
	_obj* next;

	rel = gear->irel0;
	while(1){
		if(0 == rel)break;
logtoall("@11:%.4s\n",&rel->dstfoottype);
		if(_gear_ != rel->dstfoottype)goto next1;
logtoall("@12\n");
		next = rel->psrcchip;
		if(spurgear_recur(stack,sp, next))goto next1;
		if(next->STAMP == gear->STAMP)goto next1;
		next->STAMP = gear->STAMP;
logtoall("@14\n");
		stack[sp+0].pchip = gear;
		stack[sp+1].pchip = next;
		stack[sp+1].foottype = _gear_;
		entity_giveby(next,0, stack,sp+2, 0,0, &gear->whdf.fz0,1);
next1:
		rel = samedstnextsrc(rel);
	}

	rel = gear->orel0;
	while(1){
		if(0 == rel)break;
logtoall("@21:%.4s\n",&rel->srcfoottype);
		if(_gear_ != rel->srcfoottype)goto next2;
logtoall("@22\n");
		next = rel->pdstchip;
		if(spurgear_recur(stack,sp, next))goto next2;
		if(next->STAMP == gear->STAMP)goto next2;
		next->STAMP = gear->STAMP;
logtoall("@24\n");
		stack[sp+0].pchip = gear;
		stack[sp+1].pchip = next;
		stack[sp+1].foottype = _gear_;
		entity_giveby(next,0, stack,sp+2, 0,0, &gear->whdf.fz0,1);
next2:
		rel = samesrcnextdst(rel);
	}
}




static void spurgear_write_val(_obj* gear, float* val)
{
	//wrong
	gear->whdf.fz0 = PI/32 - val[0];
}
static void spurgear_write_ray(_obj* gear,void* foot, struct fstyle* geom, float* ray)
{
	logtoall("%f,%f,%f -> %f,%f,%f\n",ray[0],ray[1],ray[2], ray[3],ray[4],ray[5]);

	int j;
	vec3 out;
	vec3 plane[2];
	for(j=0;j<3;j++){
		plane[0][j] = geom->vc[j];
		plane[1][j] = geom->vt[j];
	}
	j = ray_plane(ray, plane, out);
	if(j <= 0)return;
	logtoall("%f,%f,%f\n",out[0],out[1],out[2]);

	float uv[3];
	vec3 tr,tf;
	for(j=0;j<3;j++){
		tr[j] = out[j] - geom->vc[j];
		tf[j] = out[j] - geom->vc[j];
	}
	uv[0] = vec3_dot(tr, geom->vr) / vec3_getlen(tr) / vec3_getlen(geom->vr);
	uv[1] = vec3_dot(tf, geom->vf) / vec3_getlen(tf) / vec3_getlen(geom->vf);
	logtoall("%f,%f\n", uv[0], uv[1]);

	gear->whdf.fz0 = arctanyx(uv[1], uv[0]);
}




static void spurgear_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	spurgear_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void spurgear_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller = stack[sp-2].pchip;
	//struct style* area = stack[sp-2].pfoot;

	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	default:
		spurgear_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void spurgear_giving(_obj* gear,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{

	if(_gear_ == stack[sp-1].foottype){
		logtoall("@spurgear_gear:%llx\n", gear);
		spurgear_write_val(gear, buf);
		spurgear_spread(stack, sp, gear);
	}
	else{
		logtoall("@spurgear_mouse:%llx\n", gear);
		gear->STAMP += 1;
		spurgear_write_ray(gear, 0, (void*)arg,buf);
		spurgear_spread(stack, sp, gear);
	}
}
static void spurgear_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void spurgear_attach(struct halfrel* self, struct halfrel* peer)
{
}




void spurgear_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('s','p','u','r','g','e','a','r');

	p->oncreate = (void*)spurgear_create;
	p->ondelete = (void*)spurgear_delete;
	p->onreader = (void*)spurgear_search;
	p->onwriter = (void*)spurgear_modify;

	p->onattach = (void*)spurgear_attach;
	p->ondetach = (void*)spurgear_detach;
	p->ontaking = (void*)spurgear_taking;
	p->ongiving = (void*)spurgear_giving;
}
