#include "libuser.h"




static void pointlight_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void pointlight_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vt = sty->f.vt;
	u32 rgb = (act->data0)&0xffffff;
	carveopaque_sphere(win, 0x40000000|rgb, vc, vr, vf, vt);
}
static void pointlight_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void pointlight_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void pointlight_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void pointlight_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void pointlight_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
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
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u32 rgb;
	float* tmp;
	struct glsrc* src;

	rgb = act->data0;
	tmp = act->buf;
	tmp[2] = (rgb&0xff) / 255.0;
	tmp[1] = ((rgb >>  8)&0xff) / 255.0;
	tmp[0] = ((rgb >> 16)&0xff) / 255.0;

	src = win->gl_light;

	src->arg_fmt[0] = 'v';
	src->arg_name[0] = "sunxyz";
	src->arg_data[0] = sty->fs.vc;

	src->arg_fmt[1] = 'v';
	src->arg_name[1] = "sunrgb";
	src->arg_data[1] = tmp;
}
static void pointlight_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* ctx = buf;

	if(ctx){
		switch(ctx->type){
			case _gl41data_:{
				pointlight_light(act,pin,ctx,sty);
				pointlight_draw_vbo(act,pin,ctx,sty);
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




static void pointlight_search(struct actor* act)
{
}
static void pointlight_modify(struct actor* act)
{
}
static void pointlight_delete(struct actor* act)
{
}
static void pointlight_create(struct actor* act, void* str)
{
	act->data0 = 0xffffff;
	act->buf = memorycreate(0x1000, 0);
}




void pointlight_register(struct actor* p)
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
