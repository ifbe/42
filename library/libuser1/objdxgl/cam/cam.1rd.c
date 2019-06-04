#include "libuser.h"




static int firstperson_draw(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	int j;
	struct relation* rel;
	struct actor* tar;
	u8* src;
	u8* dst;

	//3rdcam's target
	rel = act->irel0;
	while(1)
	{
		if(0 == rel)return 0;

		if(_act_ == rel->srctype)
		{
			tar = (void*)(rel->srcchip);
			if(0 == tar)return 0;
			else goto found;
		}

		rel = samedstnextsrc(rel);
	}
	return 0;

found:
	src = (void*)(&tar->camera);
	dst = (void*)(   &win->camera);
	for(j=0;j<sizeof(struct style);j++)dst[j] = src[j];

	carvefrustum(win, &win->camera);
	return 0;
}



static void firstperson_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	firstperson_draw(act, pin, win, sty);
}
static void firstperson_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void firstperson_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void firstperson_start(struct halfrel* self, struct halfrel* peer)
{
    say("@firstperson_start\n");
}




static void firstperson_search(struct actor* act)
{
}
static void firstperson_modify(struct actor* act)
{
}
static void firstperson_delete(struct actor* act)
{
}
static void firstperson_create(struct actor* act, void* addr)
{
    say("@firstperson_create\n");
}




void firstperson_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('1', 'r', 'd', 'c', 'a', 'm', 0, 0);

	p->oncreate = (void*)firstperson_create;
	p->ondelete = (void*)firstperson_delete;
	p->onsearch = (void*)firstperson_search;
	p->onmodify = (void*)firstperson_modify;

	p->onstart = (void*)firstperson_start;
	p->onstop  = (void*)firstperson_stop;
	p->onread  = (void*)firstperson_read;
	p->onwrite = (void*)firstperson_write;
}
