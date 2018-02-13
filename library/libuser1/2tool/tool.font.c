#include "actor.h"
void drawascii_bitmap(u8* buf, int ch);
void carveascii_area(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz);




static int chosen = 0x20;




static void font_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y,m,n;
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int ww = (win->w) * (sty->wantw) / 0x20000;
	int hh = (win->h) * (sty->wanth) / 0x20000;
//say("chosen=%x\n",chosen);

	for(y=-(hh&0xfff0);y<(hh&0xfff0);y+=16)
	{
		for(x=-(ww&0xfff0);x<(ww&0xfff0);x+=16)
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
static void font_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void font_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
	float cx = (sty->cx) / 65536.0 - 0.5;
	float cy = (sty->cy) / 65536.0 - 0.5;
	float ww = (sty->wantw) / 131072.0;
	float hh = (sty->wanth) / 131072.0;
	carveascii_area(
		win, 0xffffff,
		cx, cy, 0.0,
		ww, 0.0, 0.0,
		0.0, hh, 0.0
	);
}
static void font_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void font_read_cli(struct arena* win, struct actor* act, struct style* sty)
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
static void font_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == __cli__)font_read_cli(win, act, sty);
	else if(fmt == __tui__)font_read_tui(win, act, sty);
	else if(fmt == __html__)font_read_html(win, act, sty);
	else if(fmt == __vbo__)font_read_vbo(win, act, sty);
	else font_read_pixel(win, act, sty);
}
static void font_write(struct event* ev)
{
	int k = (ev->why)&0xff;
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
static void font_list()
{
}
static void font_change()
{
}
static void font_start()
{
}
static void font_stop()
{
}
void font_create(void* base, void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'o', 'o', 'l');
	p->name = hex32('f', 'o', 'n', 't');

	p->start = (void*)font_start;
	p->stop = (void*)font_stop;
	p->list = (void*)font_list;
	p->choose = (void*)font_change;
	p->read = (void*)font_read;
	p->write = (void*)font_write;
}
void font_delete()
{
}
