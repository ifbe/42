#include "libuser.h"
void carveline_pmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void carveline_nmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void mem_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void mem_draw_vbo(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	//carveline_rect(ctx, 0x404040, vc, vr, vf);

	//vcc
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] +vf[j];
		tr[j] = vc[j] +vr[j] +vf[j];
	}
	carveline(ctx, 0xff0000, tc, tr);

	//gnd
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] -vf[j];
		tr[j] = vc[j] +vr[j] -vf[j];
	}
	carveline(ctx, 0x0000ff, tc, tr);

	//+
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j];
		tr[j] = tc[j] -tf[j]/2;
	}
	carveline(ctx, 0xff0000, tc,tr);

	//-
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j];
		tr[j] = tc[j] +tf[j]/2;
	}
	carveline(ctx, 0x0000ff, tc,tr);

	u32 lcolor[2] = {0xffffff, 0xffffff};
	u32 rcolor[2] = {0xffffff, 0xffffff};
	u32 gcolor[2];
	if(0 == act->ix0){lcolor[0] = 0xff0000;gcolor[0] = 0x0000ff;}
	else             {lcolor[1] = 0x0000ff;gcolor[0] = 0xff0000;}
	if(0 == act->iy0){rcolor[0] = 0xff0000;gcolor[1] = 0x0000ff;}
	else             {rcolor[1] = 0x0000ff;gcolor[1] = 0xff0000;}

	//l.p
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 +vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carveline_pmos(ctx, gcolor[0], lcolor[0], tc,tr,tf,vt);

	//l.n
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 -vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carveline_nmos(ctx, gcolor[0], lcolor[1], tc,tr,tf,vt);

	//l.g
	for(j=0;j<3;j++){
		tc[j] = vc[j] - vr[j]*3/4 - vf[j]/2;
		tr[j] = tc[j] + vf[j];
	}
	carveline(ctx, gcolor[0], tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vr[j]/2 - vf[j]/4;
		tr[j] = tc[j] + vf[j]/2;
	}
	carveline(ctx, gcolor[0], tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j]/8 -vr[j]*3/4;
		tr[j] = vc[j] +vf[j]/8 +vr[j]/2;
	}
	carveline(ctx, gcolor[0], tc, tr);

	//r.p
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2 +vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carveline_pmos(ctx, gcolor[1], rcolor[0], tc,tr,tf,vt);

	//r.n
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2 -vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carveline_nmos(ctx, gcolor[1], rcolor[1], tc,tr,tf,vt);

	//l.g
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*1/4 -vf[j]/2;
		tr[j] = tc[j] +vf[j];
	}
	carveline(ctx, gcolor[1], tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 -vf[j]/4;
		tr[j] = tc[j] +vf[j]/2;
	}
	carveline(ctx, gcolor[1], tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]/8 -vr[j]/2;
		tr[j] = vc[j] -vf[j]/8 +vr[j]*1/4;
	}
	carveline(ctx, gcolor[1], tc, tr);
}
static void mem_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mem_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mem_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mem_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void mem_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> mem
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)mem_draw_vbo(act,slot, win,geom, wnd,area);
	}
}
static void mem_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	u8 tmp;
	struct entity* ent = self->pchip;
	say("@memgate_write:%x\n",buf[0]);

	if('0' == buf[0])ent->ix0 = 0;
	else if('1' == buf[0])ent->ix0 = 1;
	else return;

	ent->iy0 = !ent->ix0;
}
static void mem_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void mem_start(struct halfrel* self, struct halfrel* peer)
{
}




static void mem_search(struct entity* act, u8* buf)
{
}
static void mem_modify(struct entity* act, u8* buf)
{
}
static void mem_delete(struct entity* act, u8* buf)
{
}
static void mem_create(struct entity* act, u8* buf)
{
	act->ix0 = getrandom()&1;
	act->iy0 = !act->ix0;
}




void mem_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('m','e','m', 0);

	p->oncreate = (void*)mem_create;
	p->ondelete = (void*)mem_delete;
	p->onsearch = (void*)mem_search;
	p->onmodify = (void*)mem_modify;

	p->onstart = (void*)mem_start;
	p->onstop  = (void*)mem_stop;
	p->onread  = (void*)mem_read;
	p->onwrite = (void*)mem_write;
}
