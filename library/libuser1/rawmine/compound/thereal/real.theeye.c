#include "libuser.h"




static void theeye_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@theeye_read\n");
}
static void theeye_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void theeye_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void theeye_start(struct halfrel* self, struct halfrel* peer)
{
}




static void theeye_search(struct entity* act)
{
}
static void theeye_modify(struct entity* act)
{
}
static void theeye_delete(struct entity* act)
{
}
static void theeye_create(struct entity* act, void* str)
{
}




void theeye_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'h', 'e', 'e', 'y', 'e', 0, 0);

	p->oncreate = (void*)theeye_create;
	p->ondelete = (void*)theeye_delete;
	p->onsearch = (void*)theeye_search;
	p->onmodify = (void*)theeye_modify;

	p->onstart = (void*)theeye_start;
	p->onstop  = (void*)theeye_stop;
	p->onread  = (void*)theeye_read;
	p->onwrite = (void*)theeye_write;
}
