#include "libuser.h"




static int firstperson_sread(
	struct actor* act, struct pinid* pin,
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
static int firstperson_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	//do nothing, pass all to role
	return 0;
}
static void firstperson_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void firstperson_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
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
	p->oncread  = (void*)firstperson_cread;
	p->oncwrite = (void*)firstperson_cwrite;
	p->onsread  = (void*)firstperson_sread;
	p->onswrite = (void*)firstperson_swrite;
}
