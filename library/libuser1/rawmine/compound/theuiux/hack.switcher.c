#include "libuser.h"




static void switch_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
	drawline_rect(win, 0xffffff, cx-ww, cy-hh, cx+ww-1, cy+hh-1);
}
static void switch_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void switch_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void switch_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"switch\" style=\"width:50%%;height:100px;float:left;background-color:#7d9c8a;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void switch_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void switch_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void switch_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)switch_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)switch_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)switch_draw_html(act, pin, win, sty);
	else if(fmt == _json_)switch_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)switch_draw_vbo(act, pin, win, sty);
	else switch_draw_pixel(act, pin, win, sty);
}
static void switch_data(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
	u8* buf, int len)
{
	int j;
	u8* dst;
	if(0 == win)return;

	dst = act->buf;
	for(j=0;j<len;j++)dst[j] = buf[j];
	dst[j] = 0;
}




static void switch_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//switch_draw(act, pin, win, sty);
}
static void switch_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//switch_data(act, pin, win, sty, buf, len);
}
static void switch_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void switch_start(struct halfrel* self, struct halfrel* peer)
{
}




static void switch_search(struct actor* act, u8* buf)
{
}
static void switch_modify(struct actor* act, u8* buf)
{
}
static void switch_delete(struct actor* act, u8* buf)
{
	if(0 == act)return;
	if(0 != act->buf)memorydelete(act->buf);
	act->buf = 0;
}
static void switch_create(struct actor* act, u8* buf)
{
	if(0 == act)return;
	act->buf = memorycreate(0x100000, 0);
}




void switch_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('s','w','i','t','c','h',0,0);

	p->oncreate = (void*)switch_create;
	p->ondelete = (void*)switch_delete;
	p->onsearch = (void*)switch_search;
	p->onmodify = (void*)switch_modify;

	p->onstart = (void*)switch_start;
	p->onstop  = (void*)switch_stop;
	p->onread  = (void*)switch_read;
	p->onwrite = (void*)switch_write;
}
