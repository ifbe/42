#include "actor.h"




static void fs_read(struct arena* win, struct actor* act, struct style* sty)
{
}
static void fs_write(struct event* ev)
{
}
static void fs_list()
{
}
static void fs_into()
{
}
static void fs_start()
{
}
static void fs_stop()
{
}
void fs_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('h', 'a', 'c', 'k');
	p->name = hex32('f', 's', 0, 0);

	p->start = (void*)fs_start;
	p->stop = (void*)fs_stop;
	p->list = (void*)fs_list;
	p->choose = (void*)fs_into;
	p->write = (void*)fs_read;
	p->write = (void*)fs_write;
}
void fs_delete()
{
}
