#include "libuser.h"




static int fpscam_sread(
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
static int fpscam_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	//do nothing, pass all to role
	return 0;
}
static void fpscam_cread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void fpscam_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void fpscam_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void fpscam_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
    say("@fpscam_start\n");
}
static void fpscam_delete()
{
}
static void fpscam_create(void* addr)
{
    say("@fpscam_create\n");
}




void fpscam_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('f', 'p', 's', 'c', 'a', 'm', 0, 0);

	p->oncreate = (void*)fpscam_create;
	p->ondelete = (void*)fpscam_delete;
	p->onstart  = (void*)fpscam_start;
	p->onstop   = (void*)fpscam_stop;
	p->onget    = (void*)fpscam_cread;
	p->onpost   = (void*)fpscam_cwrite;
	p->onread   = (void*)fpscam_sread;
	p->onwrite  = (void*)fpscam_swrite;
}