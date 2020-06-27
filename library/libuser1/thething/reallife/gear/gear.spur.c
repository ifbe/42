#include "libuser.h"
#define _gear_ hex32('g','e','a','r')
#define STAMP data0
int ray_plane(void* ray, void* plane, void* out);




static void spurgear_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	struct entity* world;
	struct fstyle* obb = 0;
	//say("freecam@%llx,%llx,%llx,%d\n",act,pin,buf,len);

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
static void spurgear_modify(struct entity* act, u8* buf)
{
}
static void spurgear_delete(struct entity* act, u8* buf)
{
}
static void spurgear_create(struct entity* act, void* arg, int argc, u8** argv)
{
    act->fx0 = 0.0;
    act->fy0 = 0.0;
    act->fz0 = 0.0;
	act->STAMP = 0;
}




static void spurgear_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41solid_rotategear(ctx, 0x444444, vc, vr, vf, vt, 32, act->fz0);
}
static void spurgear_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* wnd, struct style* sty)
{
}
static void spurgear_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spurgear_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spurgear_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spurgear_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static int spurgear_recur(_syn* stack,int sp, struct entity* gear)
{
	int j;
	for(j=0;j<sp;j+=2){
		if(gear == stack[j].pchip)return 1;
	}
	return 0;
}
static void spurgear_spread(_syn* stack, int sp, struct entity* gear)
{
	struct relation* rel;
	struct entity* next;

	rel = gear->irel0;
	while(1){
		if(0 == rel)break;
say("@11:%.4s\n",&rel->dstflag);
		if(_gear_ != rel->dstflag)goto next1;
say("@12\n");
		next = rel->psrcchip;
		if(spurgear_recur(stack,sp, next))goto next1;
		if(next->STAMP == gear->STAMP)goto next1;
		next->STAMP = gear->STAMP;
say("@14\n");
		stack[sp+0].pchip = gear;
		stack[sp+1].pchip = next;
		entitywrite(next,_gear_, stack,sp+2, 0,0, &gear->fz0,1);
next1:
		rel = samedstnextsrc(rel);
	}

	rel = gear->orel0;
	while(1){
		if(0 == rel)break;
say("@21:%.4s\n",&rel->srcflag);
		if(_gear_ != rel->srcflag)goto next2;
say("@22\n");
		next = rel->pdstchip;
		if(spurgear_recur(stack,sp, next))goto next2;
		if(next->STAMP == gear->STAMP)goto next2;
		next->STAMP = gear->STAMP;
say("@24\n");
		stack[sp+0].pchip = gear;
		stack[sp+1].pchip = next;
		entitywrite(next,_gear_, stack,sp+2, 0,0, &gear->fz0,1);
next2:
		rel = samesrcnextdst(rel);
	}
}




static void spurgear_write_val(struct entity* gear, float* val)
{
	//wrong
	gear->fz0 = PI/32 - val[0];
}
static void spurgear_write_ray(struct entity* gear,int foot, struct fstyle* geom, float* ray)
{
	say("%f,%f,%f -> %f,%f,%f\n",ray[0],ray[1],ray[2], ray[3],ray[4],ray[5]);

	int j;
	vec3 out;
	vec3 plane[2];
	for(j=0;j<3;j++){
		plane[0][j] = geom->vc[j];
		plane[1][j] = geom->vt[j];
	}
	j = ray_plane(ray, plane, out);
	if(j <= 0)return;
	say("%f,%f,%f\n",out[0],out[1],out[2]);

	float uv[3];
	vec3 tr,tf;
	for(j=0;j<3;j++){
		tr[j] = out[j] - geom->vc[j];
		tf[j] = out[j] - geom->vc[j];
	}
	uv[0] = vec3_dot(tr, geom->vr) / vec3_getlen(tr) / vec3_getlen(geom->vr);
	uv[1] = vec3_dot(tf, geom->vf) / vec3_getlen(tf) / vec3_getlen(geom->vf);
	say("%f,%f\n", uv[0], uv[1]);

	gear->fz0 = arctanyx(uv[1], uv[0]);
}




static void spurgear_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		spurgear_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void spurgear_giving(_ent* gear,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{

	if(_gear_ == foot){
		say("@spurgear_gear:%llx\n", gear);
		spurgear_write_val(gear, buf);
		spurgear_spread(stack, sp, gear);
	}
	else{
		say("@spurgear_mouse:%llx\n", gear);
		gear->STAMP += 1;
		spurgear_write_ray(gear, 0, arg,buf);
		spurgear_spread(stack, sp, gear);
	}
}
static void spurgear_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void spurgear_linkup(struct halfrel* self, struct halfrel* peer)
{
}




void spurgear_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s','p','u','r','g','e','a','r');

	p->oncreate = (void*)spurgear_create;
	p->ondelete = (void*)spurgear_delete;
	p->onsearch = (void*)spurgear_search;
	p->onmodify = (void*)spurgear_modify;

	p->onlinkup = (void*)spurgear_linkup;
	p->ondiscon = (void*)spurgear_discon;
	p->ontaking = (void*)spurgear_taking;
	p->ongiving = (void*)spurgear_giving;
}
