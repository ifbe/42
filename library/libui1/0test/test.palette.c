#include "actor.h"




static u32 red = 0x8d;
static u32 green = 0x63;
static u32 blue = 0x25;




static void palette_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y,min,type;
	int w = win->w;
	int h = win->h;
	int cx = (sty->cx) * w / 0x10000;
	int cy = (sty->cy) * h / 0x10000;
	int ww = (sty->wantw) * w / 0x20000;
	int hh = (sty->wanth) * h / 0x20000;
	u32* buf = (u32*)(win->buf);
	u32 palette;

	type = (win->fmt)&0xffffff;
	if(type == 0x626772)type = 1;
	else if(type == 0x726762)type = 2;
	else type = 0;

	min = 128;
	if(ww < 128)min = ww;
	if(hh < 128)min = hh;
	for(y=0;y<min*2;y++)
	{
		for(x=0;x<min*2;x++)
		{
			palette = 0xff000000 + ((y+128-min)<<8);
			if(type == 1)palette += red + ((x+128-min)<<16);
			else if(type == 2)palette += x + ((red+128-min)<<16);
			buf[w*(cy+y-min) + (cx+x-min)] = palette;
		}
	}
	palette = (red<<16)+(green<<8)+blue;
	drawhexadecimal(win, palette, cx, cy, palette);
}
static void palette_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void palette_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void palette_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void palette_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("palette(%x,%x,%x)\n",win,act,sty);
	say("r=%02x,g=%02x,b=%02x\n",red,green,blue);
}
static void palette_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == __cli__)palette_read_cli(win, act, sty);
	else if(fmt == __tui__)palette_read_tui(win, act, sty);
	else if(fmt == __html__)palette_read_html(win, act, sty);
	else if(fmt == __vbo__)palette_read_vbo(win, act, sty);
	else palette_read_pixel(win, act, sty);
}
static void palette_write(struct event* ev)
{
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == __kbd__)
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
	else if(type == __char__)
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
void palette_create(void* base, void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'e', 's', 't');
	p->name = hex64('p', 'a', 'l', 'e', 't', 't', 'e', 0);

	p->start = (void*)palette_start;
	p->stop = (void*)palette_stop;
	p->list = (void*)palette_list;
	p->choose = (void*)palette_into;
	p->read = (void*)palette_read;
	p->write = (void*)palette_write;
}
void palette_delete()
{
}

