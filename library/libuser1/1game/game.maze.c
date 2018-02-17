#include "actor.h"
void maze_generate(void*, int);




#define mazesize 32
#define halfsize (mazesize/2)
static u8 buffer[mazesize][mazesize];




static void maze_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y,z,w;
	float fx,fy,fz;
	float cx = (float)(sty->cx) / 65536.0 - 0.5;
	float cy = (float)(sty->cy) / 65536.0 - 0.5;
	float ww = (float)(sty->wantw) / 65536.0 / mazesize;
	float hh = (float)(sty->wanth) / 65536.0 / mazesize;

	for(y=0;y<mazesize;y++)
	{
		for(x=0;x<mazesize;x++)
		{
			w = buffer[y][x];
			if((w&1) == 1)	//left
			{
				fx = cx+(x+0.0-halfsize)*ww;
				fy = cy+(y+0.5-halfsize)*hh;
				fz = ww/4;
				carvesolid_rect(
					win, 0x808080,
					fx, fy, fz,
					0.0, hh/2, 0.0,
					0.0, 0.0, fz
				);
			}
			if((w&2) == 2)	//right
			{
				fx = cx+(x+1.0-halfsize)*ww;
				fy = cy+(y+0.5-halfsize)*hh;
				fz = ww/4;
				carvesolid_rect(
					win, 0x909090,
					fx, fy, fz,
					0.0, -hh/2, 0.0,
					0.0, 0.0, fz
				);
			}
			if((w&8) == 8)	//up	//careful,different
			{
				fx = cx+(x+0.5-halfsize)*ww;
				fy = cy+(y+1.0-halfsize)*hh;
				fz = ww/4;
				carvesolid_rect(
					win, 0x606060,
					fx, fy, fz,
					ww/2, 0.0, 0.0,
					0.0, 0.0, fz
				);
			}
			if((w&4) == 4)	//down	//careful,different
			{
				fx = cx+(x+0.5-halfsize)*ww;
				fy = cy+(y+0.0-halfsize)*hh;
				fz = ww/4;
				carvesolid_rect(
					win, 0x707070,
					fx, fy, fz,
					-ww/2, 0.0, 0.0,
					0.0, 0.0, fz
				);
			}
		}
	}
}
static void maze_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y,w;
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int ww = (win->w) * (sty->wantw) / 0x10000 / mazesize;
	int hh = (win->h) * (sty->wanth) / 0x10000 / mazesize;

	for(y=0;y<mazesize;y++)
	{
		for(x=0;x<mazesize;x++)
		{
			w = buffer[y][x];
			if((w&1) == 1)	//left
			{
				drawline(win, 0x808080,
				cx+(x+0-halfsize)*ww, cy+(y+0-halfsize)*hh,
				cx+(x+0-halfsize)*ww, cy+(y+1-halfsize)*hh
				);
			}
			if((w&2) == 2)	//right
			{
				drawline(win, 0x808080,
				cx-1+(x+1-halfsize)*ww, cy+(y+0-halfsize)*hh,
				cx-1+(x+1-halfsize)*ww, cy+(y+1-halfsize)*hh
				);
			}
			if((w&4) == 4)	//up
			{
				drawline(win, 0x808080,
				cx+(x+0-halfsize)*ww, cy+(y-halfsize)*hh,
				cx+(x+1-halfsize)*ww, cy+(y-halfsize)*hh
				);
			}
			if((w&8) == 8)	//down
			{
				drawline(win, 0x808080,
				cx+(x+0-halfsize)*ww, cy-1+(y+1-halfsize)*hh,
				cx+(x+1-halfsize)*ww, cy-1+(y+1-halfsize)*hh
				);
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

	for(y=0;y<mazesize;y++)
	{
		for(x=0;x<mazesize;x++)
		{
			p = buf + (y*w*4) + (x*8);
			if(buffer[y][x] != 0)mysnprintf(p, 4, "⬛");
			else mysnprintf(p, 4, "  ");
		}
	}
}
static void maze_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	for(y=0;y<mazesize;y++)
	{
		for(x=0;x<mazesize;x++)
		{
			if(buffer[y][x] != 0)say("⬛");	//██
			else say("  ");
		}
		say("\n");
	}
	say("\n\n\n\n");
}
static void maze_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)maze_read_cli(win, act, sty);
	else if(fmt == _tui_)maze_read_tui(win, act, sty);
	else if(fmt == _html_)maze_read_html(win, act, sty);
	else if(fmt == _vbo_)maze_read_vbo(win, act, sty);
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
	maze_generate(buffer, mazesize);
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
