#include "actor.h"
void rectframe(void*,
	int x1, int y1,
	int x2, int y2,
	u32 color);
void line(void*,
	int x1,int y1,
	int x2,int y2,
	u32 color);




//元件
struct wirenet
{
	u64 type;			//R
	u64 value;			//1kohm
	struct wirenet* lastpin;
	char pad0[8-sizeof(char*)];
	struct wirenet* nextpin;
	char pad1[8-sizeof(char*)];

	u64 pin;			//pin42
	u64 status;			//high, low?
	struct wirenet* lastchip;
	char pad2[8-sizeof(char*)];
	struct wirenet* nextchip;
	char pad3[8-sizeof(char*)];
};
static struct wirenet* wirenet;




/*
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
*/




static void circuit_read_html(struct arena* win, struct actor* act, struct relation* rel)
{
}
static void circuit_read_pixel(struct arena* win, struct actor* act, struct relation* rel)
{
	int cx,cy,w,h;
	cx = (win->w) * (rel->cx) / 0x10000;
	cy = (win->h) * (rel->cy) / 0x10000;
	w = (win->w) * (rel->wantw) / 0x10000;
	h = (win->h) * (rel->wanth) / 0x10000;
	rectframe(win,
		cx-w/2, cy-h/2,
		cx+w/2, cy+h/2,
		0x888888
	);
}
static void circuit_read_text(struct arena* win, struct actor* act, struct relation* rel)
{
}
static void circuit_read(struct relation* rel)
{
	struct arena* win = rel->parent_this;
	struct actor* act = rel->child_this;
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		circuit_read_text(win, act, rel);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		circuit_read_html(win, act, rel);
	}

	//pixel
	else
	{
		circuit_read_pixel(win, act, rel);
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
	/*
		  |---R---|
		pin1     pin2
		  |---V---|
	*/

	//R -- pin1
	wirenet[0].type = 'R';
	wirenet[0].value = 4100;
	wirenet[0].lastpin = 0;
	wirenet[0].nextpin = &wirenet[1];

	wirenet[0].pin = 1;
	wirenet[0].status = 0;
	wirenet[0].lastchip = 0;
	wirenet[0].nextchip = &wirenet[2];

	//R -- pin2
	wirenet[1].type = 'R';
	wirenet[1].value = 4100;
	wirenet[1].lastpin = &wirenet[0];
	wirenet[1].nextpin = 0;

	wirenet[1].pin = 2;
	wirenet[1].status = 0;
	wirenet[1].lastchip = 0;
	wirenet[1].nextchip = &wirenet[3];

	//V -- pin1
	wirenet[2].type = 'V';
	wirenet[2].value = 5;
	wirenet[2].lastpin = 0;
	wirenet[2].nextpin = &wirenet[3];

	wirenet[2].pin = 1;
	wirenet[2].status = 0;
	wirenet[2].lastchip = &wirenet[0];
	wirenet[2].nextchip = 0;

	//V -- pin2
	wirenet[3].type = 'V';
	wirenet[3].value = 5;
	wirenet[3].lastpin = &wirenet[2];
	wirenet[3].nextpin = 0;

	wirenet[3].pin = 2;
	wirenet[3].status = 0;
	wirenet[3].lastchip = 0;
	wirenet[3].nextchip = &wirenet[1];
}
static void circuit_stop()
{
}
void circuit_create(void* base,void* addr)
{
	struct actor* p = addr;
	wirenet = base+0x300000;

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
