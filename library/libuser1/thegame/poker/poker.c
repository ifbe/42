#include "libuser.h"




static u8 buffer[108];



static void poker_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
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
static void poker_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void poker_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void poker_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
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
static void poker_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void poker_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void poker_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)poker_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)poker_read_tui(win, sty, act, pin);
	else if(fmt == _html_)poker_read_html(win, sty, act, pin);
	else if(fmt == _json_)poker_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)poker_read_vbo(win, sty, act, pin);
	else poker_read_pixel(win, sty, act, pin);
}
static void poker_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void poker_get()
{
}
static void poker_post()
{
}
static void poker_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void poker_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
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
	p->onget    = (void*)poker_get;
	p->onpost   = (void*)poker_post;
	p->onread   = (void*)poker_read;
	p->onwrite  = (void*)poker_write;
}
