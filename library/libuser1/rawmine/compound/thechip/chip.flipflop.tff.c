#include "libuser.h"




static void tff_draw_pixel(
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
static void tff_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	vec3 tc,tr,tf,tu;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	carveline_prism4(ctx, 0xffffff, vc, vr, vf, vu);
}
static void tff_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tff_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tff_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tff_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tff_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)tff_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)tff_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)tff_draw_html(act, pin, win, sty);
	else if(fmt == _json_)tff_draw_json(act, pin, win, sty);
	else tff_draw_pixel(act, pin, win, sty);
}




static void tff_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> tff
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)tff_draw_gl41(act,slot, win,geom, wnd,area);
	}
}
static void tff_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void tff_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void tff_start(struct halfrel* self, struct halfrel* peer)
{
}




static void tff_search(struct entity* act, u8* buf)
{
}
static void tff_modify(struct entity* act, u8* buf)
{
}
static void tff_delete(struct entity* act, u8* buf)
{
}
static void tff_create(struct entity* act, u8* buf)
{
}




void tff_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('t','f','f', 0);

	p->oncreate = (void*)tff_create;
	p->ondelete = (void*)tff_delete;
	p->onsearch = (void*)tff_search;
	p->onmodify = (void*)tff_modify;

	p->onstart = (void*)tff_start;
	p->onstop  = (void*)tff_stop;
	p->onread  = (void*)tff_read;
	p->onwrite = (void*)tff_write;
}
