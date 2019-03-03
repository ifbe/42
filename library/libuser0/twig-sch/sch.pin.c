#include "libuser.h"
#define _pin_ hex32('p','i','n',0)




int ppin_start(struct arena* twig, void* tf, struct arena* root, void* rf)
{
    struct relation* rel;
    struct actor* act;
    if(0 == twig)return 0;

say("@ppin_start{\n");
    rel = twig->orel0;
    while(1)
    {
        if(0 == rel)break;

        if(_act_ == rel->dsttype)
        {
            act = (void*)(rel->dstchip);
            say("%llx:%.8s\n", act, &act->name);
        }

        rel = samesrcnextdst(rel);
    }
say("}\n");
    return 0;
}
int ppin_create(struct arena* win, u8* str)
{
	struct actor* p0;
	struct actor* p1;

    p0 = actorcreate(_pin_, 0);
    if(p0)relationcreate(p0, 0, _act_, win, 0, _win_);

    p1 = actorcreate(_pin_, 0);
    if(p1)relationcreate(p1, 0, _act_, win, 0, _win_);

    return 0;
}