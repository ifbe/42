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
	int width = win->w;
	int height = win->h;

	//[a,z]
	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			left = (arealeft + x*(arearight-arealeft)/8)*width/65536;
			top = (areatop + y*(areabottom-areatop)/8)*height/65536;
			right = (arealeft + (x+1)*(arearight-arealeft)/8)*width/65536;
			bottom = (areatop + (y+1)*(areabottom-areatop)/8)*height/65536;
			//say("====%d,%d,%d,%d\n",left,top,right,bottom);

			drawsolid_rect(win, 0xff00ff, left, top, right, bottom);
			drawline_rect(win, 0x000000, left, top, right, bottom);

			drawascii(
				win, table[y][x], 1,
				left, top, 0xffffffff, 0
			);
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

	if(ev->what == 0x2d70)
	{
		x = (ev->why)&0xffff;
		if(x < arealeft)return 0;
		if(x > arearight)return 0;

		y = ((ev->why)>>16)&0xffff;
		if(y < areatop)return 0;
		if(y > areabottom)return 0;

		x = (x-arealeft)*8/32768;
		y = (y-areatop)*8/16384;

		ev->what = 0x72616863;
		ev->why = table[y][x];
		return 1;
	}

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
