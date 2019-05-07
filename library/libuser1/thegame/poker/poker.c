#include "libuser.h"




static u8 buffer[108];



static void poker_draw_pixel(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	int j, cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->vc[0];
		cy = sty->vc[1];
		ww = sty->vr[0];
		hh = sty->vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);

	for(j=0;j<108/4;j++)
	{
		drawsolid_rect(
			win, 0xc0c0c0,
			cx + (j-13)*ww/27 - (ww/4),
			cy - (j/2) + (hh/2),
			cx + (j-13)*ww/27 + (ww/4),
			cy - (j/2) + hh
		);
		drawline_rect(
			win, 0x404040,
			cx + (j-13)*ww/27 - (ww/4),
			cy - (j/2) + (hh/2),
			cx + (j-13)*ww/27 + (ww/4),
			cy - (j/2) + hh
		);
/*
		drawascii(
			win, 0xffffff,
			cx + (j-13)*ww/27 - (ww/4),
			cy - (j/2) + (hh/2),
			symbol[j/2]
		);
*/
	}
}
static void poker_draw_vbo(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void poker_draw_json(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void poker_draw_html(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"poker\" style=\"width:50%%;height:100px;float:left;background-color:#202020;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");
	win->len = len;
}
static void poker_draw_tui(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void poker_draw_cli(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void poker_draw(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)poker_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)poker_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)poker_draw_html(act, pin, win, sty);
	else if(fmt == _json_)poker_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)poker_draw_vbo(act, pin, win, sty);
	else poker_draw_pixel(act, pin, win, sty);
}




static void poker_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct pinid* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	poker_draw(act, pin, win, sty);
}
static void poker_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void poker_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void poker_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void poker_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void poker_start(struct halfrel* self, struct halfrel* peer)
{
}
static void poker_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void poker_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = memorycreate(108);
}




void poker_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'o', 'k', 'e', 'r', 0, 0, 0);

	p->oncreate = (void*)poker_create;
	p->ondelete = (void*)poker_delete;
	p->onstart  = (void*)poker_start;
	p->onstop   = (void*)poker_stop;
	p->oncread  = (void*)poker_cread;
	p->oncwrite = (void*)poker_cwrite;
	p->onsread  = (void*)poker_sread;
	p->onswrite = (void*)poker_swrite;
}
