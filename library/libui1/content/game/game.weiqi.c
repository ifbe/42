#include "actor.h"




static int turn;
static int px,py;
//
static char* data;




static void weiqi_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void weiqi_read_text(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y,j,k,ret,color;
	int width = win->w;
	int height = win->h;
	u8* p = (u8*)(win->buf);

	//
	for(x=0;x<width*height*4;x++)p[x] = 0;
	for(y=0;y<19;y++)
	{
		for(x=0;x<19;x++)
		{
			//position
			ret = y*width + x*2;
			ret <<= 2;

			//color
			if( (px == x) && (py == y) )color = 7;
			else if(data[(y*19) + x] == 'b')color = 4;
			else if(data[(y*19) + x] == 'w')color = 1;
			else continue;

			//
			p[ret] = 0x20;
			p[ret + 3] = color;
			p[ret + 4] = 0x20;
			p[ret + 7] = color;
		}
	}
}
static void weiqi_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	u32 color;
	int x,y,half;
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int w = (win->w) * (sty->wantw) / 0x10000;
	int h = (win->h) * (sty->wanth) / 0x10000;

	if(w != h)
	{
		w = (w+h)/2;
		h = w;
		sty->wantw = w * 0x10000 / (win->w);
		sty->wanth = h * 0x10000 / (win->h);
	}
	half = w / 38;

	//rgb? bgr?
	if( ((win->fmt)&0xffffff) == 0x626772)color = 0x256f8d;
	else color = 0x8d6f25;
	drawsolid_rect(win, color,
		cx-w/2, cy-h/2, cx+w/2, cy+h/2);

	//heng
	for(y=-9;y<10;y++)
	{
		drawline(win, 0,
			cx - half*2*9,	cy + half*2*y,
			cx + half*2*9,	cy + half*2*y);
	}

	//shu
	for(x=-9;x<10;x++)
	{
		drawline(win, 0,
			cx + half*2*x,	cy - half*2*9,
			cx + half*2*x,	cy + half*2*9);
	}

	//dian
	for(y = cy - half*2*6; y <= cy + half*2*6; y += half*2*6)
	{
		for(x = cx - half*2*6; x <= cx + half*2*6; x += half*2*6)
		{
			drawsolid_circle(win, 0, x, y, half/4);
		}
	}

	//zi
	for(y=-9;y<=9;y++)
	{
		for(x=-9;x<=9;x++)
		{
			if(data[(y+9)*19 + x+9] == 'b')color = 0x444444;
			else if(data[(y+9)*19 + x+9] == 'w')color = 0xffffff;
			else continue;

			drawsolid_circle(win, color,
				cx + half*2*x, cy + half*2*y, half);
		}
	}
}
static void weiqi_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
	float cx = (float)(sty->cx) / 65536.0 - 0.5;
	float cy = (float)(sty->cy) / 65536.0 - 0.5;
	float w = (float)(sty->wantw) / 65536.0;
	float h = (float)(sty->wanth) / 65536.0;

	carvesolid_prism4(
		win, 0xffffff,
		cx, cy, 0.0,
		w/16, 0.0, 0.0,
		0.0, h/16, 0.0,
		0.0, 0.0, w/16
	);
}
static void weiqi_read_cli()
{
}
static void weiqi_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == hex32('c','l','i',0))weiqi_read_cli();
	else if(fmt == hex32('t','e','x','t'))weiqi_read_text(win, act, sty);
	else if(fmt == hex32('h','t','m','l'))weiqi_read_html(win, act, sty);
	else if(fmt == hex32('v','b','o',0))weiqi_read_vbo(win, act, sty);
	else weiqi_read_pixel(win, act, sty);
}




void weiqi_write(struct event* ev)
{
	char val;
	int x,y;
	u64 what = ev->what;
	u64 key = ev->why;

	if(what == 0x64626b)
	{
		if(key == 0x25)	//left
		{
			if(px<1)return;
			px--;
		}
		else if(key == 0x26)   //up
		{
			if(py<1)return;
			py--;
		}
		else if(key == 0x27)   //right
		{
			if(px>=18)return;
			px++;
		}
		else if(key == 0x28)   //down
		{
			if(py>=18)return;
			py++;
		}
	}
	else if(what == 0x72616863)
	{
		if(key == 0x20)
		{
			if((turn&1)==0)data[(py*19)+px] = 'b';
			else data[(py*19)+px] = 'w';
			turn++;
		}
	}
	else if(what == 0x2d70)
	{
		x=key & 0xffff;
		y=(key >> 16) & 0xffff;
		//say("%d,%d\n",x,y);

		x = (x*19)>>16;
		y = (y*19)>>16;
		//say("%d,%d\n",x,y);

		if(x<0)return;
		if(x>18)return;
		if(y<0)return;
		if(y>18)return;

		if((turn&0x1) == 0)data[(y*19) + x] = 'b';
		else data[(y*19) + x] = 'w';
		turn++;
	}
}




static void weiqi_list()
{
}
static void weiqi_choose()
{
}
static void weiqi_start()
{
	int x,y;

	turn=0;
	px=py=0;

	for(y=0;y<19;y++)
	{
		for(x=0;x<19;x++)
		{
			data[y*19 + x] = 0;
		}
	}
}
static void weiqi_stop()
{
}
void weiqi_create(void* base, void* addr)
{
	struct actor* p = addr;
	data = base+0x300000;

	p->type = hex32('g', 'a', 'm', 'e');
	p->name = hex64('w', 'e', 'i', 'q','i', 0, 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->start = (void*)weiqi_start;
	p->stop = (void*)weiqi_stop;
	p->list = (void*)weiqi_list;
	p->choose = (void*)weiqi_choose;
	p->read = (void*)weiqi_read;
	p->write = (void*)weiqi_write;
}
void weiqi_delete()
{
}
