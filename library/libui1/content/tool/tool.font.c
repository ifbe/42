#include "actor.h"
void drawascii_bitmap(u8* buf, int ch);
static int chosen = 0x20;




static void font_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y,j;
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int ww = (win->w) * (sty->wantw) / 0x20000;
	int hh = (win->h) * (sty->wanth) / 0x20000;
	for(j=0x20;j<0x80;j++)
	{
		x = (j%16)<<4;
		y = ((j/16) - 2)<<4;
		if(x+16 > ww*2)continue;
		if(y+16 > hh*2)break;

		drawascii(win, 0xffffff, cx-ww + x, cy-hh + y, j);
	}
}
static void font_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void font_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
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
	chosen++;
	if((chosen<0x20)|(chosen>=0x80))chosen = 0x20;
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
