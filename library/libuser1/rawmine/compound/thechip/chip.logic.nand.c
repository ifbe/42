#include "libuser.h"
void carveline_pmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void carveline_nmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void nand_draw_pixel(
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
static void nand_draw_gl41(
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
		tc[j] = vc[j] -vr[j]/2 +vf[j];
		tr[j] = tc[j] -vf[j]/4;
	}
	carveline(ctx, 0xff0000, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2 +vf[j];
		tr[j] = tc[j] -vf[j]/4;
	}
	carveline(ctx, 0xff0000, tc,tr);


	u8 pstatus[2];
	u8 nstatus[2];
	if(act->ix0){pstatus[0] = 0;nstatus[0] = 1;}
	else        {pstatus[0] = 1;nstatus[0] = 0;}
	if(act->iy0){pstatus[1] = 0;nstatus[1] = 1;}
	else        {pstatus[1] = 1;nstatus[1] = 0;}
	//say("%d,%d,%d,%d,%d,%d\n",act->ix0,act->iy0, pstatus[0],pstatus[1], nstatus[0],nstatus[1]);

	u32 xcolor = act->ix0 ? 0xff0000 : 0x0000ff;
	u32 ycolor = act->iy0 ? 0xff0000 : 0x0000ff;
	u32 ocolor = act->iz0 ? 0xff0000 : 0x0000ff;
	u32 pcolor[2] = {0xffffff, 0xffffff};
	u32 ncolor[2] = {0xffffff, 0xffffff};
	if(pstatus[0])pcolor[0] = 0xff0000;
	if(pstatus[1])pcolor[1] = 0xff0000;
	if(nstatus[1])ncolor[1] = 0x0000ff;
	if(nstatus[0]&&nstatus[1])ncolor[0] = 0x0000ff;

	//p1
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 +vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carveline_pmos(ctx, xcolor, pcolor[0], tc,tr,tf,vt);

	//p2
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2 +vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carveline_pmos(ctx, ycolor, pcolor[1], tc,tr,tf,vt);

	//n1
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carveline_nmos(ctx, xcolor, ncolor[0], tc,tr,tf,vt);

	//n2
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]*3/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carveline_nmos(ctx, ycolor, ncolor[1], tc,tr,tf,vt);

	//a
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] -vf[j]*1/4;
		tr[j] = tc[j] +vr[j];
	}
	carveline(ctx, xcolor, tc,tr);

	//b
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] -vf[j]*3/4;
		tr[j] = tc[j] +vr[j];
	}
	carveline(ctx, ycolor, tc,tr);

	//o
	for(j=0;j<3;j++){
		tc[j] = vc[j] - vr[j]/2;
		tr[j] = vc[j] + vr[j];
	}
	carveline(ctx, ocolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2;
		tr[j] = tc[j] +vf[j]/4;
	}
	carveline(ctx, ocolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2;
		tr[j] = tc[j] +vf[j]/4;
	}
	carveline(ctx, ocolor, tc,tr);
}
static void nand_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void nand_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void nand_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void nand_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void nand_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> nand
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)nand_draw_gl41(act,slot, win,geom, wnd,area);
	}
}
static void nand_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	u8 tmp;
	struct entity* ent = self->pchip;
	say("@nandgate_write:%x\n",buf[0]);

	if('a' == self->flag){
		if('0' == buf[0])ent->ix0 = 0;
		else if('1' == buf[0])ent->ix0 = 1;
		else return;
	}
	else if('b' == self->flag){
		if('0' == buf[0])ent->iy0 = 0;
		else if('1' == buf[0])ent->iy0 = 1;
		else return;
	}
	else return;

	ent->iz0 = !(ent->ix0 && ent->iy0);
	tmp = ent->iz0 + 0x30;
	relationwrite(ent, 'o', 0, 0, &tmp, 1);
}
static void nand_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void nand_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void nand_search(struct entity* act, u8* buf)
{
}
static void nand_modify(struct entity* act, u8* buf)
{
}
static void nand_delete(struct entity* act, u8* buf)
{
}
static void nand_create(struct entity* act, u8* buf)
{
	act->ix0 = getrandom()&1;
	act->iy0 = getrandom()&1;
	act->iz0 = !(act->ix0 && act->iy0);
}




void nand_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('n','a','n','d');

	p->oncreate = (void*)nand_create;
	p->ondelete = (void*)nand_delete;
	p->onsearch = (void*)nand_search;
	p->onmodify = (void*)nand_modify;

	p->onlinkup = (void*)nand_linkup;
	p->ondiscon = (void*)nand_discon;
	p->onread  = (void*)nand_read;
	p->onwrite = (void*)nand_write;
}
