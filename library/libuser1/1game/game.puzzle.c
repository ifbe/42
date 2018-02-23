#include "actor.h"




static void puzzle_read()
{
}
static void puzzle_write()
{
}
static void puzzle_list()
{
}
static void puzzle_choose()
{
}
static void puzzle_stop()
{
}
static void puzzle_start()
{
}
static void puzzle_delete()
{
}
static void puzzle_create()
{
}




void puzzle_register(struct actor* p)
{
	p->type = hex32('g', 'a', 'm', 'e');
	p->name = hex64('p', 'u', 'z', 'z', 'l', 'e', 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)puzzle_create;
	p->ondelete = (void*)puzzle_delete;
	p->onstart  = (void*)puzzle_start;
	p->onstop   = (void*)puzzle_stop;
	p->onlist   = (void*)puzzle_list;
	p->onchoose = (void*)puzzle_choose;
	p->onread   = (void*)puzzle_read;
	p->onwrite  = (void*)puzzle_write;
}