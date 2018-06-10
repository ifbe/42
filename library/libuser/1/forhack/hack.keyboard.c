#include "libuser.h"




void keyboard_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u8 buf[2];
	int x,y;
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

	buf[1] = 0;
	if(ww >= hh)
	{
		for(y=0;y<8;y++)
		{
			for(x=0;x<16;x++)
			{
				buf[0] = y*16+x;
				drawicon_1(
					win, 0x40ffffff,
					(cx-ww+1)+(2*x*ww/16),
					(cy-hh+1)+(2*y*hh/8),
					(cx-ww-1)+((2*x+2)*ww/16),
					(cy-hh-1)+((2*y+2)*hh/8),
					buf, 1
				);
			}
		}
	}
	else
	{
		for(y=0;y<8;y++)
		{
			for(x=0;x<16;x++)
			{
				buf[0] = y*16+x;
				drawicon_1(
					win, 0x40ffffff,
					(cx-ww+1)+(2*y*ww/8),
					(cy-hh+1)+(2*x*hh/16),
					(cx-ww-1)+((2*y+2)*ww/8),
					(cy-hh-1)+((2*x+2)*hh/16),
					buf, 1
				);
			}
		}
	}
}
static void keyboard_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void keyboard_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void keyboard_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"keyboard\" style=\"width:50%%;height:100px;float:left;background-color:#a7c8d9;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void keyboard_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void keyboard_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("keyboard(%x,%x,%x)\n",win,act,sty);
}
static void keyboard_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)keyboard_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)keyboard_read_tui(win, sty, act, pin);
	else if(fmt == _html_)keyboard_read_html(win, sty, act, pin);
	else if(fmt == _json_)keyboard_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)keyboard_read_vbo(win, sty, act, pin);
	else keyboard_read_pixel(win, sty, act, pin);
}
static void keyboard_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int x,y;
	//say("%x,%x\n",x,y);
}
static void keyboard_list()
{
}
static void keyboard_change()
{
}
static void keyboard_stop(struct actor* act, struct pinid* pin)
{
}
static void keyboard_start(struct actor* act, struct pinid* pin)
{
}
static void keyboard_delete(struct actor* act)
{
	if(0 == act)return;
}
static void keyboard_create(struct actor* act)
{
	if(0 == act)return;
}




void keyboard_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('k', 'e', 'y', 'b', 'o', 'a', 'r', 'd');
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)keyboard_create;
	p->ondelete = (void*)keyboard_delete;
	p->onstart  = (void*)keyboard_start;
	p->onstop   = (void*)keyboard_stop;
	p->onlist   = (void*)keyboard_list;
	p->onchoose = (void*)keyboard_change;
	p->onread   = (void*)keyboard_read;
	p->onwrite  = (void*)keyboard_write;
}
