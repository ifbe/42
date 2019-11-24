#include "libuser.h"




static void isrc_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}/*
static void isrc_draw_vbo2d(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}*/
static void isrc_draw_vbo3d(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void isrc_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void isrc_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void isrc_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void isrc_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void isrc_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)isrc_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)isrc_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)isrc_draw_html(act, pin, win, sty);
	else if(fmt == _json_)isrc_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)isrc_draw_vbo2d(act, pin, win, sty);
		//else isrc_draw_vbo3d(act, pin, win, sty);
	}
	else isrc_draw_pixel(act, pin, win, sty);
}




static void isrc_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//isrc_draw(act, pin, win, sty);
}
static void isrc_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void isrc_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void isrc_start(struct halfrel* self, struct halfrel* peer)
{
}





static void isrc_search(struct entity* act, u8* buf)
{
}
static void isrc_modify(struct entity* act, u8* buf)
{
}
static void isrc_delete(struct entity* act, u8* buf)
{
}
static void isrc_create(struct entity* act, u8* buf)
{
}




void isrc_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('i','s','r','c');

	p->oncreate = (void*)isrc_create;
	p->ondelete = (void*)isrc_delete;
	p->onsearch = (void*)isrc_search;
	p->onmodify = (void*)isrc_modify;

	p->onstart = (void*)isrc_start;
	p->onstop  = (void*)isrc_stop;
	p->onread  = (void*)isrc_read;
	p->onwrite = (void*)isrc_write;
}
