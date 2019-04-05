#include "libuser.h"




int schpin_start(struct arena* twig, void* tf, struct arena* root, void* rf)
{
    struct relation* rel;
    struct actor* act;
    if(0 == twig)return 0;

say("@schpin_start{\n");
    rel = twig->orel0;
    while(1)
    {
        if(0 == rel)break;

        if(_act_ == rel->dsttype)
        {
            act = (void*)(rel->dstchip);
            say("%llx:%.8s\n", act, &act->fmt);
        }

        rel = samesrcnextdst(rel);
    }
say("}\n");
    return 0;
}
int schpin_create(struct arena* win, u8* str)
{
#define _vsrc_ hex32('v','s','r','c')
	struct actor* vsrc = actorcreate(_vsrc_, 0);
    if(vsrc)relationcreate(vsrc, 0, _act_, win, 0, _win_);

    return 0;
}