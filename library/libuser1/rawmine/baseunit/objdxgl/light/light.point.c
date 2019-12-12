#include "libuser.h"
#define _fbo_ hex32('f','b','o',0)


#define CAMBUF buf0
#define LITBUF buf1
#define CTXBUF buf2
#define OWNBUF buf3
struct sunbuf{
	mat4 mvp;
	vec4 rgb;
	u32 u_rgb;
};




static void pointlight_search(struct entity* act)
{
}
static void pointlight_modify(struct entity* act)
{
}
static void pointlight_delete(struct entity* act)
{
}
static void pointlight_create(struct entity* act, void* str)
{
	struct sunbuf* sun;
	if(0 == act)return;

	sun = act->OWNBUF = memorycreate(0x400, 0);
	sun->u_rgb = 0x0000ff;
	sun->rgb[0] = ((sun->u_rgb >>16) & 0xff) / 255.0;
	sun->rgb[1] = ((sun->u_rgb >> 8) & 0xff) / 255.0;
	sun->rgb[2] = ((sun->u_rgb >> 0) & 0xff) / 255.0;

	act->LITBUF = memorycreate(0x400, 0);
}




static void pointlight_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void pointlight_draw_vbo(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	struct sunbuf* sun;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;

	sun = act->OWNBUF;
	if(0 == sun)return;

	carveopaque_sphere(ctx, 0x80000000|sun->u_rgb, vc, vr, vf, vt);
}
static void pointlight_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void pointlight_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void pointlight_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void pointlight_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void pointlight_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)pointlight_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)pointlight_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)pointlight_draw_html(act, pin, win, sty);
	else if(fmt == _json_)pointlight_draw_json(act, pin, win, sty);
	else pointlight_draw_pixel(act, pin, win, sty);
}
static void pointlight_light(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* wnd, struct style* area)
{
	struct sunbuf* sun;
	struct glsrc* src;

	sun = act->OWNBUF;
	if(0 == sun)return;
	src = act->LITBUF;
	if(0 == src)return;

	src->routine_name = "passtype";
	src->routine_detail = "pointlight";

	src->arg[0].fmt = 'v';
	src->arg[0].name = "sunxyz";
	src->arg[0].data = geom->fs.vc;

	src->arg[1].fmt = 'v';
	src->arg[1].name = "sunrgb";
	src->arg[1].data = sun->rgb;

	wnd->gl_light[0] = act->LITBUF;
}




static void pointlight_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;

//world -> spotlight
	struct entity* win;struct style* geom;
	struct entity* act;struct style* part;

	if(stack){
		act = self->pchip;part = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len){
			pointlight_light(act,part, win,geom, wnd,area);
			pointlight_draw_vbo(act,part, win,geom, wnd,area);
		}
	}
}
static void pointlight_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void pointlight_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void pointlight_start(struct halfrel* self, struct halfrel* peer)
{
}




void pointlight_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('p','o','i','n','t','l', 'i', 't');

	p->oncreate = (void*)pointlight_create;
	p->ondelete = (void*)pointlight_delete;
	p->onsearch = (void*)pointlight_search;
	p->onmodify = (void*)pointlight_modify;

	p->onstart = (void*)pointlight_start;
	p->onstop  = (void*)pointlight_stop;
	p->onread  = (void*)pointlight_read;
	p->onwrite = (void*)pointlight_write;
}
