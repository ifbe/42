#include "actor.h"
int cli_write(void*);




static u8* input = 0;
static u8* output = 0;




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
	drawsolid_rect(win, 0,
		x0, y0, x1, y1
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

	//outer, inner
	drawsolid_rect(win, 0x888888, x1-16,  y0, x1-1,  y1-1);
	drawsolid_rect(win, 0xffffff, x1-14, py0, x1-3, py1-1);
}
static void printstdin(struct arena* win,
	int xx0, int yy0, int xx1, int yy1)
{
	int x0 = (win->w)*xx0/0x10000;
	int y0 = (win->h)*yy0/0x10000;
	drawstring(win, 0xffffff, x0, y0, "[user@42]", 0);
	drawstring(win, 0xffffff, x0+72, y0, input, 0);
}
void vt100_read(struct arena* win,
	int x0, int y0, int x1, int y1)
{
	//backgroundcolor(win, 0);
	printstdout(win, x0, y0, x1, y1);
	//printstdin(win, x0, y0, x1, y1);
}
void vt100_write()
{
}




void vt100_create(void* base)
{
	input = base + 0x400000;
	output = base + 0x500000;
}
void vt100_delete()
{
}
