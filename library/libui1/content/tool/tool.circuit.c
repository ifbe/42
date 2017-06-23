#include "actor.h"
void backgroundcolor(void*, u32);
void rectframe(void*,
	int x1, int y1,
	int x2, int y2,
	u32 color);
void line(void*,
	int x1,int y1,
	int x2,int y2,
	u32 color);




//元件
struct cell{
	int type;	//battery, resistor, ...
	int value;	//ohm, volt, ...
	int side1;	//1,2,3, ...
	int side2;	//1,2,3, ...
};

//(batt, 5000mv), (0, 1)
static struct cell c1 = {'V', 5000, 0, 1};
//(resistor, 1000ohm), (1,0)
static struct cell c2 = {'R', 1000, 1, 0};




static void battery(struct arena* win, int x, int y, struct cell* c)
{
	line(win, x-16, y-8, x+16, y-8, 0xffffffff);
	line(win, x-8, y+8, x+8, y+8, 0xffffffff);
}
static void resistor(struct arena* win, int x, int y, struct cell* c)
{
	rectframe(win, x-8, y-16, x+8, y+16, 0xffffffff);
}
static void autowire(struct arena* win, int x1, int y1, int x2, int y2)
{
	int j;
	if(x1<x2)
	{
		j=y1-100;
		if(j > y2-100)j=y2-100;
		line(win, x1, y1, x1, j, 0xffffffff);
		line(win, x2, y2, x2, j, 0xffffffff);
		line(win, x1, j, x2, j, 0xffffffff);
	}
	else
	{
		j=y1+100;
		if(j < y2+100)j=y2+100;
		line(win, x1, y1, x1, j, 0xffffffff);
		line(win, x2, y2, x2, j, 0xffffffff);
		line(win, x1, j, x2, j, 0xffffffff);
	}
}




static void circuit_read_html(struct arena* win)
{
}
static void circuit_read_pixel(struct arena* win)
{
	int battx = (win->w)/3;
	int batty = (win->h)/2;
	int resx = (win->w)*2/3;
	int resy = (win->h)/2;
	backgroundcolor(win, 0);

	//5v battery
	battery(win, battx, batty, &c1);

	//1000ohm resistor
	resistor(win, resx, resy, &c2);

	//wire
	autowire(win, battx, batty-8, resx, resy-16);
	autowire(win, resx, resy+16, battx, batty+8);
}
static void circuit_read_text(struct arena* win)
{
}
static void circuit_read(struct arena* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		circuit_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		circuit_read_html(win);
	}

	//pixel
	else
	{
		circuit_read_pixel(win);
	}
}
static void circuit_write(struct event* ev)
{
}




static void circuit_list()
{
}
static void circuit_change()
{
}
static void circuit_start()
{
}
static void circuit_stop()
{
}
void circuit_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hexof('t','o','o','l',0,0,0,0);
	p->name = hexof('c','i','r','c','u','i','t',0);

	p->start = (void*)circuit_start;
	p->stop = (void*)circuit_stop;
	p->list = (void*)circuit_list;
	p->choose = (void*)circuit_change;
	p->read = (void*)circuit_read;
	p->write = (void*)circuit_write;
}
void circuit_delete()
{
}
