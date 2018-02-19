#include "actor.h"




static u8* databuf = 0;
static int printmethod = 0;
static u64 arenaoffset;
static u64 pointeroffset;
void hex_prep(void* name)
{
	int ret = openreadclose(name, databuf, 0, 0x1000);
	if(ret <= 0)return;

	databuf[ret] = 0;
}




static void hex_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y,nx,ny;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);

	ny = hh/8;
	nx = ww/8;
	if(nx > 0x40)nx = 0x40;
	else if(nx > 0x20)nx = 0x20;
	else if(nx > 0x10)nx = 0x10;
	else if(nx > 0x8)nx = 0x8;
	else if(nx > 0x4)nx = 0x4;
	if(printmethod == 0)		//hex
	{
		for(y=0;y<ny;y++)
		{
			for(x=0;x<nx;x++)
			{
				drawbyte(win, 0xffffff,
					cx+16*(x-nx/2), cy+16*(y-ny/2),
					databuf[arenaoffset + y*nx + x]
				);
			}
		}
	}

	else if(printmethod == 1)	//ascii
	{
		for(y=0;y<ny;y++)
		{
			for(x=0;x<nx;x++)
			{
				drawascii(win, 0xffffff,
					cx+16*(x-nx/2), cy+16*(y-ny/2),
					databuf[arenaoffset + y*nx + x]
				);
			}
		}
	}
}
static void hex_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void hex_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void hex_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void hex_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("hex(%x,%x,%x)\n",win,act,sty);
}
static void hex_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)hex_read_cli(win, act, sty);
	else if(fmt == _tui_)hex_read_tui(win, act, sty);
	else if(fmt == _vbo_)hex_read_vbo(win, act, sty);
	else if(fmt == _html_)hex_read_html(win, act, sty);
	else hex_read_pixel(win, act, sty);
}
static void hex_write(struct event* ev)
{
	int j,ret;
	u64 type = ev->what;
	u64 key = ev->why;
	//say("%x,%x\n",type,key);

	if(_drop_ == type)
	{
		ret = windowread(type, databuf);
		say("%s", databuf);

		for(j=0;j<ret;j++)
		{
			if(databuf[j] < 0x20)
			{
				databuf[j] = 0;
				break;
			}
		}
		hex_prep(databuf);
	}
}








static void hex_list()
{
}
static void hex_into()
{
}
static void hex_start()
{
}
static void hex_stop()
{
}
void hex_create(void* uibuf,void* addr)
{
	struct actor* p = addr;
	databuf = uibuf + 0x300000;

	p->type = hex32('h', 'a', 'c', 'k');
	p->name = hex32('h', 'e', 'x', 0);

	p->start = (void*)hex_start;
	p->stop = (void*)hex_stop;
	p->list = (void*)hex_list;
	p->choose = (void*)hex_into;
	p->read = (void*)hex_read;
	p->write = (void*)hex_write;
}
void hex_delete()
{
}
