#include "libuser.h"




static void dirlight_draw_pixel(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void dirlight_draw_vbo(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	float x,y;
	vec3 ta, tb;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	for(y=-1.0;y<1.01;y+=0.1)
	{
		for(x=-1.0;x<1.01;x+=0.1)
		{
			ta[0] = vc[0] + x*vr[0] + y*vu[0];
			ta[1] = vc[1] + x*vr[1] + y*vu[1];
			ta[2] = vc[2] + x*vr[2] + y*vu[2];
			tb[0] = ta[0] - vf[0];
			tb[1] = ta[1] - vf[1];
			tb[2] = ta[2] - vf[2];
			carveline(win, 0xffff00, ta, tb);
		}
	}
}
static void dirlight_draw_json(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void dirlight_draw_html(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void dirlight_draw_tui(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void dirlight_draw_cli(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void dirlight_draw(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)dirlight_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)dirlight_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)dirlight_draw_html(act, pin, win, sty);
	else if(fmt == _json_)dirlight_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)dirlight_draw_vbo(act, pin, win, sty);
	else dirlight_draw_pixel(act, pin, win, sty);
}




static void dirlight_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	dirlight_draw(act, pin, win, sty);
}
static void dirlight_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void dirlight_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void dirlight_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void dirlight_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void dirlight_start(struct halfrel* self, struct halfrel* peer)
{
}
static void dirlight_delete(struct actor* act)
{
}
static void dirlight_create(struct actor* act, void* str)
{
}




void dirlight_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('d','i','r','l', 'i', 't', 0, 0);

	p->oncreate = (void*)dirlight_create;
	p->ondelete = (void*)dirlight_delete;
	p->onstart  = (void*)dirlight_start;
	p->onstop   = (void*)dirlight_stop;
	p->oncread  = (void*)dirlight_cread;
	p->oncwrite = (void*)dirlight_cwrite;
	p->onsread  = (void*)dirlight_sread;
	p->onswrite = (void*)dirlight_swrite;
}
