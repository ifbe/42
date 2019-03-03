#include "libuser.h"
int actorstart(void*, void*, void*, void*, void*, void*);




int ui3d_sread(struct arena* cc, void* cf, struct arena* win, struct style* stack)
{
	return 0;
}
int ui3d_swrite(struct arena* cc, void* cf, struct arena* win, struct style* sty, struct event* ev)
{
	//say("@ui3d_write\n");
	return 0;
}
int ui3d_stop(struct arena* twig, void* tf, struct arena* root, void* rf)
{
    return 0;
}
int ui3d_start(struct arena* twig, void* tf, struct arena* root, void* rf)
{
	struct relation* rel;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;

	rel = twig->orel0;
	while(1)
	{
		if(0 == rel)break;

		sty = (void*)(rel->srcfoot);
		act = (void*)(rel->dstchip);
		pin = (void*)(rel->dstfoot);
		actorstart(act, pin, twig, sty, root, rf);

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int ui3d_delete(struct arena* win)
{
    return 0;
}
int ui3d_create(struct arena* win, void* str)
{
	return 0;
}
