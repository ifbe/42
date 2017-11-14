#include "actor.h"
void drawascii(
	void*, u8 ch, int size,
	int x, int y, u32 fg, u32 bg);




void font_read(struct arena* win)
{
	int x,y;
	for(y=2;y<8;y++)
	{
		for(x=0;x<16;x++)
		{
			drawascii(
				win, x+y*16, 1,
				x*16, y*16, 0xffffffff, 0);
		}
	}
}
void font_write(struct event* ev)
{
}
void font_list()
{
}
void font_change()
{
}
void font_start()
{
}
void font_stop()
{
}
void font_create(void* base, void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'e', 's', 't');
	p->name = hex32('f', 'o', 'n', 't');

	p->start = (void*)font_start;
	p->stop = (void*)font_stop;
	p->list = (void*)font_list;
	p->choose = (void*)font_change;
	p->read = (void*)font_read;
	p->write = (void*)font_write;
}
void font_delete()
{
}
