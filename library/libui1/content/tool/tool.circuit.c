#include "actor.h"
void rectbody(void*,
	int x1, int y1,
	int x2, int y2,
	u32 color);
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
	u64 pininfo;
	u64 order;
	struct wirenet* lastchip;
	char pad2[8-sizeof(char*)];
	struct wirenet* nextchip;
	char pad3[8-sizeof(char*)];

	//link all foot on this chip
	u64 chipinfo;
	u64 footid;
	struct wirenet* lastfoot;
	char pad0[8-sizeof(char*)];
	struct wirenet* nextfoot;
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
static void circuit_read_pixel_element(struct arena* win, struct wirenet* this, int x, int y)
{
	int chiptype = (this->footid)>>16;
	if(chiptype == 'R')
	{
		circuit_read_pixel_resistor(win,x,y);
	}
	else if(chiptype == 'V')
	{
		circuit_read_pixel_battery(win,x,y);
	}
	//say("@%x\n",this);
}
static void circuit_read_pixel_recursive(
	struct arena* win, struct wirenet* base, int px, int py,
	int cx, int cy, int w, int h)
{
	struct wirenet* this;
	int dx,dy,ret,foottype;
	if(base == 0)return;
	if(px<-4)return;
	if(px>4)return;
	if(py<-4)return;
	if(py>4)return;
	//say("%x\n",base);

	//chip
	foottype = (base->footid)&0xff;
	if(foottype != 'i')return;
	circuit_read_pixel_element(win, base, cx+w*px/16, cy+h*py/16);

	//left
	if(px <= 0)
	{
		dx = -1;
		this = base;
		while(1)
		{
			this = this->lastchip;
			if(this == 0)break;

			foottype = (this->footid)&0xff;
			if(foottype != 'i')continue;

			circuit_read_pixel_element(
				win, this, cx+w*(px+dx)/16, cy+h*py/16
			);
			dx--;
		}
	}

	//right
	if(px >= 0)
	{
		dx = 1;
		this = base;
		while(1)
		{
			this = this->nextchip;
			if(this == 0)break;

			foottype = (this->footid)&0xff;
			if(foottype != 'i')continue;

			circuit_read_pixel_element(
				win, this, cx+w*(px+dx)/16, cy+h*py/16
			);
			dx++;
		}
	}

	//upper
	if(py <= 0)
	{
		dy = -1;
		this = base;
		while(1)
		{
			//same chip, last foot
			this = this->lastfoot;
			if(this == 0)break;

		}
	}

	//lower
	if(py >= 0)
	{
		dy = 1;
		this = base;
		while(1)
		{
			//same chip, next foot
			this = this->nextfoot;
			if(this == 0)break;

			//same pin, 'i' foot
			if(px >= 0)
			{
				while(1)
				{
					this = this->nextchip;
					if(this == 0)break;

					foottype = (this->footid)&0xff;
					if(foottype != 'i')continue;

					circuit_read_pixel_recursive(
						win, this, px, py+dy,
						cx, cy, w, h
					);
					break;
				}
			}

			dy++;
			break;
		}
	}
}
static void circuit_read_pixel(struct arena* win, struct actor* act, struct relation* rel)
{
	//
	int cx,cy,w,h;
	cx = (win->w) * (rel->cx) / 0x10000;
	cy = (win->h) * (rel->cy) / 0x10000;
	w = (win->w) * (rel->wantw) / 0x10000;
	h = (win->h) * (rel->wanth) / 0x10000;

	//
	rectbody(win,
		cx-w/2, cy-h/2,
		cx+w/2, cy+h/2,
		0
	);
	circuit_read_pixel_recursive(
		win, wn, 0, 0,
		cx, cy, w, h
	);
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




/*
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
*/
static void circuit_list()
{
}
static void circuit_change(
	u64 operation, u64 signature,
	u64 pininfo, u64 order,
	u64 chipinfo, u64 footid)
{
	int j,k;
	struct wirenet* this;
	struct wirenet* samepin;
	struct wirenet* samechip;
	if(signature != 42)return;
//say("here\n");

	//search this
	for(j=0;j<100;j++)
	{
		this = &wn[j];
		if(this->pininfo == 0)break;
	}
//say("1\n");
	//prepare this
	this->pininfo = pininfo;
	//this->order = order;
	this->lastchip = 0;
	this->nextchip = 0;

	this->chipinfo = chipinfo;
	this->footid = footid;
	this->lastfoot = 0;
	this->nextfoot = 0;
//say("2\n");
	//search pin
	samepin = 0;
	for(k=0;k<j;k++)
	{
		if(wn[k].pininfo == pininfo)
		{
			samepin = &wn[k];
			break;
		}
	}
//say("3\n");
	//insert pin
	if(samepin != 0)
	{
		while(samepin->nextchip != 0)samepin = samepin->nextchip;
		samepin->nextchip = this;
		this->lastchip = samepin;
	}
//say("4\n");
	//search chip
	samechip = 0;
	for(k=0;k<j;k++)
	{
		if(wn[k].chipinfo == chipinfo)
		{
			samechip = &wn[k];
			break;
		}
	}
//say("5\n");
	//insert chip
	if(samechip != 0)
	{
		while(samechip->nextfoot != 0)samechip = samechip->nextfoot;
		samechip->nextfoot = this;
		this->lastfoot = samechip;
	}
//say("6\n");
}
static void circuit_start()
{
	int j;
	u8* p = (u8*)wn;
	for(j=0;j<0x1000;j++)
	{
		p[j] = 0;
	}

/*
	|-------pin1----
	|	R1    V2    R3
	|	----pin2----
	|	V4    R5
	|	----pin3----
	|	R6
	|   ----pin4----
	|   R7    R8    R9
	|-------pin1----
*/
	//operation, signature, pininfo, order, chipinfo, footid
	circuit_change('+', 42, 1, 0, 1, ('R'<<16)+'i');
	circuit_change('+', 42, 1, 0, 2, ('V'<<16)+'i');
	circuit_change('+', 42, 1, 0, 3, ('R'<<16)+'i');
	
	circuit_change('+', 42, 2, 0, 1, ('R'<<16)+'o');
	circuit_change('+', 42, 2, 0, 2, ('V'<<16)+'o');
	circuit_change('+', 42, 2, 0, 3, ('R'<<16)+'o');

	circuit_change('+', 42, 2, 0, 4, ('V'<<16)+'i');
	circuit_change('+', 42, 2, 0, 5, ('R'<<16)+'i');
	
	circuit_change('+', 42, 3, 0, 4, ('V'<<16)+'o');
	circuit_change('+', 42, 3, 0, 5, ('R'<<16)+'o');

	circuit_change('+', 42, 3, 0, 6, ('R'<<16)+'i');

	circuit_change('+', 42, 4, 0, 6, ('R'<<16)+'o');

	circuit_change('+', 42, 4, 0, 7, ('R'<<16)+'i');
	circuit_change('+', 42, 4, 0, 8, ('R'<<16)+'i');
	circuit_change('+', 42, 4, 0, 9, ('R'<<16)+'i');

	circuit_change('+', 42, 1, 0, 7, ('R'<<16)+'o');
	circuit_change('+', 42, 1, 0, 8, ('R'<<16)+'o');
	circuit_change('+', 42, 1, 0, 9, ('R'<<16)+'o');
/*
	for(j=0;j<12;j++)
	{
		say("@%x:	%x,%x,%x,	%x,%x,%x,%x\n", &wn[j],
			wn[j].pininfo, wn[j].lastchip, wn[j].nextchip,
			wn[j].chipinfo, wn[j].footid, wn[j].lastfoot, wn[j].nextfoot
		);
	}
*/
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
