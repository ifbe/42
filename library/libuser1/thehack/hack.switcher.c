#include "libuser.h"




static void switch_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx, cy, ww, hh;
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
	drawline_rect(win, 0xffffff, cx-ww, cy-hh, cx+ww-1, cy+hh-1);
}
static void switch_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void switch_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void switch_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
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
static void switch_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void switch_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void switch_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)switch_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)switch_read_tui(win, sty, act, pin);
	else if(fmt == _html_)switch_read_html(win, sty, act, pin);
	else if(fmt == _json_)switch_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)switch_read_vbo(win, sty, act, pin);
	else switch_read_pixel(win, sty, act, pin);
}
static void switch_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
	int j;
	u8* dst;
	if(0 == win)return;

	dst = act->buf;
	for(j=0;j<len;j++)dst[j] = buf[j];
	dst[j] = 0;

	struct relation* orel = act->orel0;
	while(1)
	{
		if(0 == orel)break;
		if(_fd_ == orel->dsttype)
		{
			system_leafwrite(
				(void*)(orel->dstchip), (void*)(orel->dstfoot),
				(void*)(orel->srcchip), (void*)(orel->srcfoot),
				act->buf, len
			);
		}
		orel = samesrcnextdst(orel);
	}
}
static void switch_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void switch_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void switch_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void switch_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
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
	void* addr;
	if(0 == act)return;
	act->buf = memorycreate(0x100000);

	addr = systemcreate(_udp_, "127.0.0.1:2222");
	if(0 == addr)return;

	relationcreate(addr, 0, _fd_, act, 0, _act_);
}




void switch_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('s','w','i','t','c','h',0,0);

	p->oncreate = (void*)switch_create;
	p->ondelete = (void*)switch_delete;
	p->onstart  = (void*)switch_start;
	p->onstop   = (void*)switch_stop;
	p->oncread  = (void*)switch_cread;
	p->oncwrite = (void*)switch_cwrite;
	p->onsread  = (void*)switch_sread;
	p->onswrite = (void*)switch_swrite;
}
