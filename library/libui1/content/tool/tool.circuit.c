#include "actor.h"




struct wirenet
{
	//link all chip on this pin
	u64 pininfo;
	u64 order;
	struct wirenet* samepinlastchip;
	char pad2[8-sizeof(char*)];
	struct wirenet* samepinnextchip;
	char pad3[8-sizeof(char*)];

	//link all foot on this chip
	u64 chipinfo;
	u64 footid;
	struct wirenet* samechiplastpin;
	char pad0[8-sizeof(char*)];
	struct wirenet* samechipnextpin;
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
	drawline(win, 0xffffff,
		x-8, y-4, x+8, y-4);
	drawline(win, 0xffffff,
		x-4, y+4, x+4, y+4);
}
static void circuit_read_pixel_resistor(struct arena* win, int x, int y)
{
	drawline_rect(win, 0xffffff,
		x-4, y-8, x+4, y+8);
}
static void circuit_read_pixel_element(struct arena* win, struct wirenet* this, int x, int y)
{
	int chiptype = (this->chipinfo)&0xffff;
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
	int dx,dy,ret;
	if(base == 0)return;
	if(px<-4)return;
	if(px>4)return;
	if(py<-4)return;
	if(py>4)return;
	//say("%x\n",base);

	//chip
	if((base->footid) != 'i')return;
	circuit_read_pixel_element(win, base, cx+w*px/16, cy+h*py/16);

	//left
	if(px <= 0)
	{
		dx = -1;
		this = base;
		while(1)
		{
			this = this->samepinlastchip;
			if(this == 0)break;
			if((this->footid) != 'i')continue;

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
			this = this->samepinnextchip;
			if(this == 0)break;
			if((this->footid) != 'i')continue;

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
			this = this->samechiplastpin;
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
			this = this->samechipnextpin;
			if(this == 0)break;

			//same pin, 'i' foot
			if(px >= 0)
			{
				while(1)
				{
					this = this->samepinnextchip;
					if(this == 0)break;
					if((this->footid) != 'i')continue;

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
static void circuit_read_pixel(struct arena* win, struct actor* act, struct style* rel)
{
	//
	int cx,cy,w,h;
	cx = (win->w) * (rel->cx) / 0x10000;
	cy = (win->h) * (rel->cy) / 0x10000;
	w = (win->w) * (rel->wantw) / 0x10000;
	h = (win->h) * (rel->wanth) / 0x10000;

	//
	drawsolid_rect(win, 0,
		cx-w/2, cy-h/2,
		cx+w/2, cy+h/2
	);
	circuit_read_pixel_recursive(
		win, wn, 0, 0,
		cx, cy, w, h
	);
}
static void circuit_read_html(struct arena* win, struct actor* act, struct style* rel)
{
}
static void circuit_read_text(struct arena* win, struct actor* act, struct style* rel)
{
}
static void circuit_read(struct arena* win, struct actor* act, struct style* rel)
{
	//text
	if(win->fmt == 0x74786574)
	{
		circuit_read_text(win, act, rel);
	}

	//html
	else if(win->fmt == 0x6c6d7468)
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
static void circuit_change(
	u64 operation, u64 signature,
	u64 pininfo, u64 order,
	u64 chipinfo, u64 footid)
{
	int j,k;
	struct wirenet* this;
	struct wirenet* temp;
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
	this->samepinlastchip = 0;
	this->samepinnextchip = 0;

	this->chipinfo = chipinfo;
	this->footid = footid;
	this->samechiplastpin = 0;
	this->samechipnextpin = 0;
//say("2\n");

	//search pin
	temp = 0;
	for(k=0;k<j;k++)
	{
		if(wn[k].pininfo == pininfo)
		{
			temp = &wn[k];
			break;
		}
	}
//say("3\n");

	//insert pin
	if(temp != 0)
	{
		while(temp->samepinnextchip != 0)temp = temp->samepinnextchip;
		temp->samepinnextchip = this;
		this->samepinlastchip = temp;
	}
//say("4\n");

	//search chip
	temp = 0;
	for(k=0;k<j;k++)
	{
		if(wn[k].chipinfo == chipinfo)
		{
			temp = &wn[k];
			break;
		}
	}
//say("5\n");

	//insert chip
	if(temp != 0)
	{
		while(temp->samechipnextpin != 0)temp = temp->samechipnextpin;
		temp->samechipnextpin = this;
		this->samechiplastpin = temp;
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
	circuit_change('+', 42, 1, 0, (1<<16)+'R', 'i');
	circuit_change('+', 42, 1, 0, (2<<16)+'V', 'i');
	circuit_change('+', 42, 1, 0, (3<<16)+'R', 'i');
	
	circuit_change('+', 42, 2, 0, (1<<16)+'R', 'o');
	circuit_change('+', 42, 2, 0, (2<<16)+'V', 'o');
	circuit_change('+', 42, 2, 0, (3<<16)+'R', 'o');

	circuit_change('+', 42, 2, 0, (4<<16)+'V', 'i');
	circuit_change('+', 42, 2, 0, (5<<16)+'R', 'i');
	
	circuit_change('+', 42, 3, 0, (4<<16)+'V', 'o');
	circuit_change('+', 42, 3, 0, (5<<16)+'R', 'o');

	circuit_change('+', 42, 3, 0, (6<<16)+'R', 'i');

	circuit_change('+', 42, 4, 0, (6<<16)+'R', 'o');

	circuit_change('+', 42, 4, 0, (7<<16)+'R', 'i');
	circuit_change('+', 42, 4, 0, (8<<16)+'R', 'i');
	circuit_change('+', 42, 4, 0, (9<<16)+'R', 'i');

	circuit_change('+', 42, 1, 0, (7<<16)+'R', 'o');
	circuit_change('+', 42, 1, 0, (8<<16)+'R', 'o');
	circuit_change('+', 42, 1, 0, (9<<16)+'R', 'o');
/*
	for(j=0;j<12;j++)
	{
		say("@%x:	%x,%x,%x,	%x,%x,%x,%x\n", &wn[j],
			wn[j].pininfo, wn[j].samepinlastchip, wn[j].samepinnextchip,
			wn[j].chipinfo, wn[j].footid, wn[j].samechiplastpin, wn[j].samechipnextpin
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

	p->type = hex32('t', 'o', 'o', 'l');
	p->name = hex64('c', 'i', 'r', 'c', 'u', 'i', 't', 0);

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
