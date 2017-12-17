#include "actor.h"
void maze_generate(void*, int);




#define mazesize 16
#define mazelens mazesize*2+1
static u8 buffer[mazelens][mazelens];




static void maze_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	float fx,fy,fz;
	float cx = (float)(sty->cx) / 65536.0 - 0.5;
	float cy = (float)(sty->cy) / 65536.0 - 0.5;
	float ww = (float)(sty->wantw) / 65536.0 / mazesize;
	float hh = (float)(sty->wanth) / 65536.0 / mazesize;

	for(y=0;y<mazelens;y++)
	{
		for(x=0;x<mazelens;x++)
		{
			if(((x&1) != 0)&&((y&1) == 0))
			{
				if(buffer[y][x] != 0)
				{
					fx = (float)x*0.5;
					fx = cx+(fx-mazesize/2)*ww;
					fy = cy+(y/2-mazesize/2)*hh;
					carvesolid_rect(
						win, 0xffffff,
						fx, fy, ww/2,
						ww/2, 0.0, 0.0,
						0.0, 0.0, ww/2
					);
				}
			}
			if(((x&1) == 0)&&((y&1) != 0))
			{
				if(buffer[y][x] != 0)
				{
					fx = cx+(x/2-mazesize/2)*ww;
					fy = (float)y*0.5;
					fy = cy+(fy-mazesize/2)*hh;
					carvesolid_rect(
						win, 0xffffff,
						fx, fy, ww/2,
						0.0, hh/2, 0.0,
						0.0, 0.0, ww/2
					);
				}
			}
		}
	}
}
static void maze_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int ww = (win->w) * (sty->wantw) / 0x10000 / mazesize;
	int hh = (win->h) * (sty->wanth) / 0x10000 / mazesize;

	for(y=0;y<mazelens;y++)
	{
		for(x=0;x<mazelens;x++)
		{
			if(((x&1) != 0)&&((y&1) == 0))
			{
				if(buffer[y][x] != 0)
				{
					drawline(win, 0xff,
						cx+(x/2-mazesize/2)*ww,
						cy+(y/2-mazesize/2)*hh,
						cx+(x/2-mazesize/2+1)*ww,
						cy+(y/2-mazesize/2)*hh
					);
				}
			}
			if(((x&1) == 0)&&((y&1) != 0))
			{
				if(buffer[y][x] != 0)
				{
					drawline(win, 0xff,
						cx+(x/2-mazesize/2)*ww,
						cy+(y/2-mazesize/2)*hh,
						cx+(x/2-mazesize/2)*ww,
						cy+(y/2-mazesize/2+1)*hh
					);
				}
			}
		}
	}
}
static void maze_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void maze_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	int w = win->w;
	int h = win->h;
	u8* buf = win->buf;
	u8* p;

	for(y=0;y<mazelens;y++)
	{
		for(x=0;x<mazelens;x++)
		{
			p = buf + (y*w*4) + (x*8);
			if(((x&1) != 0)&&((y&1) == 0))
			{
				if(buffer[y][x] != 0)
				{
					mysnprintf(p, 4, "一");
					continue;
				}
			}
			if(((x&1) == 0)&&((y&1) != 0))
			{
				if(buffer[y][x] != 0)
				{
					mysnprintf(p, 4, "丨");
					continue;
				}
			}
			if(((x&1) == 0)&&((y&1) == 0))
			{
				mysnprintf(p, 4, "十");
				continue;
			}
			mysnprintf(p, 4, "  ");
		}
	}
}
static void maze_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	for(y=0;y<mazelens;y++)
	{
		for(x=0;x<mazelens;x++)
		{
			if(((x&1) != 0)&&((y&1) == 0))
			{
				if(buffer[y][x] != 0)
				{
					//say("-");
					say("一");
					continue;
				}
			}
			if(((x&1) == 0)&&((y&1) != 0))
			{
				if(buffer[y][x] != 0)
				{
					//say("|");
					say("丨");
					continue;
				}
			}
			if(((x&1) == 0)&&((y&1) == 0))
			{
				say("十");
				continue;
			}
			say("  ");
		}
		say("\n");
	}
	say("\n\n\n\n");
}
static void maze_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == __cli__)maze_read_cli(win, act, sty);
	else if(fmt == __tui__)maze_read_tui(win, act, sty);
	else if(fmt == __html__)maze_read_html(win, act, sty);
	else if(fmt == __vbo__)maze_read_vbo(win, act, sty);
	else maze_read_pixel(win, act, sty);
}




static void maze_write(struct event* ev)
{
}








static void maze_list()
{
}
static void maze_choose()
{
}
static void maze_stop()
{
}
static void maze_start()
{
	maze_generate(buffer, mazelens);
}
void maze_delete()
{
}
void maze_create(void* base, struct actor* act)
{
	act->type = hex32('g', 'a', 'm', 'e');
	act->name = hex32('m', 'a', 'z', 'e');
	act->irel = 0;
	act->orel = 0;

	act->start = (void*)maze_start;
	act->stop = (void*)maze_stop;
	act->list = (void*)maze_list;
	act->choose = (void*)maze_choose;
	act->read = (void*)maze_read;
	act->write = (void*)maze_write;
}
