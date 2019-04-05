#include "libuser.h"




static int firstperson_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int j;
	struct relation* rel;
	struct arena* tmpwin;
	struct actor* tmpact;
	u8* src;
	u8* dst;

	rel = win->orel0;
	while(1)
	{
		if(0 == rel)return 0;

		if(_win_ == rel->dsttype)
		{
			tmpwin = (void*)(rel->dstchip);
			if(_ev3d_ == tmpwin->fmt)goto found;
		}

		rel = samesrcnextdst(rel);
	}
	return 0;

found:
	rel = tmpwin->orel0;
	while(1)
	{
		if(0 == rel)return 0;

		if(_act_ == rel->dsttype)
		{
			tmpact = (void*)(rel->dstchip);
			src = (void*)(&tmpact->camera);
			dst = (void*)(   &win->camera);
			for(j=0;j<sizeof(struct style);j++)dst[j] = src[j];
			return 0;
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}
static int firstperson_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	//do nothing, pass all to role
	return 0;
}
static void firstperson_cread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void firstperson_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void firstperson_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void firstperson_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
    say("@firstperson_start\n");
}
static void firstperson_delete()
{
}
static void firstperson_create(void* addr)
{
    say("@firstperson_create\n");
}




void firstperson_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('1', 'r', 'd', 'c', 'a', 'm', 0, 0);

	p->oncreate = (void*)firstperson_create;
	p->ondelete = (void*)firstperson_delete;
	p->onstart  = (void*)firstperson_start;
	p->onstop   = (void*)firstperson_stop;
	p->onget    = (void*)firstperson_cread;
	p->onpost   = (void*)firstperson_cwrite;
	p->onread   = (void*)firstperson_sread;
	p->onwrite  = (void*)firstperson_swrite;
}