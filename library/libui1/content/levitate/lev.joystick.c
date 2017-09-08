#include "actor.h"
//
void drawascii(
	void*, char ch, int size,
	int x1, int y1, u32 fg, u32 bg);
void drawrect_body(void*,
	int x1, int y1,
	int x2, int y2,
	u32 color);
void drawrect_frame(void*,
	int x1, int y1,
	int x2, int y2,
	u32 color);
void drawcircle_body(void*,
	int x, int y,
	int r, u32 color);




static int aaaa = 0;




static void keyboard(struct arena* win)
{
	int x,y;
	int width = win->w;
	int height = win->h;

	for(x=0;x<32;x++)
	{
		drawrect_frame(win,
			x, x,
			width-32+x, height-32+x,
			0x040404*x
		);
	}
	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			drawrect_frame(win,
				32 + (width-32)*x/8, 32 + (height-32)*y/8,
				31 + (width-32)*(x+1)/8, 31 + (height-32)*(y+1)/8,
				0xffffff
			);

			drawascii(
				win, 'a', 4,
				32 + (width-32)*x/8,
				32 + (height-32)*y/8,
				0,
				0xffffffff
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
		drawrect_frame(win,
			j, j,
			width-32+j, height-32+j,
			0x040404*j
		);
	}

	if(width > height)
	{
		j = height/16;

		drawrect_frame(win,
			width*13/32, height*3/4,
			width/2,height*11/16,
			0
		);
		drawrect_frame(win,
			width/2, height*3/4,
			width*19/32, height*11/16,
			0
		);

		drawcircle_body(win, width/8, height/2, j, 0xff);
		drawcircle_body(win, width/4, height/4, j, 0xff00);
		drawcircle_body(win, width/4, height*3/4, j, 0xffff);
		drawcircle_body(win, width*3/8, height/2, j, 0xff0000);

		drawcircle_body(win, width*5/8, height/2, j, 0xff00ff);
		drawcircle_body(win, width*3/4, height/4, j, 0xfedcba);
		drawcircle_body(win, width*3/4, height*3/4, j, 0xabcdef);
		drawcircle_body(win, width*7/8, height/2, j, 0xffff00);
	}
	else
	{
		j = width/16;

		drawrect_frame(win, width/4,height*13/32,width*5/16,height/2, 0);
		drawrect_frame(win, width/4,height/2,width*5/16,height*19/32, 0);

		drawcircle_body(win, width/2, height/8, j, 0xff);
		drawcircle_body(win, width/4, height/4, j, 0xffff);
		drawcircle_body(win, width*3/4, height/4, j, 0xff00);
		drawcircle_body(win, width/2, height*3/8, j, 0xff0000);

		drawcircle_body(win, width/2, height*5/8, j, 0xff00ff);
		drawcircle_body(win, width/4, height*3/4, j, 0xabcdef);
		drawcircle_body(win, width*3/4, height*3/4, j, 0xfedcba);
		drawcircle_body(win, width/2, height*7/8, j, 0xffff00);
	}
}
static void touchpad(struct arena* win)
{
	int j;
	int width = win->w;
	int height = win->h;

	for(j=0;j<32;j++)
	{
		drawrect_frame(win,
			j, j,
			width-32+j, height-32+j,
			0x040404*j
		);
	}
}
static void control_read_text(struct arena* win)
{
}
static void control_read_html(struct arena* win)
{
}
static void control_read(struct arena* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		control_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		control_read_html(win);
	}

	//pixel
	else
	{
		backgroundcolor(win, 0x444444);

		if(aaaa == 0)joystick(win);
		else if(aaaa == 1)keyboard(win);
		else if(aaaa == 2)touchpad(win);

		drawrect_body(win,
			0, 0,
			64, 64,
			0xffffff
		);
		drawrect_body(win,
			(win->w)-64, 0,
			(win->w)-1, 64,
			0xffffff
		);
		drawrect_body(win,
			0, (win->h)-64,
			64, (win->h)-1,
			0xffffff
		);
	}
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
