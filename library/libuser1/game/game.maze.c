#include "actor.h"
void maze_generate(void* buf, int w, int h);
void maze_solve(void* buf, int w, int h);




#define width 32
#define height 32
static u8 buffer[height][width];




static void maze_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y,z,w;
	float fx,fy,fz;
	float cx = sty->cx;
	float cy = sty->cy;
	float cz = sty->cz;
	float ww = sty->rx;
	float hh = sty->fy;
	float dd = sty->uz;

	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			w = buffer[y][x];
			if((w&1) == 1)	//left
			{
				fx = (cx-ww) + ((2*x+0)*ww/width);
				fy = (cy-hh) + ((2*y+1)*hh/height);
				fz = ww / width;
				carvesolid_rect(
					win, 0x808080,
					fx, fy, fz,
					0.0, hh/height, 0.0,
					0.0, 0.0, fz
				);
			}
			if((w&2) == 2)	//right
			{
				fx = (cx-ww) + ((2*x+2)*ww/width);
				fy = (cy-hh) + ((2*y+1)*hh/height);
				fz = ww / width;
				carvesolid_rect(
					win, 0x909090,
					fx, fy, fz,
					0.0, -hh/height, 0.0,
					0.0, 0.0, fz
				);
			}
			if((w&4) == 4)	//down	//careful,different
			{
				fx = (cx-ww) + ((2*x+1)*ww/width);
				fy = (cy-hh) + ((2*y+0)*hh/height);
				fz = ww / width;
				carvesolid_rect(
					win, 0x707070,
					fx, fy, fz,
					-ww/width, 0.0, 0.0,
					0.0, 0.0, fz
				);
			}
			if((w&8) == 8)	//up	//careful,different
			{
				fx = (cx-ww) + ((2*x+1)*ww/width);
				fy = (cy-hh) + ((2*y+2)*hh/height);
				fz = ww / width;
				carvesolid_rect(
					win, 0x606060,
					fx, fy, fz,
					ww/width, 0.0, 0.0,
					0.0, 0.0, fz
				);
			}
		}
	}
}
static void maze_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y,w;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;

	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			w = buffer[y][x];
			if((w&1) == 1)	//left
			{
				drawline(
					win, 0xff0000,
					(cx-ww) + (2*x+0)*ww/width,
					(cy-hh) + (2*y+0)*hh/height,
					(cx-ww) + (2*x+0)*ww/width,
					(cy-hh) + (2*y+2)*hh/height
				);
			}
			else if((w&0x80) == 0x80)
			{
				drawline(
					win, 0x00ff00,
					(cx-ww) + (2*x+0)*ww/width,
					(cy-hh) + (2*y+1)*hh/height,
					(cx-ww) + (2*x+1)*ww/width,
					(cy-hh) + (2*y+1)*hh/height
				);
			}

			if((w&2) == 2)	//right
			{
				drawline(
					win, 0x00ffff,
					(cx-ww-1) + (2*x+2)*ww/width,
					(cy-hh)   + (2*y+0)*hh/height,
					(cx-ww-1) + (2*x+2)*ww/width,
					(cy-hh)   + (2*y+2)*hh/height
				);
			}
			else if((w&0x80) == 0x80)
			{
				drawline(
					win, 0x00ff00,
					(cx-ww) + (2*x+1)*ww/width,
					(cy-hh) + (2*y+1)*hh/height,
					(cx-ww) + (2*x+2)*ww/width,
					(cy-hh) + (2*y+1)*hh/height
				);
			}

			if((w&4) == 4)	//up
			{
				drawline(
					win, 0x0000ff,
					(cx-ww) + (2*x+0)*ww/width,
					(cy-hh) + (2*y+0)*hh/height,
					(cx-ww) + (2*x+2)*ww/width,
					(cy-hh) + (2*y+0)*hh/height
				);
			}
			else if((w&0x80) == 0x80)
			{
				drawline(
					win, 0x00ff00,
					(cx-ww) + (2*x+1)*ww/width,
					(cy-hh) + (2*y+0)*hh/height,
					(cx-ww) + (2*x+1)*ww/width,
					(cy-hh) + (2*y+1)*hh/height
				);
			}

			if((w&8) == 8)	//down
			{
				drawline(
					win, 0xffff00,
					(cx-ww)   + (2*x+0)*ww/width,
					(cy-hh-1) + (2*y+2)*hh/height,
					(cx-ww)   + (2*x+2)*ww/width,
					(cy-hh-1) + (2*y+2)*hh/height
				);
			}
			else if((w&0x80) == 0x80)
			{
				drawline(
					win, 0x00ff00,
					(cx-ww) + (2*x+1)*ww/width,
					(cy-hh) + (2*y+1)*hh/height,
					(cx-ww) + (2*x+1)*ww/width,
					(cy-hh) + (2*y+2)*hh/height
				);
				/*
				drawsolid_rect(
					win, 0x00ff00,
					(cx-ww) + (4*x+1)*ww/width/2,
					(cy-hh) + (4*y+1)*hh/height/2,
					(cx-ww) + (4*x+3)*ww/width/2,
					(cy-hh) + (4*y+3)*hh/height/2
				);
				*/
			}
		}
	}
}
static void maze_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void maze_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	int stride = win->stride;
	u8* buf = win->buf;
	u8* p;

	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			p = buf + (y*stride*4) + (x*8);
			if(buffer[y][x] != 0)mysnprintf(p, 4, "⬛");
			else mysnprintf(p, 4, "  ");
		}
	}
}
static void maze_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			if(buffer[y][x] != 0)say("⬛");	//██
			else say("  ");
		}
		say("\n");
	}
	say("\n\n\n\n");
}
static void maze_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)maze_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)maze_read_tui(win, sty, act, pin);
	else if(fmt == _html_)maze_read_html(win, sty, act, pin);
	else if(fmt == _vbo_)maze_read_vbo(win, sty, act, pin);
	else maze_read_pixel(win, sty, act, pin);
}
static void maze_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void maze_list()
{
}
static void maze_choose()
{
}
static void maze_stop(struct actor* act, struct pinid* pin)
{
}
static void maze_start(struct actor* act, struct pinid* pin)
{
	maze_generate(buffer, width, height);
	maze_solve(buffer, width, height);
}
static void maze_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)stopmemory(act->buf);
}
static void maze_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = startmemory(width*height);
}




void maze_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex32('m', 'a', 'z', 'e');
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)maze_create;
	p->ondelete = (void*)maze_delete;
	p->onstart  = (void*)maze_start;
	p->onstop   = (void*)maze_stop;
	p->onlist   = (void*)maze_list;
	p->onchoose = (void*)maze_choose;
	p->onread   = (void*)maze_read;
	p->onwrite  = (void*)maze_write;
}
