#include "libuser.h"




int schbus_start(struct arena* twig, void* tf, struct arena* root, void* rf)
{
    struct relation* rel;
    struct actor* act;
    if(0 == twig)return 0;

say("@schbus_start{\n");
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
    return 0;
}
int schbus_create(struct arena* win, u8* str)
{
    return 0;
}