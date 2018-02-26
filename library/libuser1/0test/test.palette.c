#include "actor.h"




static u32 red = 0x8d;
static u32 green = 0x63;
static u32 blue = 0x25;




static void palette_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	int g,b;
	int x,y,type;
	int w = win->w;
	int h = win->h;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;
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
static void palette_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void palette_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void palette_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void palette_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	say("palette(%x,%x,%x)\n",win,act,sty);
	say("r=%02x,g=%02x,b=%02x\n",red,green,blue);
}
static void palette_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)palette_read_cli(win, sty, act, com);
	else if(fmt == _tui_)palette_read_tui(win, sty, act, com);
	else if(fmt == _html_)palette_read_html(win, sty, act, com);
	else if(fmt == _vbo_)palette_read_vbo(win, sty, act, com);
	else palette_read_pixel(win, sty, act, com);
}
static void palette_write(
	struct actor* act, struct compo* com,
	struct event* ev)
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
static void palette_start()
{
}
static void palette_stop()
{
}
static void palette_delete()
{
}
static void palette_create()
{
}




void palette_register(struct actor* p)
{
	p->type = hex32('t', 'e', 's', 't');
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