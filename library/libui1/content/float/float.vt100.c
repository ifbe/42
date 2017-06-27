#include "actor.h"
int cli_write(void*);



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




static void vt100_read_text(struct arena* win)
{
	int x,y;
	int width = win->w;
	int height = win->h;
	char* p = (char*)(win->buf);
	char* src;
	char* dst;

	for(y=height/4;y<height*3/4;y++)
	{
		for(x=width/4;x<width*3/4;x++)
		{
			dst = p + ((x + width*y)<<2);
			dst[0]=dst[1]=dst[2]=dst[3]=0;
		}
	}
	for(x=width/4;x<=width*3/4;x++)
	{
		p[(x + (height/4)*width)<<2] = '-';
		p[(x + (height*3/4)*width)<<2] = '-';
	}
	for(y=height/4;y<=height*3/4;y++)
	{
		p[(width/4 + y*width)<<2] = '|';
		p[(width*3/4 + y*width)<<2] = '|';
	}
}
static void vt100_read_html(struct arena* win)
{
	char* p = (char*)(win->buf);

	win->len = fmt(
		p, 0x1000,
		"<div style=\""
		"position:absolute;"
		"z-index:100;"
		"left:25%%;"
		"top:25%%;"
		"width:50%%;"
		"height:50%%;"
		"border:4px solid #000;"
		"background:#444444;"
		"color:#000;"
		"text-align:center;"
		"\">"
		"what do you want<hr>%s"
		"</div>",

		input
	);
}
static void vt100_read(struct arena* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		vt100_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		vt100_read_html(win);
	}

	//pixel
	else
	{
		draw_vt100(win, 0x2000, 0x2000, 0xe000, 0x8000);
	}
}
static void vt100_write(struct event* ev)
{
	cli_write(ev);
}
static void vt100_choose()
{
}
static void vt100_list()
{
}
static void vt100_start()
{
}
static void vt100_stop()
{
}
void vt100_create(void* base, struct actor* act)
{
	input = base + 0x400000;
	output = base + 0x500000;

	act->type = 0;
	act->name = hexof('v','t','1','0','0',0,0,0);
	act->first = 0;
	act->last = 0;

	act->start = (void*)vt100_start;
	act->stop = (void*)vt100_stop;
	act->list = (void*)vt100_list;
	act->choose = (void*)vt100_choose;
	act->read = (void*)vt100_read;
	act->write = (void*)vt100_write;
}
void vt100_delete()
{
}
