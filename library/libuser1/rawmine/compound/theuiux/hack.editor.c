#include "libuser.h"



static u8 buffer[16];




static void editor_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void editor_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void editor_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void editor_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"editor\" style=\"width:50%%;height:100px;float:left;background-color:#4e819a;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void editor_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void editor_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	say("editor(%x,%x,%x)\n",win,act,sty);
}
static void editor_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)editor_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)editor_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)editor_draw_html(act, pin, win, sty);
	else if(fmt == _json_)editor_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)editor_draw_vbo(act, pin, win, sty);
	else editor_draw_pixel(act, pin, win, sty);
}




static void editor_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//editor_draw(act, pin, win, sty);
}
static void editor_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void editor_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void editor_start(struct halfrel* self, struct halfrel* peer)
{
}




static void editor_search(struct actor* act)
{
}
static void editor_modify(struct actor* act)
{
}
static void editor_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void editor_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = memorycreate(16);
}




void editor_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('e', 'd', 'i', 't', 'o', 'r', 0, 0);

	p->oncreate = (void*)editor_create;
	p->ondelete = (void*)editor_delete;
	p->onsearch = (void*)editor_search;
	p->onmodify = (void*)editor_modify;

	p->onstart = (void*)editor_start;
	p->onstop  = (void*)editor_stop;
	p->onread  = (void*)editor_read;
	p->onwrite = (void*)editor_write;
}
