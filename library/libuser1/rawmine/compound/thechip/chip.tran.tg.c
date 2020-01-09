#include "libuser.h"




static void tg_draw_pixel(
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
static void tg_draw_gl41(
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
static void tg_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tg_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tg_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tg_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tg_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)tg_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)tg_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)tg_draw_html(act, pin, win, sty);
	else if(fmt == _json_)tg_draw_json(act, pin, win, sty);
	else tg_draw_pixel(act, pin, win, sty);
}




static void tg_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> tg
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)tg_draw_gl41(act,slot, win,geom, wnd,area);
	}
}
static void tg_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void tg_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void tg_start(struct halfrel* self, struct halfrel* peer)
{
}




static void tg_search(struct entity* act, u8* buf)
{
}
static void tg_modify(struct entity* act, u8* buf)
{
}
static void tg_delete(struct entity* act, u8* buf)
{
}
static void tg_create(struct entity* act, u8* buf)
{
}




void tg_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('t','g', 0, 0);

	p->oncreate = (void*)tg_create;
	p->ondelete = (void*)tg_delete;
	p->onsearch = (void*)tg_search;
	p->onmodify = (void*)tg_modify;

	p->onstart = (void*)tg_start;
	p->onstop  = (void*)tg_stop;
	p->onread  = (void*)tg_read;
	p->onwrite = (void*)tg_write;
}
