#include "actor.h"




static void minesweep_read()
{
}
static void minesweep_write()
{
}
static void minesweep_list()
{
}
static void minesweep_choose()
{
}
static void minesweep_stop()
{
}
static void minesweep_start()
{
}
static void minesweep_delete()
{
}
static void minesweep_create()
{
}




void minesweep_register(struct actor* p)
{
	p->type = hex32('g', 'a', 'm', 'e');
	p->name = hex64('m', 'i', 'n', 'e', 's', 0, 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)minesweep_create;
	p->ondelete = (void*)minesweep_delete;
	p->onstart  = (void*)minesweep_start;
	p->onstop   = (void*)minesweep_stop;
	p->onlist   = (void*)minesweep_list;
	p->onchoose = (void*)minesweep_choose;
	p->onread   = (void*)minesweep_read;
	p->onwrite  = (void*)minesweep_write;
}