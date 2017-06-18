#include "actor.h"



void rect(struct arena* win,
	int x1, int y1, int x2, int y2, u32 bc, u32 fc);
void drawascii(
	void* win, u8 ch, int size,
	int x, int y, u32 fg, u32 bg);
void drawstring(
	void*, void* str, int size,
	int x, int y, u32 fg, u32 bg);




static u8* input = 0;
static u8* output = 0;




/*
static void printstdout(struct window* win,
	int xx0, int yy0, int xx1, int yy1)
{
	u8 ch;
	int x,y;
	int winw = win->w;
	int winh = win->h;
	int x0 = winw * xx0 / 0x10000;
	int y0 = winh * yy0 / 0x10000;
	int x1 = winw * xx1 / 0x10000;
	int y1 = winh * yy1 / 0x10000;
	h >>= 4;
	w >>= 3;
	if(w>0x80)w = 0x80;

	pos = *(u32*)(output+0x100000-16);
	pos = pos - (pos%0x80);
	if(pos < h*0x80)pos = 0;
	else pos -= (h-1)*0x80;

	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			ch = output[pos + (y*0x80) + x];
			if( (ch==0) | (ch=='\n') )break;

			drawascii(
				win, ch, 1,
				x*8, (y*16)+(h%16), 0xffffff, 0
			);
		}
	}
}
*/
static void printstdout(struct arena* win,
	int xx0, int yy0, int xx1, int yy1)
{
	int x;
	int y;
	int ch;
	int tmp;
	int py0;
	int py1;
	int winw = win->w;
	int winh = win->h;
	int x0 = winw * xx0 / 0x10000;
	int y0 = winh * yy0 / 0x10000;
	int x1 = winw * xx1 / 0x10000;
	int y1 = winh * yy1 / 0x10000;
	u32* palette;
	u8* where;
	rect(win,
		x0, y0,
		x1, y1,
		0, 0xff00ff
	);

	//
	tmp = *(u32*)(output+0x100000-16);
	tmp = tmp - (tmp%0x80);
	y = (y1-y0)/16;
	if(tmp < y*0x80)
	{
		tmp = 0;
		where = output;
	}
	else
	{
		tmp -= (y-1)*0x80;
		where = output + tmp;
	}
	py0 = y0 + (y1-y0) * tmp / (tmp + y*0x80);
	py1 = y1;

	//string
	tmp = (x1-x0)/8;
	if(tmp > 0x80)tmp = 0x80;
	for(y=1;y<(y1-y0)/16;y++)
	{
		for(x=0;x<tmp;x++)
		{
			ch = where[ (y*0x80) + x ];
			if( (ch==0) | (ch=='\n') )break;

			drawascii(
				win, ch, 1,
				x0+(x*8), y0+(y*16), 0xffffff, 0x444444
			);
		}
	}

	//outer
	palette = (u32*)(win->buf);
	for(y=y0;y<y1;y++)
	{
		for(x=x1-16;x<x1;x++)
		{
			palette[winw*y + x] = 0xff888888;
		}
	}

	//inner
	for(y=py0;y<py1;y++)
	{
		for(x=x1-16+2;x<x1-2;x++)
		{
			palette[winw*y + x] = 0xffffffff;
		}
	}
}
static void printstdin(struct arena* win,
	int xx0, int yy0, int xx1, int yy1)
{
	int x0 = (win->w)*xx0/0x10000;
	int y0 = (win->h)*yy0/0x10000;
	drawstring(
		win, "[user@42]", 1,
		x0, y0, 0, 0xffffffff);
	drawstring(
		win, input, 1,
		x0+72, y0, 0, 0xffffffff);
}
void draw_vt100(struct arena* win,
	int x0, int y0, int x1, int y1)
{
	//backgroundcolor(win, 0);
	printstdout(win, x0, y0, x1, y1);
	printstdin(win, x0, y0, x1, y1);
}




void vt100_create(void* addr)
{
	input = addr + 0x400000;
	output = addr + 0x500000;
}
void vt100_delete()
{
}
