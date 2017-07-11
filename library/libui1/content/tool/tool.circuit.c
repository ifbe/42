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
	//link all chip on this pin
	u64 pinnum;			//pin42
	u64 pinsts;			//high, low?
	struct wirenet* lastchip;
	char pad2[8-sizeof(char*)];
	struct wirenet* nextchip;
	char pad3[8-sizeof(char*)];

	//link all pin on this chip
	u64 type;			//R
	u64 value;			//1kohm
	struct wirenet* lastpin;
	char pad0[8-sizeof(char*)];
	struct wirenet* nextpin;
	char pad1[8-sizeof(char*)];
};
static struct wirenet* wn;




/*
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




static void circuit_read_pixel_battery(struct arena* win, int x, int y)
{
	line(win, x-8, y-4, x+8, y-4, 0xffffffff);
	line(win, x-4, y+4, x+4, y+4, 0xffffffff);
}
static void circuit_read_pixel_resistor(struct arena* win, int x, int y)
{
	rectframe(win, x-4, y-8, x+4, y+8, 0xffffffff);
}
static void circuit_read_pixel(struct arena* win, struct actor* act, struct relation* rel)
{
	int depth;
	int cx,cy,w,h;
	struct wirenet* this;

	//
	cx = (win->w) * (rel->cx) / 0x10000;
	cy = (win->h) * (rel->cy) / 0x10000;
	w = (win->w) * (rel->wantw) / 0x10000;
	h = (win->h) * (rel->wanth) / 0x10000;

	//
	this = wn;
	while(1)
	{
		if((this->type) == 'V')circuit_read_pixel_battery(win, cx+depth, cy);
		else if((this->type) == 'R')circuit_read_pixel_resistor(win, cx+depth, cy);

		//same chip next pin
		this = this->nextpin;
		if(this == 0)break;

		//same pin next chip
		this = this->nextchip;
		if(this == 0)break;

		depth += 32;
	}
}
static void circuit_read_html(struct arena* win, struct actor* act, struct relation* rel)
{
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
		  |----V0----|
	pin1--|----R1----|--pin2
		  |----R2----|
	*/

	//pin1 -- V0
	wn[0].pinnum = 1;
	wn[0].pinsts = 0;
	wn[0].lastchip = 0;
	wn[0].nextchip = &wn[1];
	wn[0].type = 'V';
	wn[0].value = 5;
	wn[0].lastpin = 0;
	wn[0].nextpin = &wn[3];

	//pin1 -- R1
	wn[1].pinnum = 1;
	wn[1].pinsts = 0;
	wn[1].lastchip = &wn[0];
	wn[1].nextchip = &wn[2];
	wn[1].type = 'R';
	wn[1].value = 4100;
	wn[1].lastpin = 0;
	wn[1].nextpin = &wn[4];

	//pin1 -- R2
	wn[2].pinnum = 1;
	wn[2].pinsts = 0;
	wn[2].lastchip = &wn[1];
	wn[2].nextchip = 0;
	wn[2].type = 'R';
	wn[2].value = 9000;
	wn[2].lastpin = 0;
	wn[2].nextpin = &wn[5];

	//pin2 -- V0
	wn[3].pinnum = 2;
	wn[3].pinsts = 0;
	wn[3].lastchip = 0;
	wn[3].nextchip = &wn[4];
	wn[3].type = 'V';
	wn[3].value = 5;
	wn[3].lastpin = &wn[0];
	wn[3].nextpin = 0;

	//pin2 -- R1
	wn[4].pinnum = 2;
	wn[4].pinsts = 0;
	wn[4].lastchip = &wn[3];
	wn[4].nextchip = &wn[5];
	wn[4].type = 'R';
	wn[4].value = 4100;
	wn[4].lastpin = &wn[1];
	wn[4].nextpin = 0;

	//pin2 -- R2
	wn[5].pinnum = 2;
	wn[5].pinsts = 0;
	wn[5].lastchip = &wn[4];
	wn[5].nextchip = 0;
	wn[5].type = 'R';
	wn[5].value = 9000;
	wn[5].lastpin = &wn[2];
	wn[5].nextpin = 0;
}
static void circuit_stop()
{
}
void circuit_create(void* base,void* addr)
{
	struct actor* p = addr;
	wn = base+0x300000;

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
