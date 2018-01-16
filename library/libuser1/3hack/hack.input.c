#include "actor.h"




//max=32768
static int arealeft = 16384;
static int areatop = 32768;
static int arearight = 49152;
static int areabottom = 49152;
static u8 table[8][8] = {
	'a','b','c','d','e','f','g','h',
	'i','j','k','l','m','n','o','p',
	'q','r','s','t','u','v','w','x',
	'y','z',' ',' ',' ',' ',0x8,0xd,
	'0','1','2','3','4','5','6','7',
	'8','9',' ',' ',' ',' ',' ',' ',
	'+','-','*','/',' ',' ',' ',' ',
	'=',' ',' ',' ',' ',' ',' ',' '
};




static void joystick_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int ww = (win->w) * (sty->wantw) / 0x20000;
	int hh = (win->h) * (sty->wanth) / 0x20000;
	int radius = hh/8;

	//outer
	drawline_rect(win, 0xffffff, cx-ww, cy-hh, cx+ww, cy+hh);

	//select, start
	drawline_rect(win, 0xffffff,
		cx-(ww/8), cy+(hh*11/16),
		cx, cy+(hh*13/16)
	);
	drawline_rect(win, 0xffffff,
		cx, cy+(hh*11/16),
		cx+(ww/8), cy+(hh*13/16)
	);

	//left, up, down, right
	drawsolid_rect(win, 0x0000ff,
		cx-(ww*5/8)-radius, cy-(radius/2),
		cx-(ww*5/8)+radius, cy+(radius/2)
	);
	drawsolid_rect(win, 0xff0000,
		cx-(ww*4/8)-(radius/2), cy-(hh/4)-radius,
		cx-(ww*4/8)+(radius/2), cy-(hh/4)+radius
	);
	drawsolid_rect(win, 0x00ffff,
		cx-(ww*4/8)-(radius/2), cy+(hh/4)-radius,
		cx-(ww*4/8)+(radius/2), cy+(hh/4)+radius
	);
	drawsolid_rect(win, 0xffff00,
		cx-(ww*3/8)-radius, cy-(radius/2),
		cx-(ww*3/8)+radius, cy+(radius/2)
	);

	//y, x, b, a
	drawsolid_circle(win, 0xff00ff,
		cx+(ww*3/8), cy, radius);
	drawsolid_circle(win, 0xfedcba,
		cx+(ww*4/8), cy-(hh/4), radius);
	drawsolid_circle(win, 0xabcdef,
		cx+(ww*4/8), cy+(hh/4), radius);
	drawsolid_circle(win, 0xffff00,
		cx+(ww*5/8), cy, radius);
}
static void joystick_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void joystick_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void joystick_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void joystick_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("joystick(%x,%x,%x)\n",win,act,sty);
}




void keyboard_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	int left,top,right,bottom;
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int ww = (win->w) * (sty->wantw) / 0x20000;
	int hh = (win->h) * (sty->wanth) / 0x20000;

	//[a,z]
	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			left = cx + (x-4)*ww/4;
			top = cy + (y-4)*hh/4;
			right = cx + (x-3)*ww/4;
			bottom = cy + (y-3)*hh/4;

			drawicon_1(win, 0, left, top, right, bottom, &table[y][x], 1);
		}
	}
}
static void keyboard_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void keyboard_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void keyboard_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void keyboard_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("keyboard(%x,%x,%x)\n",win,act,sty);
}




static void input_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == __cli__)keyboard_read_cli(win, act, sty);
	else if(fmt == __tui__)keyboard_read_tui(win, act, sty);
	else if(fmt == __html__)keyboard_read_html(win, act, sty);
	else if(fmt == __vbo__)keyboard_read_vbo(win, act, sty);
	else keyboard_read_pixel(win, act, sty);
}
static void input_write(struct event* ev)
{
	int x,y;
	//say("%x,%x\n",x,y);
}
static void input_list()
{
}
static void input_change()
{
}
static void input_stop()
{
}
static void input_start()
{
}
void input_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('h', 'a', 'c', 'k');
	p->name = hex64('i', 'n', 'p', 'u', 't', 0, 0, 0);

	p->start = (void*)input_start;
	p->stop = (void*)input_stop;
	p->list = (void*)input_list;
	p->choose = (void*)input_change;
	p->read = (void*)input_read;
	p->write = (void*)input_write;
}
void input_delete()
{
}