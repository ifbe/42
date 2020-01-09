#include "libuser.h"
#define _gear_ hex32('g','e','a','r')
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
		if(_world3d_ == world->type){
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
}




static void spurgear_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	carvesolid_rotategear(ctx, 0x444444, vc, vr, vf, vt, 32, act->fz0);
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




static void spurgear_write_gear(struct entity* gear, float* angle)
{
	gear->fz0 = PI/32 - angle[0];
}
static void spurgear_write_ray(struct entity* gear, struct fstyle* geom, float* ray)
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

	gear->fz0 = arctan2(uv[1], uv[0]);
	relationwrite(gear, _gear_, 0, 0, &gear->fz0, 1);
}




static void spurgear_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//world -> 2048
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;
	act = self->pchip;slot = self->pfoot;
	win = peer->pchip;geom = peer->pfoot;
	if(stack){
		//wnd -> cam, cam -> world
		struct entity* wnd;struct style* area;
		struct entity* wrd;struct style* camg;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		if('v' == len)spurgear_draw_gl41(act,slot, win,geom, wnd,area);
	}
}
static void spurgear_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct entity* gear = self->pchip;
	say("@spurgear_write:%llx\n", gear);

	if(_gear_ == self->flag)spurgear_write_gear(gear, buf);
	else spurgear_write_ray(gear, arg, buf);
}
static void spurgear_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void spurgear_start(struct halfrel* self, struct halfrel* peer)
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

	p->onstart = (void*)spurgear_start;
	p->onstop  = (void*)spurgear_stop;
	p->onread  = (void*)spurgear_read;
	p->onwrite = (void*)spurgear_write;
}
