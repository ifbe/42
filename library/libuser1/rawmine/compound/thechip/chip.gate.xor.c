#include "libuser.h"
void carveline_pmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void carveline_nmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void xor_draw_pixel(
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
static void xor_draw_vbo(
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
		tc[j] = vc[j] -vr[j]/4 +vf[j];
		tr[j] = tc[j] -tf[j]/2;
	}
	carveline(ctx, 0xff0000, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/4 +vf[j];
		tr[j] = tc[j] -tf[j]/2;
	}
	carveline(ctx, 0xff0000, tc,tr);

	//-
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/4 -vf[j];
		tr[j] = tc[j] +tf[j]/2;
	}
	carveline(ctx, 0x0000ff, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/4 -vf[j];
		tr[j] = tc[j] +tf[j]/2;
	}
	carveline(ctx, 0x0000ff, tc,tr);

	u32 acolor = act->ix0 ? 0xff0000 : 0x0000ff;
	u32 xcolor = act->ix0 ? 0x0000ff : 0xff0000;	//x=a'
	u32 bcolor = act->iy0 ? 0xff0000 : 0x0000ff;
	u32 ycolor = act->iy0 ? 0x0000ff : 0xff0000;	//y=b'
	u32 ocolor = act->iz0 ? 0xff0000 : 0x0000ff;
	u32 mcolor[8] = {
		0xffffff, 0xffffff,
		0xffffff, 0xffffff,
		0xffffff, 0xffffff,
		0xffffff, 0xffffff
	};
	if(1 == act->ix0){
		mcolor[0] = 0xff0000;
		if(0 == act->iy0)mcolor[2] = 0xff0000;
	}
	if(0 == act->ix0){
		mcolor[1] = 0xff0000;
		if(1 == act->iy0)mcolor[3] = 0xff0000;
	}
	if(1 == act->iy0){
		mcolor[6] = 0x0000ff;
		if(1 == act->ix0)mcolor[4] = 0x0000ff;
	}
	if(0 == act->iy0){
		mcolor[7] = 0x0000ff;
		if(0 == act->ix0)mcolor[5] = 0x0000ff;
	}

	//p: a', a, b, b'
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/4 +vf[j]*3/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carveline_pmos(ctx, xcolor, mcolor[0], tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/4 +vf[j]*3/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carveline_pmos(ctx, acolor, mcolor[1], tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/4 +vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carveline_pmos(ctx, bcolor, mcolor[2], tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/4 +vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carveline_pmos(ctx, ycolor, mcolor[3], tc,tr,tf,vt);

	//n: a, a', b, b'
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/4 -vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carveline_nmos(ctx, acolor, mcolor[4], tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/4 -vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carveline_nmos(ctx, xcolor, mcolor[5], tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/4 -vf[j]*3/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carveline_nmos(ctx, bcolor, mcolor[6], tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/4 -vf[j]*3/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carveline_nmos(ctx, ycolor, mcolor[7], tc,tr,tf,vt);

	//a
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] -vf[j]*1/4;
		tr[j] = tc[j] +vr[j]/2;
	}
	carveline(ctx, acolor, tc,tr);

	//b
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] -vf[j]*3/4;
		tr[j] = tc[j] +vr[j]/2;
	}
	carveline(ctx, bcolor, tc,tr);

	//o
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j];
		tr[j] = tc[j] -vr[j]*5/4;
	}
	carveline(ctx, ocolor, tc,tr);
}
static void xor_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void xor_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void xor_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void xor_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void xor_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> xor
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)xor_draw_vbo(act,slot, win,geom, wnd,area);
	}
}
static void xor_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	struct entity* ent = self->pchip;
	say("@xorgate_write:%x\n",buf[0]);
}
static void xor_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void xor_start(struct halfrel* self, struct halfrel* peer)
{
}




static void xor_search(struct entity* act, u8* buf)
{
}
static void xor_modify(struct entity* act, u8* buf)
{
}
static void xor_delete(struct entity* act, u8* buf)
{
}
static void xor_create(struct entity* act, u8* buf)
{
	act->ix0 = getrandom()&1;
	act->iy0 = getrandom()&1;
	act->iz0 = act->ix0 ^ act->iy0;
}




void xor_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('x','o','r', 0);

	p->oncreate = (void*)xor_create;
	p->ondelete = (void*)xor_delete;
	p->onsearch = (void*)xor_search;
	p->onmodify = (void*)xor_modify;

	p->onstart = (void*)xor_start;
	p->onstop  = (void*)xor_stop;
	p->onread  = (void*)xor_read;
	p->onwrite = (void*)xor_write;
}
