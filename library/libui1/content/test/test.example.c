/*
1:	add name in library/libui1/makefile
2:	add code in character.c character_create() and character_delete()
3:	replace "example" to "whatname"----->		:9,$s/example/whatname/g
4:	do your logic in this code
*/
#include "actor.h"




static void example_read_html(struct arena* win)
{
}
static void example_read_pixel(struct arena* win)
{
}
static void example_read_text(struct arena* win)
{
}
static void example_read(struct arena* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		example_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		example_read_html(win);
	}

	//pixel
	else
	{
		example_read_pixel(win);
	}
}
static void example_write(struct event* ev)
{
}




static void example_list()
{
}
static void example_change()
{
}
static void example_start()
{
}
static void example_stop()
{
}
void example_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'e', 's', 't');
	p->name = hex64('e', 'x', 'a', 'm', 'p', 'l', 'e', 0);

	p->start = (void*)example_start;
	p->stop = (void*)example_stop;
	p->list = (void*)example_list;
	p->choose = (void*)example_change;
	p->read = (void*)example_read;
	p->write = (void*)example_write;
}
void example_delete()
{
}
