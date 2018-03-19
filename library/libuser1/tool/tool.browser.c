#include "actor.h"
void* arenastart(u64 type, void* addr);
void* relation_write(void*, void*, u64, void*, void*, u64);




static void browser_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void browser_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx = sty->cx;
	int cy = sty->cy;
	int ww = sty->rx;
	int hh = sty->fy;
	struct mystring* str = (void*)(act->detail);
	drawsolid_rect(win, 0x202020, cx-ww, cy-hh, cx+ww-1, cy+hh-1);
	drawstring(win, 0xffffff, cx-ww, cy-hh, str->buf, str->len);
}
static void browser_read_vbo(
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
	else if(fmt == _vbo_)browser_read_vbo(win, sty, act, pin);
	else browser_read_pixel(win, sty, act, pin);
}
static void browser_write(
	struct actor* act, struct pinid* pin,
	struct event* ev)
{
	struct arena* win;
	struct mystring* haha;
	int len;
	u8* buf;
	if(_char_ != ev->what)return;

	haha = (void*)(act->detail);
	len = haha->len;
	buf = haha->buf;
	if(0xd == ev->why)
	{
		haha->len = 0;
		win = arenastart(hex32('w','s',0,0), buf);
		relation_write(act, 0, _act_, win, 0, _win_);
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
			haha->len = len+1;
		}
	}
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
	if(_copy_ == act->type)stopmemory(act->buf);
}
static void browser_create(struct actor* act)
{
	int j;
	if(0 == act)return;

	act->buf = startmemory(0x100000);
	for(j=0;j<0x100;j++)act->detail[j] = 0;
}




void browser_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('b', 'r', 'o', 'w', 's', 'e', 'r', 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)browser_create;
	p->ondelete = (void*)browser_delete;
	p->onstart  = (void*)browser_start;
	p->onstop   = (void*)browser_stop;
	p->onlist   = (void*)browser_list;
	p->onchoose = (void*)browser_change;
	p->onread   = (void*)browser_read;
	p->onwrite  = (void*)browser_write;
}
