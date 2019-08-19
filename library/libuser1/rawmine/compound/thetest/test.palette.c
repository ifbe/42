#include "libuser.h"




static u32 red = 0x8d;
static u32 green = 0x63;
static u32 blue = 0x25;
static u8 buffer[16];




static void palette_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int g,b;
	int x,y,type;
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
static void palette_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void palette_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void palette_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"palette\" style=\"width:50%%;height:100px;float:left;background-color:#f624ab;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void palette_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void palette_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	say("palette(%x,%x,%x)\n",win,act,sty);
	say("r=%02x,g=%02x,b=%02x\n",red,green,blue);
}
static void palette_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)palette_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)palette_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)palette_draw_html(act, pin, win, sty);
	else if(fmt == _json_)palette_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)palette_draw_vbo(act, pin, win, sty);
	else palette_draw_pixel(act, pin, win, sty);
}
static void palette_event(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
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




static void palette_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//palette_draw(act, pin, win, sty);
}
static void palette_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//palette_event(act, pin, win, sty, ev, 0);
}
static void palette_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void palette_start(struct halfrel* self, struct halfrel* peer)
{
}




static void palette_search(struct actor* act)
{
}
static void palette_modify(struct actor* act)
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
	p->fmt = hex64('p', 'a', 'l', 'e', 't', 't', 'e', 0);

	p->oncreate = (void*)palette_create;
	p->ondelete = (void*)palette_delete;
	p->onsearch = (void*)palette_search;
	p->onmodify = (void*)palette_modify;

	p->onstart = (void*)palette_start;
	p->onstop  = (void*)palette_stop;
	p->onread  = (void*)palette_read;
	p->onwrite = (void*)palette_write;
}
