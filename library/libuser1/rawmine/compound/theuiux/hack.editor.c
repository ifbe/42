#include "libuser.h"



static u8 buffer[16];




static void editor_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void editor_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void editor_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void editor_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void editor_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("editor(%x,%x,%x)\n",win,act,sty);
}




static void editor_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void editor_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void editor_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void editor_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void editor_search(struct entity* act)
{
}
static void editor_modify(struct entity* act)
{
}
static void editor_delete(struct entity* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void editor_create(struct entity* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = memorycreate(16, 0);
}




void editor_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('e', 'd', 'i', 't', 'o', 'r', 0, 0);

	p->oncreate = (void*)editor_create;
	p->ondelete = (void*)editor_delete;
	p->onsearch = (void*)editor_search;
	p->onmodify = (void*)editor_modify;

	p->onlinkup = (void*)editor_linkup;
	p->ondiscon = (void*)editor_discon;
	p->onread  = (void*)editor_read;
	p->onwrite = (void*)editor_write;
}
