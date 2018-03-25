#include "actor.h"
void drawascii_bitmap(u8* buf, int ch);
void carveunicode_surround(
	struct arena*,u32,
	float,float,float,
	float,float,float,
	float,float,float,
	int, int);




static int chosen = 0x20;
static u8 buffer[16];




static void font_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y,m,n;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	drawline_rect(win, 0xff, cx-ww, cy-hh, cx+ww, cy+hh);

	ww &= 0xfff0;
	hh &= 0xfff0;
	for(y=-hh;y<hh;y+=16)
	{
		for(x=-ww;x<ww;x+=16)
		{
			m = (x>>4) + (chosen&0xff);
			if(m < 0)continue;
			if(m > 0xff)continue;

			n = (y>>4) + (chosen>>8);
			if(n < 0)continue;
			if(n > 0xff)continue;

//say("%d,%d\n",x,y);
			if((0 == x)|(0 == y))
			{
				drawsolid_rect(
					win, (255-m)+(0x008000)+(n<<16),
					cx+x, cy+y,
					cx+x+15, cy+y+15
				);
			}

			m = m + (n<<8);
			if((m > 0x20)&&(m < 0x80))
			{
				drawascii(win, 0xffffff, cx+x, cy+y, m);
			}
			else
			{
				drawunicode(win, 0xffffff, cx+x, cy+y, m);
			}
		}
	}

	drawsolid_rect(win, 0x0000ff, cx-32, cy-16, cx-1, cy-1);
	drawhexadecimal(win, 0xff0000, cx-32, cy-16, chosen);
}
static void font_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void font_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y,dx,dy;
	int left,right,near,far;
	int cx = sty->cx;
	int cy = sty->cy;
	int ww = sty->rx;
	int hh = sty->fy;
	carveline(win, 0xffffff, cx, cy-hh, 0.0, cx, cy+hh, 0.0);
	carveline(win, 0xffffff, cx-ww, cy, 0.0, cx+ww, cy, 0.0);
	carveline_rect(
		win, 0xffffff,
		cx, cy, 0.0,
		ww, 0.0, 0.0,
		0.0, hh, 0.0
	);
	for(y=-4;y<4;y++)
	{
		for(x=-4;x<4;x++)
		{
			dx = x + (chosen&0xff);
			dy = y + ((chosen>>8)&0xff);
			if(dx < 0)continue;
			if(dy < 0)continue;
			if(dx >= 256)continue;
			if(dy >= 256)continue;
			carveunicode(
				win, 0xffffff,
				cx+(2*x+1)*ww/8, cy+(2*y+1)*hh/8, 0.0,
				ww/8, 0.0, 0.0,
				0.0, hh/8, 0.0,
				chosen+x+(y*256)
			);
		}
	}
/*
	x = (float)(chosen&0xff) / 256.0;
	y = (float)(chosen&0xff80) / 65536.0;
	carveascii_area(
		win, 0xffffff,
		cx, cy, 0.0,
		ww, 0.0, 0.0,
		0.0, hh, 0.0,
		x-1.0/16, y-1.0/16, x+1.0/16, y+1.0/16
	);
*/
}
static void font_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void font_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	u8 ch;
	u8 buf[0x20];
	drawascii_bitmap(buf, chosen);

	say("\n%02x0 1 2 3 4 5 6 7 8 9 a b c d e f\n", chosen);
	for(y=0;y<16;y++)
	{
		if(y<=9)say("%c ", 0x30+y);
		else say("%c ", 0x57+y);

		ch = buf[y];
		for(x=0;x<16;x++)
		{
			if(x >= 8)say("**");
			else
			{
				if((ch&0x80) != 0)say("%@");
				else say("--");
				ch = ch<<1;
			}
		}
		say("\n");
	}
}
static void font_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)font_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)font_read_tui(win, sty, act, pin);
	else if(fmt == _html_)font_read_html(win, sty, act, pin);
	else if(fmt == _vbo_)font_read_vbo(win, sty, act, pin);
	else font_read_pixel(win, sty, act, pin);
}
static void font_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int k;
	if(_kbd_ == ev->what)
	{
		k = ev->why;
		if(k == 0x48)
		{
			if(chosen >= 256)chosen -= 256;
		}
		else if(k == 0x4b)
		{
			if((chosen&0xff) > 0)chosen--;
		}
		else if(k == 0x4d)
		{
			if((chosen&0xff) < 0xff)chosen++;
		}
		else if(k == 0x50)
		{
			if(chosen <= 65535-256)chosen += 256;
		}
	}
}
static void font_list()
{
}
static void font_change()
{
}
static void font_stop(struct actor* act, struct pinid* pin)
{
}
static void font_start(struct actor* act, struct pinid* pin)
{
}
static void font_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)stopmemory(act->buf);
}
static void font_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = startmemory(16);
}




void font_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex32('f', 'o', 'n', 't');
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)font_create;
	p->ondelete = (void*)font_delete;
	p->onstart  = (void*)font_start;
	p->onstop   = (void*)font_stop;
	p->onlist   = (void*)font_list;
	p->onchoose = (void*)font_change;
	p->onread   = (void*)font_read;
	p->onwrite  = (void*)font_write;
}
