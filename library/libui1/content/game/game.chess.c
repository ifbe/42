#include "actor.h"




void rectbody(void*,
	int x1, int y1,
	int x2, int y2,
	u32 color
);




static void chess_read_pixel(struct arena* win, struct actor* act, struct relation* rel)
{
	u32 color;
	int x,y;
	int cx = (win->w) * (rel->cx) / 0x10000;
	int cy = (win->h) * (rel->cy) / 0x10000;
	int w = (win->w) * (rel->wantw) / 0x10000 / 8;
	int h = (win->h) * (rel->wanth) / 0x10000 / 8;
	if(w >= h)w=h;
	else h=w;

	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			if(((x+y+32)%2) == 0)color = 0;
			else color = 0xffffff;

			rectbody(win,
				cx+(x-4)*w, cy+(y-4)*h,
				cx+(x-3)*w, cy+(y-3)*h,
				color
			);
		}
	}
}
static void chess_read_html(struct arena* win, struct actor* act, struct relation* rel)
{
}
static void chess_read_text(struct arena* win, struct actor* act, struct relation* rel)
{
}
static void chess_read(struct relation* rel)
{
	struct arena* win = rel->parent_this;
	struct actor* act = rel->child_this;
	u64 fmt = win->fmt;

	//text 
	if(fmt == 0x74786574)
	{
		chess_read_text(win, act, rel);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		chess_read_html(win, act, rel);
	}

	//pixel
	else
	{
		chess_read_pixel(win, act, rel);
	}
}
static void chess_write(struct event* ev)
{
	say("@chess:%x,%x\n", ev->why, ev->what);
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

	act->type = hexof('g','a','m','e',0,0,0,0);
	act->name = hexof('c','h','e','s','s',0,0,0);
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
