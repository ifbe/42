#include "actor.h"




static void gobang_read()
{
}
static void gobang_write()
{
}
static void gobang_list()
{
}
static void gobang_choose()
{
}
static void gobang_stop()
{
}
static void gobang_start()
{
}
static void gobang_delete()
{
}
static void gobang_create()
{
}




void gobang_register(struct actor* p)
{
	p->type = hex32('g', 'a', 'm', 'e');
	p->name = hex64('g', 'o', 'b', 'a', 'n', 'g', 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)gobang_create;
	p->ondelete = (void*)gobang_delete;
	p->onstart  = (void*)gobang_start;
	p->onstop   = (void*)gobang_stop;
	p->onlist   = (void*)gobang_list;
	p->onchoose = (void*)gobang_choose;
	p->onread   = (void*)gobang_read;
	p->onwrite  = (void*)gobang_write;
}