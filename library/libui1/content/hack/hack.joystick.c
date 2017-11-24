#include "actor.h"




static int aaaa = 0;




static void keyboard(struct arena* win)
{
	int x,y;
	int width = win->w;
	int height = win->h;

	for(x=0;x<32;x++)
	{
		drawline_rect(win, 0x040404*x,
			x, x, width-32+x, height-32+x
		);
	}
	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			drawline_rect(win, 0xffffff,
				32 + (width-32)*x/8, 32 + (height-32)*y/8,
				31 + (width-32)*(x+1)/8, 31 + (height-32)*(y+1)/8
			);

			drawascii(
				win, 'a', 4,
				32 + (width-32)*x/8, 32 + (height-32)*y/8,
				0, 0xffffffff
			);
		}
	}
}
static void joystick(struct arena* win)
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
}
static void touchpad(struct arena* win)
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
}
static void control_read_text(struct arena* win)
{
}
static void control_read_html(struct arena* win)
{
}
static void control_read_pixel(struct arena* win)
{
	if(aaaa == 0)joystick(win);
	else if(aaaa == 1)keyboard(win);
	else if(aaaa == 2)touchpad(win);

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
static void control_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == hex32('t','e','x','t'))control_read_text(win);
	else if(fmt == hex32('h','t','m','l'))control_read_html(win);
	else control_read_pixel(win);
}




static void control_write(u64* who, u64* what, u64* how)
{
	int x,y;
	if(*what == 0x2d70)
	{
		x = *(u16*)how;
		y = *(u16*)(how+2);

		if(y<64)
		{
			if(x<64)aaaa = 0;
			if(x>(512-64))aaaa = 1;
		}
		if(y>512-64)
		{
			if(x<64)aaaa = 2;
		}
	}
}




static void control_list()
{
}
static void control_change()
{
}




static void control_start()
{
}
static void control_stop()
{
}
void control_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('h', 'a', 'c', 'k');
	p->name = hex64('i', 'n', 'p', 'u', 't', 0, 0, 0);

	p->start = (void*)control_start;
	p->stop = (void*)control_stop;
	p->list = (void*)control_list;
	p->choose = (void*)control_change;
	p->read = (void*)control_read;
	p->write = (void*)control_write;
}
void control_delete()
{
}
