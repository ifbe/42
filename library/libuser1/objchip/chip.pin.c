#include "libuser.h"




static void chippin_draw_pixel(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void chippin_draw_vbo2d(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void chippin_draw_vbo3d(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void chippin_draw_json(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void chippin_draw_html(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void chippin_draw_tui(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void chippin_draw_cli(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void chippin_draw(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)chippin_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)chippin_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)chippin_draw_html(act, pin, win, sty);
	else if(fmt == _json_)chippin_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)chippin_draw_vbo2d(act, pin, win, sty);
		else chippin_draw_vbo3d(act, pin, win, sty);
	}
	else chippin_draw_pixel(act, pin, win, sty);
}




static void chippin_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct pinid* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	chippin_draw(act, pin, win, sty);
}
static void chippin_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void chippin_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void chippin_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void chippin_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void chippin_start(struct halfrel* self, struct halfrel* peer)
{
}
static void chippin_delete(struct actor* act, u8* buf)
{
}
static void chippin_create(struct actor* act, u8* buf)
{
}




void chippin_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('p','i','n',0);

	p->oncreate = (void*)chippin_create;
	p->ondelete = (void*)chippin_delete;
	p->onstart  = (void*)chippin_start;
	p->onstop   = (void*)chippin_stop;
	p->oncread  = (void*)chippin_cread;
	p->oncwrite = (void*)chippin_cread;
	p->onsread  = (void*)chippin_sread;
	p->onswrite = (void*)chippin_swrite;
}
