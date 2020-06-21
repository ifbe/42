#include "libuser.h"




static u32 red = 0x8d;
static u32 green = 0x63;
static u32 blue = 0x25;
static u8 buffer[16];




static void palette_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int g,b;
	int x,y,type;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
	u32* buf = (u32*)(win->rgbabuf);
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

			buf = win->rgbabuf + 4*win->fbwidth*(cy+y);
			buf[cx+x] = pal | (g<<8) | 0xff000000;
		}
	}
	pal = (red<<16)+(green<<8)+blue;
	drawhexadecimal(win, pal, cx, cy, pal);
}
static void palette_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void palette_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void palette_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void palette_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void palette_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("palette(%x,%x,%x)\n",win,act,sty);
	say("r=%02x,g=%02x,b=%02x\n",red,green,blue);
}
static void palette_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
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




static void palette_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void palette_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void palette_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void palette_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void palette_search(struct entity* act)
{
}
static void palette_modify(struct entity* act)
{
}
static void palette_delete(struct entity* act)
{
	if(0 == act)return;
	if(act->buf0){
		memorydelete(act->buf0);
		act->buf0 = 0;
	}
}
static void palette_create(struct entity* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf0 = buffer;
	if(_copy_ == act->type)act->buf0 = memorycreate(16, 0);
}




void palette_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'a', 'l', 'e', 't', 't', 'e', 0);

	p->oncreate = (void*)palette_create;
	p->ondelete = (void*)palette_delete;
	p->onsearch = (void*)palette_search;
	p->onmodify = (void*)palette_modify;

	p->onlinkup = (void*)palette_linkup;
	p->ondiscon = (void*)palette_discon;
	p->ontaking = (void*)palette_taking;
	p->ongiving = (void*)palette_giving;
}
