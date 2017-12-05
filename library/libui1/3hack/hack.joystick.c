#include "actor.h"




static void joystick_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int j;
	int width = win->w;
	int height = win->h;

	for(j=0;j<32;j++)
	{
		drawline_rect(win, 0x040404*j,
			j, j, width-32+j, height-32+j
		);
	}

	if(width > height)
	{
		j = height/16;

		drawline_rect(win, 0,
			width*13/32, height*3/4,
			width/2, height*11/16
		);
		drawline_rect(win, 0,
			width/2, height*3/4,
			width*19/32, height*11/16
		);

		drawsolid_circle(win, 0x0000ff, width/8, height/2, j);
		drawsolid_circle(win, 0x00ff00, width/4, height/4, j);
		drawsolid_circle(win, 0x00ffff, width/4, height*3/4, j);
		drawsolid_circle(win, 0xff0000, width*3/8, height/2, j);

		drawsolid_circle(win, 0xff00ff, width*5/8, height/2, j);
		drawsolid_circle(win, 0xfedcba, width*3/4, height/4, j);
		drawsolid_circle(win, 0xabcdef, width*3/4, height*3/4, j);
		drawsolid_circle(win, 0xffff00, width*7/8, height/2, j);
	}
	else
	{
		j = width/16;

		drawline_rect(win, 0,
			width/4,height*13/32,width*5/16,height/2);
		drawline_rect(win, 0,
			width/4,height/2,width*5/16,height*19/32);

		drawsolid_circle(win, 0x0000ff, width/2, height/8, j);
		drawsolid_circle(win, 0x00ffff, width/4, height/4, j);
		drawsolid_circle(win, 0x00ff00, width*3/4, height/4, j);
		drawsolid_circle(win, 0xff0000, width/2, height*3/8, j);

		drawsolid_circle(win, 0xff00ff, width/2, height*5/8, j);
		drawsolid_circle(win, 0xabcdef, width/4, height*3/4, j);
		drawsolid_circle(win, 0xfedcba, width*3/4, height*3/4, j);
		drawsolid_circle(win, 0xffff00, width/2, height*7/8, j);
	}

	drawsolid_rect(win, 0xffffff,
		0, 0, 64, 64
	);
	drawsolid_rect(win, 0xffffff,
		(win->w)-64, 0, (win->w)-1, 64
	);
	drawsolid_rect(win, 0xffffff,
		0, (win->h)-64, 64, (win->h)-1
	);
}
static void joystick_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void joystick_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void joystick_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void joystick_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("joystick(%x,%x,%x)\n",win,act,sty);
}
static void joystick_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == __cli__)joystick_read_cli(win, act, sty);
	else if(fmt == __tui__)joystick_read_tui(win, act, sty);
	else if(fmt == __html__)joystick_read_html(win, act, sty);
	else if(fmt == __vbo__)joystick_read_vbo(win, act, sty);
	else joystick_read_pixel(win, act, sty);
}
static void joystick_write(u64* who, u64* what, u64* how)
{
}
static void joystick_list()
{
}
static void joystick_change()
{
}
static void joystick_start()
{
}
static void joystick_stop()
{
}
void joystick_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('h', 'a', 'c', 'k');
	p->name = hex64('j', 'o', 'y', 's', 't', 'i', 'c', 'k');

	p->start = (void*)joystick_start;
	p->stop = (void*)joystick_stop;
	p->list = (void*)joystick_list;
	p->choose = (void*)joystick_change;
	p->read = (void*)joystick_read;
	p->write = (void*)joystick_write;
}
void joystick_delete()
{
}
