#include "actor.h"
void backgroundcolor(void*, u32);





static void bgpicture_read(struct arena* win, struct actor* act, struct relation* rel)
{
}
static void bgpicture_write(struct event* ev)
{
	say("@bgpic:%x,%x,%x,%x\n",ev->why,ev->what,ev->where,ev->when);
}
static void bgpicture_list()
{
}
static void bgpicture_choose()
{
}
static void bgpicture_stop()
{
}
static void bgpicture_start()
{
}
void bgpicture_delete()
{
}
void bgpicture_create(void* base, struct actor* act)
{
	act->type = hexof('b','g',0,0,0,0,0,0);
	act->name = hexof('b','g','p','i','c',0,0,0);
	act->first = 0;
	act->last = 0;

	act->start = (void*)bgpicture_start;
	act->stop = (void*)bgpicture_stop;
	act->list = (void*)bgpicture_list;
	act->choose = (void*)bgpicture_choose;
	act->read = (void*)bgpicture_read;
	act->write = (void*)bgpicture_write;
}