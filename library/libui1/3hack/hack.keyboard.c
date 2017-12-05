#include "actor.h"




//max=32768
static int arealeft = 16384;
static int areatop = 32768;
static int arearight = 49152;
static int areabottom = 49152;
static char table[8][8] = {
	'a','b','c','d','e','f','g','h',
	'i','j','k','l','m','n','o','p',
	'q','r','s','t','u','v','w','x',
	'y','z',' ',' ',' ',' ',0x8,0xd,
	'0','1','2','3','4','5','6','7',
	'8','9',' ',' ',' ',' ',' ',' ',
	'+','-','*','/',' ',' ',' ',' ',
	'=',' ',' ',' ',' ',' ',' ',' '
};




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
static void keyboard_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == __cli__)keyboard_read_cli(win, act, sty);
	else if(fmt == __tui__)keyboard_read_tui(win, act, sty);
	else if(fmt == __html__)keyboard_read_html(win, act, sty);
	else if(fmt == __vbo__)keyboard_read_vbo(win, act, sty);
	else keyboard_read_pixel(win, act, sty);
}
int keyboard_write(struct event* ev)
{
	int x,y;
	//say("%x,%x\n",x,y);

	return 0;
}
void keyboard_list()
{
}
void keyboard_change()
{
}
void keyboard_start()
{
}
void keyboard_stop()
{
}
void keyboard_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('h', 'a', 'c', 'k');
	p->name = hex64('k', 'e', 'y', 'b', 'o', 'a', 'r', 'd');

	p->start = (void*)keyboard_start;
	p->stop = (void*)keyboard_stop;
	p->list = (void*)keyboard_list;
	p->choose = (void*)keyboard_change;
	p->read = (void*)keyboard_read;
	p->write = (void*)keyboard_write;
}
void keyboard_delete()
{
}
