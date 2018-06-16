#include "libuser.h"
#define _http_ hex32('h','t','t','p')
void* arterycreate(u64 type, void* addr);




static void browser_read_pixel(
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
	struct mystring* str = act->idx;
	drawsolid_rect(win, 0x202020, cx-ww, cy-hh, cx+ww-1, cy+hh-1);
	drawstring(win, 0xffffff, cx-ww, cy-hh, str->buf, str->len);
	drawtext(win, 0xffffff, cx-ww, cy-hh+16, cx+ww-1, cy+hh-1, act->buf, act->len);
}
static void browser_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void browser_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void browser_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void browser_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void browser_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("browser(%x,%x,%x)\n",win,act,sty);
}
static void browser_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)browser_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)browser_read_tui(win, sty, act, pin);
	else if(fmt == _html_)browser_read_html(win, sty, act, pin);
	else if(fmt == _json_)browser_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)browser_read_vbo(win, sty, act, pin);
	else browser_read_pixel(win, sty, act, pin);
}




static void browser_write_event(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev)
{
	int len;
	u8* buf;
	void* art;
	struct mystring* haha;
	if(_char_ != ev->what)return;

	haha = act->idx;
	len = haha->len;
	buf = haha->buf;
	if(0xd == ev->why)
	{
		act->len = 0;
		haha->len = 0;

		art = arterycreate(0, buf);
		relationcreate(act, 0, _act_, art, 0, _art_);
	}
	else if(0x8 == ev->why)
	{
		if(len <= 0)
		{
			haha->len = 0;
		}
		else
		{
			len--;
			buf[len] = 0;
			haha->len = len;
		}
	}
	else
	{
		if(len < 0xfc)
		{
			buf[len] = ev->why;
			buf[len+1] = 0;
			haha->len = len+1;
		}
	}
}
static void browser_write_data(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
	int j;
	u8* dst;

	dst = (act->buf)+(act->len);
	act->len += len;

	for(j=0;j<len;j++)dst[j] = buf[j];
	dst[j] = 0;
}
static void browser_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	void* buf, int len)
{
	if(0 == win)browser_write_event(act, pin, win, sty, buf);
	else browser_write_data(act, pin, win, sty, buf, len);
}




static void browser_list()
{
}
static void browser_change()
{
}
static void browser_stop(struct actor* act, struct pinid* pin)
{
}
static void browser_start(struct actor* act, struct pinid* pin)
{
}
static void browser_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
}
static void browser_create(struct actor* act)
{
	int j;
	if(0 == act)return;

	act->idx = memorycreate(0x1000);;
	act->buf = memorycreate(0x100000);
}




void browser_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('b', 'r', 'o', 'w', 's', 'e', 'r', 0);

	p->oncreate = (void*)browser_create;
	p->ondelete = (void*)browser_delete;
	p->onstart  = (void*)browser_start;
	p->onstop   = (void*)browser_stop;
	p->onlist   = (void*)browser_list;
	p->onchoose = (void*)browser_change;
	p->onread   = (void*)browser_read;
	p->onwrite  = (void*)browser_write;
}
