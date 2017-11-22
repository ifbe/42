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
void carvesolid_rect(
	void* win, u32 color,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz
);
void carvesolid_prism4(
	void* win, u32 color,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz
);




//
static int num;
static u8 buffer[256];
//
static u8 len2048[17] = {
0, 1, 1, 1,
2, 2, 2, 3,
3, 3, 4, 4,
4, 4, 5, 5, 5
};
static u32 val2048[17] = {
   0,    2,    4,    8,
  16,   32,   64,  128,
 256,  512, 1024, 2048,
4096, 8192,16384,32768,65536
};
static u32 color2048[17] = {
0x444444, 0xfffff0, 0xffffc0, 0x995000,
0xc05000, 0xb03000, 0xff0000, 0xffffa0,
0xffff80, 0xffff00, 0xffffb0, 0x233333,
0x783d72, 0xd73762
};




static void the2048_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
	u32 color;
	int x,y;
	float xxx, yyy, zzz;
	u8 (*tab)[4] = (void*)buffer + num*16;

	float cx = (float)(sty->cx) / 65536.0 - 0.5;
	float cy = (float)(sty->cy) / 65536.0 - 0.5;
	float w = (float)(sty->wantw) / 65536.0;
	float h = (float)(sty->wanth) / 65536.0;

	carvesolid_rect(
		win, 0x444444,
		cx, cy, 0.0,
		w/2, 0.0, 0.0,
		0.0, h/2, 0.0
	);
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			color = color2048[tab[y][x]];
			//say("%x\n",color);

			xxx = cx + (x+x-3)*w/8;
			yyy = cy - (y+y-3)*h/8;
			if(tab[y][x] == 0)zzz = 0.1/100.0;
			else zzz = (float)tab[y][x]/100.0;

			carvesolid_prism4(
				win, color,
				xxx, yyy, zzz,
				w/16, 0.0, 0.0,
				0.0, h/16, 0.0,
				0.0, 0.0, zzz
			);
		}
	}
}
static void the2048_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	u32 color;
	u32 length;
	int x,y;
	int cx,cy,w,h;
	int x0,y0,x1,y1;
	u8 (*tab)[4] = (void*)buffer + num*16;

	//position
	cx = (win->w) * (sty->cx) / 0x10000;
	cy = (win->h) * (sty->cy) / 0x10000;
	w = (win->w) * (sty->wantw) / 0x10000;
	h = (win->h) * (sty->wanth) / 0x10000;
	if(w-h<-16 | w-h>16)
	{
		w = (w+h)/2;
		h = w;
		sty->wantw = w * 0x10000 / (win->w);
		sty->wanth = h * 0x10000 / (win->h);
	}

	//cubies
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			//color
			color = color2048[tab[y][x]];
			if( ( (win->fmt)&0xffffff) == 0x626772)	//bgra->rgba
			{
				color = 0xff000000
					+ ((color&0xff)<<16)
					+ (color&0xff00)
					+ ((color&0xff0000)>>16);
			}

			//cubie
			x0 = cx + (x-2)*w/4;
			y0 = cy + (y-2)*h/4;
			x1 = cx + (x-1)*w/4;
			y1 = cy + (y-1)*h/4;
			drawrect(win,
				x0, y0,
				x1, y1,
				color, 0
			);

			//decimal
			length = len2048[tab[y][x]];
			if(length == 0)continue;

			drawdecimal(
				win, val2048[tab[y][x]], 4,
				-(length*16)+(x0+x1)/2, -32+(y0+y1)/2,
				0, 0
			);
		}
	}
}
static void the2048_read_html(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	int cx,cy,w,h;
	int len = 0;
	u32 color;
	u8* buf = (u8*)(win->buf);
	u8 (*tab)[4] = (void*)buffer + num*16;

	cx = (sty->cx) * 100 / 0x10000;
	cy = (sty->cy) * 100 / 0x10000;
	w = (sty->wantw) * 25 / 0x10000;
	h = (sty->wanth) * 25 / 0x10000;

	len += mysnprintf(
		buf+len, 0x1000,
		"<style type=\"text/css\">"
		".rect{"
		"border:1px solid #000;"
		"position:absolute;"
		"color:#000;"
		"width:%d%%;"
		"height:%d%%;"
		"}</style>",
		w, h
	);

	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			color = color2048[tab[y][x]];
			len += mysnprintf(
				buf+len, 0x1000,
				"<div class=\"rect\" style=\""
				"left:%d%%;"
				"top:%d%%;"
				"background:#%06x;\">",
				cx+(x-2)*w, cy+(y-2)*h, color
			);

			if(tab[y][x] == 0)len += mysnprintf(buf+len, 0x1000, "</div>");
			else len += mysnprintf(buf+len, 0x1000, "%d</div>", val2048[tab[y][x]]);
		}
	}
	win->info[0] = len;
}
static void the2048_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y,j,k,ret;
	u8 src[10];
	u8 (*tab)[4] = (void*)buffer + num*16;

	int w = win->w;
	int h = win->h;
	u8* buf = (u8*)(win->buf);

	for(x=0;x<w*h*4;x++)buf[x]=0;
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			data2decstr(val2048[tab[y][x]], src);
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
	u8 (*tab)[4] = (void*)buffer + num*16;

	say("%d	", val2048[tab[0][0]]);
	say("%d	", val2048[tab[0][1]]);
	say("%d	", val2048[tab[0][2]]);
	say("%d\n",val2048[tab[0][3]]);

	say("%d	", val2048[tab[1][0]]);
	say("%d	", val2048[tab[1][1]]);
	say("%d	", val2048[tab[1][2]]);
	say("%d\n",val2048[tab[1][3]]);

	say("%d	", val2048[tab[2][0]]);
	say("%d	", val2048[tab[2][1]]);
	say("%d	", val2048[tab[2][2]]);
	say("%d\n",val2048[tab[2][3]]);

	say("%d	", val2048[tab[3][0]]);
	say("%d	", val2048[tab[3][1]]);
	say("%d	", val2048[tab[3][2]]);
	say("%d\n",val2048[tab[3][3]]);
}
static void the2048_read(struct arena* win, struct actor* act, struct style* sty)
{
	//cli
	if(win->fmt == hex32('c','l','i',0))the2048_read_cli();

	//text
	else if(win->fmt == hex32('t','u','i',0))the2048_read_tui(win, act, sty);

	//html
	else if(win->fmt == hex32('h','t','m','l'))the2048_read_html(win, act, sty);

	//vbo
	else if(win->fmt == hex32('v','b','o',0))the2048_read_vbo(win, act, sty);

	//pixel
	else the2048_read_pixel(win, act, sty);
}




static void the2048_write(struct event* ev)
{
	//kbd
	int j,k;
	u8* p;
	u8* q;
	//say("%x,%x\n",ev->why, ev->what);

	//
	if(ev->what == 0x64626b)
	{
		k = (ev->why)&0xff;
		if( (k>=0x25) && (k<=0x28) )
		{
			//
			p = (void*)buffer + 16*num;
			num = (num+1)%4;
			q = (void*)buffer + 16*num;
			for(j=0;j<16;j++)q[j] = p[j];

			//
			if(k == 0x25)left2048((void*)q);
			else if(k == 0x26)up2048((void*)q);
			else if(k == 0x27)right2048((void*)q);
			else if(k == 0x28)down2048((void*)q);

			//new number?
			new2048((void*)q);
		}
	}
	else if(ev->what == 0x72616863)
	{
		if(k == 0x8)
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
	act->irel = 0;
	act->orel = 0;

	act->start = (void*)the2048_start;
	act->stop = (void*)the2048_stop;
	act->list = (void*)the2048_list;
	act->choose = (void*)the2048_choose;
	act->read = (void*)the2048_read;
	act->write = (void*)the2048_write;
}
