#include "libuser.h"




static void or_draw_pixel(
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
static void or_draw_vbo(
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
static void or_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void or_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void or_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void or_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void or_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)or_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)or_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)or_draw_html(act, pin, win, sty);
	else if(fmt == _json_)or_draw_json(act, pin, win, sty);
	else or_draw_pixel(act, pin, win, sty);
}




static void or_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> or
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)or_draw_vbo(act,slot, win,geom, wnd,area);
	}
}
static void or_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void or_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void or_start(struct halfrel* self, struct halfrel* peer)
{
}




static void or_search(struct entity* act, u8* buf)
{
}
static void or_modify(struct entity* act, u8* buf)
{
}
static void or_delete(struct entity* act, u8* buf)
{
}
static void or_create(struct entity* act, u8* buf)
{
}




void or_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('o','r', 0, 0);

	p->oncreate = (void*)or_create;
	p->ondelete = (void*)or_delete;
	p->onsearch = (void*)or_search;
	p->onmodify = (void*)or_modify;

	p->onstart = (void*)or_start;
	p->onstop  = (void*)or_stop;
	p->onread  = (void*)or_read;
	p->onwrite = (void*)or_write;
}
