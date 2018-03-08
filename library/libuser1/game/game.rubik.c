#include "actor.h"
void rubikscube_generate(void*);




//l,r,f,b,u,d
static u8 buffer[6][3][3];
u32 rubikcolor[6] = {
	0x00ff00, 0x0000ff,		//l, r
	0xff0000, 0xfa8010,		//f, b
	0xffffff, 0xffff00		//u, d
};




static void rubikscube_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;

	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			//l
			carvesolid_rect(
				win, 0xff00,
				cx-ww*2/2, cy-(x-1)*hh*2/3, (y-1)*hh*2/3,
				0.0, -ww*2/7, 0.0,
				0.0, 0.0, hh*2/7
			);
			carveascii(
				win, ~0xff00,
				cx-ww*2/1.99, cy-(x-1)*hh*2/3, (y-1)*hh*2/3,
				0.0, -ww*2/14, 0.0,
				0.0, 0.0, hh*2/14,
				(y*3)+x+0x30
			);

			//r
			carvesolid_rect(
				win, 0xff,
				cx+ww*2/2, cy+(x-1)*hh*2/3, (y-1)*hh*2/3,
				0.0, ww*2/7, 0.0,
				0.0, 0.0, hh*2/7
			);
			carveascii(
				win, ~0xff,
				cx+ww*2/1.99, cy+(x-1)*hh*2/3, (y-1)*hh*2/3,
				0.0, ww*2/14, 0.0,
				0.0, 0.0, hh*2/14,
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
				cx+(x-1)*ww*2/3, cy-hh*2/2, (y-1)*hh*2/3,
				ww*2/7, 0.0, 0.0,
				0.0, 0.0, hh*2/7
			);
			carveascii(
				win, ~0xff0000,
				cx+(x-1)*ww*2/3, cy-hh*2/1.99, (y-1)*hh*2/3,
				ww*2/14, 0.0, 0.0,
				0.0, 0.0, hh*2/14,
				(y*3)+x+0x30
			);

			//b
			carvesolid_rect(
				win, 0xfa8010,
				cx-(x-1)*ww*2/3, cy+hh*2/2, (y-1)*hh*2/3,
				-ww*2/7, 0.0, 0.0,
				0.0, 0.0, hh*2/7
			);
			carveascii(
				win, ~0xfa8010,
				cx-(x-1)*ww*2/3, cy+hh*2/1.99, (y-1)*hh*2/3,
				-ww*2/14, 0.0, 0.0,
				0.0, 0.0, hh*2/14,
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
				cx+(x-1)*ww*2/3, cy+(y-1)*hh*2/3, dd*2/2,
				ww*2/7, 0.0, 0.0,
				0.0, hh*2/7, 0.0
			);
			carveascii(
				win, ~0xffffff,
				cx+(x-1)*ww*2/3, cy+(y-1)*hh*2/3, dd*2/1.99,
				ww*2/14, 0.0, 0.0,
				0.0, hh*2/14, 0.0,
				(y*3)+x+0x30
			);

			//d
			carvesolid_rect(
				win, 0xffff00,
				cx+(x-1)*ww*2/3, cy+(y-1)*hh*2/3, -dd*2/2,
				ww*2/7, 0.0, 0.0,
				0.0, -hh*2/7, 0.0
			);
			carveascii(
				win, ~0xffff00,
				cx+(x-1)*ww*2/3, cy+(y-1)*hh*2/3, -dd*2/1.99,
				ww*2/14, 0.0, 0.0,
				0.0, -hh*2/14, 0.0,
				(y*3)+x+0x30
			);
		}
	}
}
static void rubikscube_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 bg;
	int x,y;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx / 3;
	int hh = sty->fy / 3;
	int dd = sty->uz / 3;

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
static void rubikscube_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void rubikscube_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void rubikscube_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("rubik(%x,%x,%x)\n",win,act,sty);
}
static void rubikscube_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)rubikscube_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)rubikscube_read_tui(win, sty, act, pin);
	else if(fmt == _html_)rubikscube_read_html(win, sty, act, pin);
	else if(fmt == _vbo_)rubikscube_read_vbo(win, sty, act, pin);
	else rubikscube_read_pixel(win, sty, act, pin);
}
static void rubikscube_write(
	struct actor* act, struct pinid* pin,
	struct event* ev)
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
static void rubikscube_stop(struct actor* act, struct pinid* pin)
{
}
static void rubikscube_start(struct actor* act, struct pinid* pin)
{
	rubikscube_generate(buffer);
}
static void rubikscube_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)stopmemory(act->buf);
}
static void rubikscube_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = startmemory(54);
}




void rubikscube_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('r', 'u', 'b', 'i', 'k', 0, 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)rubikscube_create;
	p->ondelete = (void*)rubikscube_delete;
	p->onstart  = (void*)rubikscube_start;
	p->onstop   = (void*)rubikscube_stop;
	p->onlist   = (void*)rubikscube_list;
	p->onchoose = (void*)rubikscube_choose;
	p->onread   = (void*)rubikscube_read;
	p->onwrite  = (void*)rubikscube_write;
}
