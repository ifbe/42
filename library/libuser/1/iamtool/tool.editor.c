#include "libuser.h"



static u8 buffer[16];




static void editor_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void editor_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void editor_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void editor_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
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
static void editor_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void editor_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("editor(%x,%x,%x)\n",win,act,sty);
}
static void editor_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)editor_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)editor_read_tui(win, sty, act, pin);
	else if(fmt == _html_)editor_read_html(win, sty, act, pin);
	else if(fmt == _json_)editor_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)editor_read_vbo(win, sty, act, pin);
	else editor_read_pixel(win, sty, act, pin);
}
static void editor_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void editor_list()
{
}
static void editor_choose()
{
}
static void editor_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void editor_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
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
	p->name = hex64('e', 'd', 'i', 't', 'o', 'r', 0, 0);

	p->oncreate = (void*)editor_create;
	p->ondelete = (void*)editor_delete;
	p->onstart  = (void*)editor_start;
	p->onstop   = (void*)editor_stop;
	p->onlist   = (void*)editor_list;
	p->onchoose = (void*)editor_choose;
	p->onread   = (void*)editor_read;
	p->onwrite  = (void*)editor_write;
}
