#include "actor.h"




void rectbody(void*,
	int x1, int y1,
	int x2, int y2,
	u32 color
);




static void chess_read_pixel(struct arena* win)
{
	u32 color;
	int x,y;
	int w = win->w;
	int h = win->h;
	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			if(((x+y+32)%2) == 0)color = 0;
			else color = 0xffffff;

			rectbody(win,
				x*w/8, y*h/8,
				(x+1)*w/8, (y+1)*h/8,
				color
			);
		}
	}
}
static void chess_read_html(struct arena* win)
{
}
static void chess_read_text(struct arena* win)
{
}
static void chess_read(struct arena* win)
{
	u64 fmt = win->fmt;

	//text 
	if(fmt == 0x74786574)
	{
		chess_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		chess_read_html(win);
	}

	//pixel
	else
	{
		chess_read_pixel(win);
	}
}
static void chess_write(struct event* ev)
{
}




static void chess_list()
{
}
static void chess_choose()
{
}
static void chess_start()
{
}
static void chess_stop()
{
}
void chess_create(void* base, void* addr)
{
	struct actor* act = addr;

	act->type = 0x656d6167;
	act->name = 0x7373656863;
	act->first = 0;
	act->last = 0;

	act->start = (void*)chess_start;
	act->stop = (void*)chess_stop;
	act->list = (void*)chess_list;
	act->choose = (void*)chess_choose;
	act->read = (void*)chess_read;
	act->write = (void*)chess_write;
}
void chess_delete()
{
}
