#include "libuser.h"
void carvepmos(struct entity* wnd, vec3 vc, vec3 vr, vec3 vf, vec3 vt)
{
	int j,k;
	vec3 t1,t2;
	vec3 tc,tr,tf;

	//gate
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j];
		tr[j] = vc[j] -vr[j]*9/16;
	}
	carveline(wnd, 0xffffff, tc,tr);

	for(j=0;j<3;j++){
		t1[j] = vc[j] -vr[j]*9/16 -vf[j]/4;
		t2[j] = vc[j] -vr[j]*9/16 +vf[j]/4;
	}
	carveline(wnd, 0xffffff, t1, t2);

	//p,n,p
	for(k=0;k<3;k++){
		for(j=0;j<3;j++){
			t1[j] = vc[j] -vr[j]/2 + vf[j]*(3*k-4)/16;
			t2[j] = t1[j] +vf[j]*2/16;
		}
		carveline(wnd, 0xffffff, t1,t2);
	}

	//s,d,b
	for(k=-1;k<2;k+=2){
		for(j=0;j<3;j++){
			t1[j] = vc[j] - vr[j]/2 + vf[j]*k*3/16;
			t2[j] = vc[j] + vf[j]*k*3/16;
		}
		carveline(wnd, 0xffffff, t1,t2);
	}
	for(j=0;j<3;j++){
		t1[j] = vc[j] - vr[j]/2;
		t2[j] = vc[j];
	}
	carveline_arrow(wnd, 0xffffff, t1, t2, vt);

	//s,d
	for(j=0;j<3;j++){
		t1[j] = vc[j];
		t2[j] = vc[j] + vf[j];
	}
	carveline(wnd, 0xffffff, t1,t2);

	for(j=0;j<3;j++){
		t1[j] = vc[j] - vf[j]*3/16;
		t2[j] = vc[j] - vf[j];
	}
	carveline(wnd, 0xffffff, t1,t2);
}
void carvenmos(struct entity* wnd, vec3 vc, vec3 vr, vec3 vf, vec3 vt)
{
	int j,k;
	vec3 t1,t2;
	vec3 tc,tr,tf;

	//gate
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j];
		tr[j] = vc[j] -vr[j]*9/16;
	}
	carveline(wnd, 0xffffff, tc,tr);

	for(j=0;j<3;j++){
		t1[j] = vc[j] -vr[j]*9/16 -vf[j]/4;
		t2[j] = vc[j] -vr[j]*9/16 +vf[j]/4;
	}
	carveline(wnd, 0xffffff, t1, t2);

	//p,n,p
	for(k=0;k<3;k++){
		for(j=0;j<3;j++){
			t1[j] = vc[j] -vr[j]/2 + vf[j]*(3*k-4)/16;
			t2[j] = t1[j] +vf[j]*2/16;
		}
		carveline(wnd, 0xffffff, t1,t2);
	}

	//s,d,b
	for(k=-1;k<2;k+=2){
		for(j=0;j<3;j++){
			t1[j] = vc[j] - vr[j]/2 + vf[j]*k*3/16;
			t2[j] = vc[j] + vf[j]*k*3/16;
		}
		carveline(wnd, 0xffffff, t1,t2);
	}
	for(j=0;j<3;j++){
		t1[j] = vc[j] - vr[j]/2;
		t2[j] = vc[j];
	}
	carveline_arrow(wnd, 0xffffff, t2, t1, vt);

	//s,d
	for(j=0;j<3;j++){
		t1[j] = vc[j] + vf[j]*3/16;
		t2[j] = vc[j] + vf[j];
	}
	carveline(wnd, 0xffffff, t1,t2);

	for(j=0;j<3;j++){
		t1[j] = vc[j];
		t2[j] = vc[j] - vf[j];
	}
	carveline(wnd, 0xffffff, t1,t2);
}




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
static void nand_draw_vbo(
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
	carveline_rect(ctx, 0xffffff, vc, vr, vf);

	//p1
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 +vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carvepmos(ctx, tc,tr,tf,vt);

	//p2
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2 +vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carvepmos(ctx, tc,tr,tf,vt);

	//n1
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carvenmos(ctx, tc,tr,tf,vt);

	//n2
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]*3/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carvenmos(ctx, tc,tr,tf,vt);

	//o
	for(j=0;j<3;j++){
		tc[j] = vc[j] - vr[j]/2;
		tr[j] = vc[j] + vr[j];
	}
	carveline(ctx, 0xffffff, tc,tr);

	//
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 +vf[j];
		tr[j] = tc[j] -vf[j]/4;
	}
	carveline(ctx, 0xffffff, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2 +vf[j];
		tr[j] = tc[j] -vf[j]/4;
	}
	carveline(ctx, 0xffffff, tc,tr);

	//
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2;
		tr[j] = tc[j] +vf[j]/4;
	}
	carveline(ctx, 0xffffff, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2;
		tr[j] = tc[j] +vf[j]/4;
	}
	carveline(ctx, 0xffffff, tc,tr);
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
static void nand_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)nand_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)nand_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)nand_draw_html(act, pin, win, sty);
	else if(fmt == _json_)nand_draw_json(act, pin, win, sty);
	else nand_draw_pixel(act, pin, win, sty);
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
		if('v' == len)nand_draw_vbo(act,slot, win,geom, wnd,area);
	}
}
static void nand_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void nand_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void nand_start(struct halfrel* self, struct halfrel* peer)
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
}




void nand_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('n','a','n','d');

	p->oncreate = (void*)nand_create;
	p->ondelete = (void*)nand_delete;
	p->onsearch = (void*)nand_search;
	p->onmodify = (void*)nand_modify;

	p->onstart = (void*)nand_start;
	p->onstop  = (void*)nand_stop;
	p->onread  = (void*)nand_read;
	p->onwrite = (void*)nand_write;
}
