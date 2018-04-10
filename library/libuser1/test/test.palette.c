#include "actor.h"




static u32 red = 0x8d;
static u32 green = 0x63;
static u32 blue = 0x25;
static u8 buffer[16];




static void palette_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int g,b;
	int x,y,type;
	int cx = sty->cx;
	int cy = sty->cy;
	int ww = sty->rx;
	int hh = sty->fy;
	int w = win->stride;
	u32* buf = (u32*)(win->buf);
	u32 pal;

	type = (win->fmt)&0xffffff;
	if(hex32('r','g','b',0) == type)type = 1;
	else if(hex32('b','g','r',0) == type)type = 2;
	else type = 0;

	for(y=-hh;y<hh;y++)
	{
		for(x=-ww;x<ww;x++)
		{
			b = 128+(x*128/ww);
			g = 128+(y*128/hh);
			if(type == 1)pal = red + (b<<16);
			else if(type == 2)pal = b + (red<<16);
			buf[w*(cy+y) + cx+x] = pal | (g<<8) | 0xff000000;
		}
	}
	pal = (red<<16)+(green<<8)+blue;
	drawhexadecimal(win, pal, cx, cy, pal);
}
static void palette_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	carvesolid_rect(
		win, 0xffffff,
		cx, cy, cz,
		ww, 0.0, 0.0,
		0.0, hh, 0.0
	);
}
static void palette_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void palette_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"palette\" style=\"width:100%%;height:100px;background-color:#f624ab;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void palette_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void palette_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("palette(%x,%x,%x)\n",win,act,sty);
	say("r=%02x,g=%02x,b=%02x\n",red,green,blue);
}
static void palette_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)palette_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)palette_read_tui(win, sty, act, pin);
	else if(fmt == _html_)palette_read_html(win, sty, act, pin);
	else if(fmt == _json_)palette_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)palette_read_vbo(win, sty, act, pin);
	else palette_read_pixel(win, sty, act, pin);
}
static void palette_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == _kbd_)
	{
		if(key==0x48)		//up
		{
			if(green>0)green--;
		}
		else if(key==0x4b)	//left
		{
			if(blue>0)blue--;
		}
		else if(key==0x4d)	//right
		{
			if(blue<255)blue++;
		}
		else if(key==0x50)	//down
		{
			if(green<255)green++;
		}
	}
	else if(type == _char_)
	{
		if(key=='+')
		{
			if(red<255) red++;
		}
		if(key=='-')
		{
			if(red>0) red--;
		}
	}
	else if(type == 0x2d70)
	{
		if((key>>48) == 1)	//left
		{
			int x = key&0xffff;
			int y = (key>>16)&0xffff;
			int min = 512;

			if((x>min)|(y>min))return;
			blue=x*256/min;
			green=y*256/min;
		}
		else if((key>>48) == 4)
		{
			if(red<0xff)red++;
		}
		else if((key>>48) == 5)
		{
			if(red>0)red--;
		}
	}
}
static void palette_list()
{
}
static void palette_into()
{
}
static void palette_stop(struct actor* act, struct pinid* pin)
{
}
static void palette_start(struct actor* act, struct pinid* pin)
{
}
static void palette_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void palette_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = memorycreate(16);
}




void palette_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('p', 'a', 'l', 'e', 't', 't', 'e', 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)palette_create;
	p->ondelete = (void*)palette_delete;
	p->onstart  = (void*)palette_start;
	p->onstop   = (void*)palette_stop;
	p->onlist   = (void*)palette_list;
	p->onchoose = (void*)palette_into;
	p->onread   = (void*)palette_read;
	p->onwrite  = (void*)palette_write;
}
