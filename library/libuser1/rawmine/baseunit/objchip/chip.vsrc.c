#include "libuser.h"




static void vsrc_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}/*
static void vsrc_draw_vbo2d(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}*/
static void vsrc_draw_vbo3d(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void vsrc_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void vsrc_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void vsrc_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void vsrc_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void vsrc_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)vsrc_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)vsrc_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)vsrc_draw_html(act, pin, win, sty);
	else if(fmt == _json_)vsrc_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)vsrc_draw_vbo2d(act, pin, win, sty);
		//else vsrc_draw_vbo3d(act, pin, win, sty);
	}
	else vsrc_draw_pixel(act, pin, win, sty);
}




static void vsrc_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct entity* tmp = buf;

	switch(tmp->type){
		case _hoffdata_:say("@vsrc_read:%llx %.4s -> %llx %.4s\n", act, &self->flag, win, &peer->flag);break;
	}
	//vsrc_draw(act, pin, win, sty);
}
static void vsrc_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void vsrc_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void vsrc_start(struct halfrel* self, struct halfrel* peer)
{
}




static void vsrc_search(struct entity* act, u8* buf)
{
}
static void vsrc_modify(struct entity* act, u8* buf)
{
}
static void vsrc_delete(struct entity* act, u8* buf)
{
}
static void vsrc_create(struct entity* act, u8* buf)
{
}




void vsrc_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('v','s','r','c');

	p->oncreate = (void*)vsrc_create;
	p->ondelete = (void*)vsrc_delete;
	p->onsearch = (void*)vsrc_search;
	p->onmodify = (void*)vsrc_modify;

	p->onstart = (void*)vsrc_start;
	p->onstop  = (void*)vsrc_stop;
	p->onread  = (void*)vsrc_read;
	p->onwrite = (void*)vsrc_write;
}
