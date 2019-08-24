#include "libuser.h"




static void resistor_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}/*
static void resistor_draw_vbo2d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}*/
static void resistor_draw_vbo3d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void resistor_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void resistor_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void resistor_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void resistor_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void resistor_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)resistor_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)resistor_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)resistor_draw_html(act, pin, win, sty);
	else if(fmt == _json_)resistor_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)resistor_draw_vbo2d(act, pin, win, sty);
		//else resistor_draw_vbo3d(act, pin, win, sty);
	}
	else resistor_draw_pixel(act, pin, win, sty);
}




static void resistor_read(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* tmp = buf;

	switch(tmp->type){
		case _hoffdata_:say("@resistor_read:%llx %.4s <-> %llx %.4s\n", act, &self->flag, win, &peer->flag);break;
	}
	//resistor_draw(act, pin, win, sty);
}
static void resistor_write(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
}
static void resistor_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void resistor_start(struct halfrel* self, struct halfrel* peer)
{
}




static void resistor_search(struct actor* act, u8* buf)
{
}
static void resistor_modify(struct actor* act, u8* buf)
{
}
static void resistor_delete(struct actor* act, u8* buf)
{
}
static void resistor_create(struct actor* act, u8* buf)
{
}




void resistor_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('r','e','s','i','s','t','o','r');

	p->oncreate = (void*)resistor_create;
	p->ondelete = (void*)resistor_delete;
	p->onsearch = (void*)resistor_search;
	p->onmodify = (void*)resistor_modify;

	p->onstart = (void*)resistor_start;
	p->onstop  = (void*)resistor_stop;
	p->onread  = (void*)resistor_read;
	p->onwrite = (void*)resistor_write;
}
