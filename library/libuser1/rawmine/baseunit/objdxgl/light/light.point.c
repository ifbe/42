#include "libuser.h"
struct sunbuf{
	mat4 mvp;
	vec4 rgb;
	u32 u_rgb;

	u8 data[0];
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

	sun = act->buf0 = memorycreate(0x1000, 0);
	if(0 == sun)return;

	//
	sun->u_rgb = 0x0000ff;
	sun->rgb[0] = ((sun->u_rgb >>16) & 0xff) / 255.0;
	sun->rgb[1] = ((sun->u_rgb >> 8) & 0xff) / 255.0;
	sun->rgb[2] = ((sun->u_rgb >> 0) & 0xff) / 255.0;
}




static void pointlight_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void pointlight_draw_vbo(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	struct sunbuf* sun;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vt = sty->f.vt;

	sun = act->buf0;
	if(0 == sun)return;

	carveopaque_sphere(win, 0x80000000|sun->u_rgb, vc, vr, vf, vt);
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
	else if(fmt == _vbo_)pointlight_draw_vbo(act, pin, win, sty);
	else pointlight_draw_pixel(act, pin, win, sty);
}




void pointlight_light(
	struct entity* act, struct fstyle* pin,
	struct entity* win, struct fstyle* sty)
{
	struct sunbuf* sun;
	struct glsrc* src;

	sun = act->buf0;
	if(0 == sun)return;
	src = win->gl_light;
	if(0 == src)return;

	src->routine_name = "passtype";
	src->routine_detail = "pointlight";

	src->arg[0].fmt = 'v';
	src->arg[0].name = "sunxyz";
	src->arg[0].data = sty->vc;

	src->arg[1].fmt = 'v';
	src->arg[1].name = "sunrgb";
	src->arg[1].data = sun->rgb;
}
static void pointlight_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct entity* ctx = buf;

	if(ctx){
		switch(ctx->type){
			case _gl41data_:{
				pointlight_light(act, &pin->fs, ctx, &sty->fs);
				pointlight_draw_vbo(act, pin, ctx, sty);
			}
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
