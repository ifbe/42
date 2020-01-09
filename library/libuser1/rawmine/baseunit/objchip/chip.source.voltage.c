#include "libuser.h"




static void vsrc_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void vsrc_draw_vbo(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int j;
	u64 time;
	vec3 tc,tr,tf,tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;

	//negative
	for(j=0;j<3;j++){
		tf[j] = vf[j]/16;
		tc[j] = vc[j] -vf[j]+tf[j];
	}
	carvesolid_cylinder(wnd, 0x404040, tc, vr, vt, tf);

	//positive
	for(j=0;j<3;j++){
		tf[j] = vf[j]/16;
		tc[j] = vc[j] +vf[j]-tf[j];
	}
	carvesolid_cylinder(wnd, 0xc00000, tc, vr, vt, tf);

	//middle
	gl41opaque_cylinder(wnd, 0x80808080, vc, vr, vt, vf);
	time = timeread()%1000000;
	for(j=0;j<3;j++)tc[j] = vc[j] + vf[j]*(time/500000.0 - 1.0);
	carvesolid_circle(wnd, (time/4000)<<16, tc,vr,vt);
}
static void vsrc_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void vsrc_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void vsrc_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void vsrc_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void vsrc_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)vsrc_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)vsrc_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)vsrc_draw_html(act, pin, win, sty);
	else if(fmt == _json_)vsrc_draw_json(act, pin, win, sty);
	else vsrc_draw_pixel(act, pin, win, sty);
}




static void vsrc_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> vsrc
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len){
			vsrc_draw_vbo(act,slot, win,geom, wnd,area);
		}
	}
}
static void vsrc_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void vsrc_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void vsrc_start(struct halfrel* self, struct halfrel* peer)
{
}




static void vsrc_search(struct entity* act, u8* buf)
{
}
static void vsrc_modify(struct entity* act, u8* buf)
{
}
static void vsrc_delete(struct entity* act, u8* buf)
{
}
static void vsrc_create(struct entity* act, u8* buf)
{
}




void vsrc_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('v','s','r','c');

	p->oncreate = (void*)vsrc_create;
	p->ondelete = (void*)vsrc_delete;
	p->onsearch = (void*)vsrc_search;
	p->onmodify = (void*)vsrc_modify;

	p->onstart = (void*)vsrc_start;
	p->onstop  = (void*)vsrc_stop;
	p->onread  = (void*)vsrc_read;
	p->onwrite = (void*)vsrc_write;
}
