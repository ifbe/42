#include "libuser.h"



static u8 data[144];




static void mahjong_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void mahjong_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void mahjong_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void mahjong_start(struct halfrel* self, struct halfrel* peer)
{
}




static void mahjong_search(struct actor* act)
{
}
static void mahjong_modify(struct actor* act)
{
}
static void mahjong_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void mahjong_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = data;
	if(_copy_ == act->type)act->buf = memorycreate(144);
}




void mahjong_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('m', 'a', 'h', 'j', 'o', 'n', 'g', 0);

	p->oncreate = (void*)mahjong_create;
	p->ondelete = (void*)mahjong_delete;
	p->onsearch = (void*)mahjong_search;
	p->onmodify = (void*)mahjong_modify;

	p->onstart = (void*)mahjong_start;
	p->onstop  = (void*)mahjong_stop;
	p->onread  = (void*)mahjong_read;
	p->onwrite = (void*)mahjong_write;
}
