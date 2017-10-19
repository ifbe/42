#include "actor.h"
//
void new2048(void*);
void left2048(void*);
void right2048(void*);
void up2048(void*);
void down2048(void*);
//
void drawdecimal(
	void*, int data, int size,
	int x, int y, u32 fg, u32 bg);
void drawrect(
	void* win,
	int x1, int y1,
	int x2, int y2,
	u32 bc, u32 fc);
void carvecubie(
	void* win,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz
);




//
static int num;
static u8 buffer[256];




u32 the2048_length(int val)
{
	if(val == 0)return 0;
	if(val <= 8)return 1;
	if(val <= 64)return 2;
	if(val <= 512)return 3;
	if(val <= 8192)return 4;
	if(val <= 65536)return 5;
	return 6;
}
u32 the2048_color(int val)
{
	switch(val)
	{
		case    0:	return 0x555555;
		case    2:	return 0xfffff0;
		case    4:	return 0xffffc0;
		case    8:	return 0x995000;
		case   16:	return 0xc05000;
		case   32:	return 0xb03000;
		case   64:	return 0xff0000;
		case  128:	return 0xffffa0;
		case  256:	return 0xffff80;
		case  512:	return 0xffff00;
		case 1024:	return 0xffffb0;
		case 2048:	return 0x233333;
		case 4096:	return 0x783d72;
		case 8192:	return 0xd73762;
	}
	return 0;
}
static void cubie(
	struct arena* win, int data,
	int x0, int y0, int x1, int y1)
{
	u32 color;
	u32 length;

	//color
	color = the2048_color(data);
	if( ( (win->fmt)&0xffffffff) == 0x61626772)	//bgra->rgba
	{
		color	= 0xff000000
			+ ((color&0xff)<<16)
			+ (color&0xff00)
			+ ((color&0xff0000)>>16);
	}
	drawrect(win,
		x0, y0,
		x1, y1,
		color, 0
	);

	//decimal
	length = the2048_length(data);
	if(length == 0)return;
	drawdecimal(
		win, data, 4,
		-(length*16)+(x0+x1)/2, -32+(y0+y1)/2,
		0, 0
	);
}
static void the2048_read_pixel(struct arena* win, struct actor* act, struct style* rel)
{
	int x,y;
	int cx,cy,w,h;
	int (*table)[4] = (void*)buffer + num*16*4;

	//position
	cx = (win->w) * (rel->cx) / 0x10000;
	cy = (win->h) * (rel->cy) / 0x10000;
	w = (win->w) * (rel->wantw) / 0x10000;
	h = (win->h) * (rel->wanth) / 0x10000;
	if(w >= h)w=h;
	else h=w;

	//cubies
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			cubie(
				win,
				table[y][x],
				cx + (x-2)*w/4,
				cy + (y-2)*h/4,
				cx + (x-1)*w/4,
				cy + (y-1)*h/4
			);
		}
	}
}
static void the2048_read_vbo(struct arena* win, struct actor* act, struct style* rel)
{
	int x,y;
	float xxx, yyy, zzz;
	int (*tab)[4] = (void*)buffer + num*16*4;

	float cx = (float)(rel->cx) / 65536.0 - 0.5;
	float cy = (float)(rel->cy) / 65536.0 - 0.5;
	float w = (float)(rel->wantw) / 65536.0;
	float h = (float)(rel->wanth) / 65536.0;

	//
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			xxx = cx + (x+x-3)*w/8;
			yyy = cy - (y+y-3)*h/8;
			zzz = (float)tab[y][x]/100.0;

			carvecubie(win,
				xxx, yyy, zzz,
				w/16, 0.0, 0.0,
				0.0, h/16, 0.0,
				0.0, 0.0, zzz
			);
		}
	}
}
static void the2048_read_html(struct arena* win, struct actor* act, struct style* rel)
{
	int x,y;
	u32 color;
	int (*table)[4] = (void*)buffer + num*16*4;
	u8* buf = (u8*)(win->buf);
	int len = 0;

	len += mysnprintf(
		buf+len, 0x1000,
		"<style type=\"text/css\">"
		".rect{"
		"border:1px solid #000;"
		"position:absolute;"
		"color:#000;"
		"width:25%%;"
		"height:25%%;"
		"}"
		"</style>"
	);

	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			if(table[y][x] == 0)continue;

			color = the2048_color(table[y][x]);
			len += mysnprintf(
				buf+len, 0x1000,
				"<div class=\"rect\" style=\""
				"left:%d%%;"
				"top:%d%%;"
				"background:#%06x;"
				"\">%d</div>",
				25*x, 25*y,
				color, table[y][x]
			);
		}
	}
	win->info[0] = len;
}
static void the2048_read_tui(struct arena* win, struct actor* act, struct style* rel)
{
	int x,y,j,k,ret;
	u8 src[10];
	int (*table)[4] = (void*)buffer + num*16*4;

	u8* buf = (u8*)(win->buf);
	int w = win->w;
	int h = win->h;

	for(x=0;x<w*h*4;x++)buf[x]=0;
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			data2decstr(table[y][x], src);
			ret = w*(4*y+1) + 8*x + 2;
			ret <<= 2;

			//color
			for(j=-1;j<=1;j++)
			{
				for(k=-2;k<=3;k++)
				{
					buf[ret + (j*w*4) +(k*4) +3] = 4;
				}
			}

			//number
			j=k=0;
			for(j=0;j<10;j++)
			{
				if(src[j] == 0)break;

				buf[ret + k] = src[j];
				k += 4;
			}
		}
	}
}
static void the2048_read_cli()
{
	int (*table)[4] = (void*)buffer + num*16*4;
	say("%d	%d	%d	%d\n", table[0][0], table[0][1], table[0][2], table[0][3]);
	say("%d	%d	%d	%d\n", table[1][0], table[1][1], table[1][2], table[1][3]);
	say("%d	%d	%d	%d\n", table[2][0], table[2][1], table[2][2], table[2][3]);
	say("%d	%d	%d	%d\n", table[3][0], table[3][1], table[3][2], table[3][3]);
	say("\n");
}
static void the2048_read(struct arena* win, struct actor* act, struct style* rel)
{
	//cli
	if(win->fmt == hex32('c','l','i',0))the2048_read_cli();

	//text
	else if(win->fmt == hex32('t','u','i',0))the2048_read_tui(win, act, rel);

	//html
	else if(win->fmt == hex32('h','t','m','l'))the2048_read_html(win, act, rel);

	//vbo
	else if(win->fmt == hex32('v','b','o',0))the2048_read_vbo(win, act, rel);

	//pixel
	else the2048_read_pixel(win, act, rel);
}




static void the2048_write(struct event* ev)
{
	//kbd
	u8 key;
	int j;
	int* p;
	int* q;
	//say("%x,%x\n",ev->why, ev->what);

	//
	if(ev->what == 0x64626b)
	{
		key = (ev->why)&0xff;
		if( (key>=0x25) && (key<=0x28) )
		{
			//
			p = (void*)buffer + 64*num;
			num = (num+1)%4;
			q = (void*)buffer + 64*num;
			for(j=0;j<16;j++)q[j] = p[j];

			//
			if(key == 0x25)left2048((void*)q);
			else if(key == 0x26)up2048((void*)q);
			else if(key == 0x27)right2048((void*)q);
			else if(key == 0x28)down2048((void*)q);

			//new number?
			new2048((void*)q);
		}
	}
	else if(ev->what == 0x72616863)
	{
		if(key == 0x8)
		{
			if(num>0)num--;
		}
	}
}








static void the2048_list()
{
}
static void the2048_choose()
{
}
static void the2048_stop()
{
}
static void the2048_start()
{
	int j;
	for(j=0;j<256;j++)buffer[j] = 0;

	//
	num = 0;
	new2048((void*)buffer);
}
void the2048_delete()
{
}
void the2048_create(void* base, struct actor* act)
{
	act->type = hex32('g', 'a', 'm', 'e');
	act->name = hex32('2', '0', '4', '8');
	act->first = 0;
	act->last = 0;

	act->start = (void*)the2048_start;
	act->stop = (void*)the2048_stop;
	act->list = (void*)the2048_list;
	act->choose = (void*)the2048_choose;
	act->read = (void*)the2048_read;
	act->write = (void*)the2048_write;
}
