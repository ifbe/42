#include "actor.h"
void rubikscube_generate(void*);




//l,r,f,b,u,d
static u8 buffer[6][3][3];
u32 rubikcolor[6] = {
	0x00ff00, 0x0000ff,		//l, r
	0xff0000, 0xfa8010,		//f, b
	0xffffff, 0xffff00		//u, d
};




static void rubikscube_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;

	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			//l
			carvesolid_rect(
				win, 0xff00,
				cx-ww/2, cy-(x-1)*hh/3, (y-1)*hh/3,
				0.0, -ww/7, 0.0,
				0.0, 0.0, hh/7
			);
			carveascii(
				win, ~0xff00,
				cx-ww/1.99, cy-(x-1)*hh/3, (y-1)*hh/3,
				0.0, -ww/14, 0.0,
				0.0, 0.0, hh/14,
				(y*3)+x+0x30
			);

			//r
			carvesolid_rect(
				win, 0xff,
				cx+ww/2, cy+(x-1)*hh/3, (y-1)*hh/3,
				0.0, ww/7, 0.0,
				0.0, 0.0, hh/7
			);
			carveascii(
				win, ~0xff,
				cx+ww/1.99, cy+(x-1)*hh/3, (y-1)*hh/3,
				0.0, ww/14, 0.0,
				0.0, 0.0, hh/14,
				(y*3)+x+0x30
			);
		}
	}

	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			//f
			carvesolid_rect(
				win, 0xff0000,
				cx+(x-1)*ww/3, cy-hh/2, (y-1)*hh/3,
				ww/7, 0.0, 0.0,
				0.0, 0.0, hh/7
			);
			carveascii(
				win, ~0xff0000,
				cx+(x-1)*ww/3, cy-hh/1.99, (y-1)*hh/3,
				ww/14, 0.0, 0.0,
				0.0, 0.0, hh/14,
				(y*3)+x+0x30
			);

			//b
			carvesolid_rect(
				win, 0xfa8010,
				cx-(x-1)*ww/3, cy+hh/2, (y-1)*hh/3,
				-ww/7, 0.0, 0.0,
				0.0, 0.0, hh/7
			);
			carveascii(
				win, ~0xfa8010,
				cx-(x-1)*ww/3, cy+hh/1.99, (y-1)*hh/3,
				-ww/14, 0.0, 0.0,
				0.0, 0.0, hh/14,
				(y*3)+x+0x30
			);
		}
	}

	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			//u
			carvesolid_rect(
				win, 0xffffff,
				cx+(x-1)*ww/3, cy+(y-1)*hh/3, dd/2,
				ww/7, 0.0, 0.0,
				0.0, hh/7, 0.0
			);
			carveascii(
				win, ~0xffffff,
				cx+(x-1)*ww/3, cy+(y-1)*hh/3, dd/1.99,
				ww/14, 0.0, 0.0,
				0.0, hh/14, 0.0,
				(y*3)+x+0x30
			);

			//d
			carvesolid_rect(
				win, 0xffff00,
				cx+(x-1)*ww/3, cy+(y-1)*hh/3, -dd/2,
				ww/7, 0.0, 0.0,
				0.0, -hh/7, 0.0
			);
			carveascii(
				win, ~0xffff00,
				cx+(x-1)*ww/3, cy+(y-1)*hh/3, -dd/1.99,
				ww/14, 0.0, 0.0,
				0.0, -hh/14, 0.0,
				(y*3)+x+0x30
			);
		}
	}
}
static void rubikscube_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	u32 bg;
	int x,y;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx / 3;
	int hh = sty->i_fy / 3;
	int dd = sty->i_uz / 3;

	bg = rubikcolor[2];
	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			drawsolid_rect(win, bg,
				cx+1 + ww*(2*x-3), cy+1 + hh*(2*y-3),
				cx-1 + ww*(2*x-1), cy-1 + hh*(2*y-1)
			);
			drawdecimal(win, 0xffffff,
				cx-4 + ww*2*(x-1), cy-8 + hh*2*(y-1),
				buffer[2][y][x] & 0xf
			);
		}
	}
}
static void rubikscube_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void rubikscube_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void rubikscube_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("rubik(%x,%x,%x)\n",win,act,sty);
}
static void rubikscube_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)rubikscube_read_cli(win, act, sty);
	else if(fmt == _tui_)rubikscube_read_tui(win, act, sty);
	else if(fmt == _html_)rubikscube_read_html(win, act, sty);
	else if(fmt == _vbo_)rubikscube_read_vbo(win, act, sty);
	else rubikscube_read_pixel(win, act, sty);
}




static void rubikscube_write(struct event* ev)
{
	if(ev->what == _kbd_)
	{
	}
	else if(ev->what == _char_)
	{
	}
}








static void rubikscube_list()
{
}
static void rubikscube_choose()
{
}
static void rubikscube_stop()
{
}
static void rubikscube_start()
{
	rubikscube_generate(buffer);
}
void rubikscube_delete()
{
}
void rubikscube_create(void* base, struct actor* act)
{
	act->type = hex32('g', 'a', 'm', 'e');
	act->name = hex64('r', 'u', 'b', 'i', 'k', 0, 0, 0);
	act->irel = 0;
	act->orel = 0;

	act->start = (void*)rubikscube_start;
	act->stop = (void*)rubikscube_stop;
	act->list = (void*)rubikscube_list;
	act->choose = (void*)rubikscube_choose;
	act->read = (void*)rubikscube_read;
	act->write = (void*)rubikscube_write;
}
